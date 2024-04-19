# fhewasm
https://ronantakizawa.github.io/fhewasm/

An FHE scheme (BFV scheme) generation that takes the sum of 2 encrypted numbers. 

The original code was written in C++ using Microsoft SEAL, then compiled into WASM using Emscripten. 

By using WASM, a client can generate a FHE scheme without the server computation, so there is no risk of the server storing the private key of the scheme. 

Microsoft SEAL: https://github.com/microsoft/SEAL

Emscripten: https://github.com/emscripten-core/emsdk

https://github.com/ronantakizawa/fhewasm/assets/71115970/26930b67-46f4-48e1-a339-f59c93fb48e3

