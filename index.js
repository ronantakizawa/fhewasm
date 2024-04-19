var sealModule;

        function readBinaryData(ptr, length) {
            const data = new Uint8Array(sealModule.HEAPU8.buffer, ptr, length);
            let binaryString = '';
            for (let i = 0; i < length; i++) {
                binaryString += String.fromCharCode(data[i]);
            }
            return window.btoa(binaryString);
        }

        Module.onRuntimeInitialized = async function() {
            sealModule = Module;
            sealModule._initialize_seal();
        };
    
        function encryptAndAdd() {
            if (!sealModule) {
                document.getElementById('publicKeyDisplay').innerText = 'SEAL module is not yet initialized';
                return;
            }

            let num1 = parseInt(document.getElementById('num1').value);
            let num2 = parseInt(document.getElementById('num2').value);

            let encrypted1 = sealModule._encrypt_integer(num1);
            let encrypted2 = sealModule._encrypt_integer(num2);
            let resultId = sealModule._add_encrypted_numbers(encrypted1, encrypted2);
            let decryptedResult = sealModule._decrypt_result(resultId);
            document.getElementById('decryptedResultDisplay').innerText = decryptedResult;


            let pub = sealModule._get_public_key();
            let publicKey = readBinaryData(pub, 400);
            document.getElementById('publicKeyDisplay').innerText = publicKey;
            let priv = sealModule._get_private_key();
            let privateKey = readBinaryData(priv, 400);
            document.getElementById('privateKeyDisplay').innerText = privateKey;
            let hash1 = sealModule._serialize_encrypted_data(encrypted1);
            let hash2 = sealModule._serialize_encrypted_data(encrypted2);
            document.getElementById('encryptedNum1Display').innerText = readBinaryData(hash1, 400);
            document.getElementById('encryptedNum2Display').innerText = readBinaryData(hash2, 400);

            let hash3 = sealModule._serialize_encrypted_data(resultId);
            document.getElementById('encryptedSumDisplay').innerText = readBinaryData(hash3, 400);

            
            sealModule._clear_all_data();
            
        }