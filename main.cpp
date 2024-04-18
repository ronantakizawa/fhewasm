#include "seal/seal.h"
#include <emscripten.h>
#include <iostream>
#include <vector>
#include <unordered_map>

using namespace std;
using namespace seal;

// Global objects for SEAL context and tools
unique_ptr<SEALContext> context;
unique_ptr<KeyGenerator> keygen;
unique_ptr<PublicKey> public_key;
unique_ptr<Encryptor> encryptor;
unique_ptr<Evaluator> evaluator;
unique_ptr<Decryptor> decryptor;
unique_ptr<BatchEncoder> encoder;

// Storage for Ciphertext objects
unordered_map<int, Ciphertext> cipher_storage;
int cipher_id_counter = 0;

// Initialize SEAL components
extern "C" {
    EMSCRIPTEN_KEEPALIVE
    void initialize_seal() {
        EncryptionParameters params(scheme_type::bgv);
        size_t poly_modulus_degree = 8192;
        params.set_poly_modulus_degree(poly_modulus_degree);
        params.set_coeff_modulus(CoeffModulus::Create(poly_modulus_degree, { 60, 40, 40, 60}));
        params.set_plain_modulus(PlainModulus::Batching(poly_modulus_degree, 20));

        context = make_unique<SEALContext>(params);
        keygen = make_unique<KeyGenerator>(*context);
        public_key = make_unique<PublicKey>();
        keygen->create_public_key(*public_key);

        encryptor = make_unique<Encryptor>(*context, *public_key);
        evaluator = make_unique<Evaluator>(*context);
        decryptor = make_unique<Decryptor>(*context, keygen->secret_key());
        encoder = make_unique<BatchEncoder>(*context);
    }

    EMSCRIPTEN_KEEPALIVE
    int encrypt_integer(int value) {
        vector<uint64_t> vec(encoder->slot_count(), 0);
        vec[0] = value;
        Plaintext plain;
        encoder->encode(vec, plain);
        Ciphertext encrypted;
        encryptor->encrypt(plain, encrypted);
        
        int id = cipher_id_counter++;
        cipher_storage[id] = std::move(encrypted);
        return id;
    }

    EMSCRIPTEN_KEEPALIVE
    int add_encrypted_numbers(int id1, int id2) {
    if (cipher_storage.find(id1) == cipher_storage.end() || cipher_storage.find(id2) == cipher_storage.end()) {
        std::cerr << "One or both ciphertext IDs not found." << std::endl;
        return -1; // Return an error code or handle it more gracefully
    }

    const Ciphertext& encrypted1 = cipher_storage[id1];
    const Ciphertext& encrypted2 = cipher_storage[id2];
    Ciphertext encrypted_sum;
    evaluator->add(encrypted1, encrypted2, encrypted_sum);

    int sum_id = cipher_id_counter++;
    cipher_storage[sum_id] = std::move(encrypted_sum);
    return sum_id;
}

    EMSCRIPTEN_KEEPALIVE
    int decrypt_result(int id) {
        const Ciphertext& encrypted_result = cipher_storage[id];
        Plaintext plain_result;
        decryptor->decrypt(encrypted_result, plain_result);
        vector<uint64_t> result;
        encoder->decode(plain_result, result);
        return static_cast<int>(result[0]);  // Assuming the result is in the first slot
    }

    EMSCRIPTEN_KEEPALIVE
    void free_ciphertext(int id) {
        cipher_storage.erase(id);
    }
}
