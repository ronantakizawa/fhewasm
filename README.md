# fhewasm
https://ronantakizawa.github.io/fhewasm/

An FHE scheme (BFV scheme) generation that takes the sum of 2 encrypted numbers. 

The original code was written in C++ using Microsoft SEAL, then compiled into WASM using emsdk. 

By using WASM, a client can generate a FHE scheme without the server computation, so there is no risk of the server storing the private key of the scheme. 


