
# Cryptography Implementations

This repository contains implementations of various cryptographic algorithms and protocols including:

## Algorithms

### 1. RSA (Rivest-Shamir-Adleman)
Located in [RSA/](RSA/) directory
- Implementation of RSA encryption/decryption
- Includes key generation, encryption and decryption functions
- Uses GMP library for large number operations

### 2. ElGamal
Located in [ElGamal/](ElGamal/) directory
- Implementation of ElGamal public-key cryptosystem
- Includes key generation and encryption/decryption
- Features QR/QNR-safe generator selection
- Uses GMP library for modular arithmetic

### 3. BLS Signatures
Located in [BLS Signature/](BLS%20Signature/) directory
- Implementation of Boneh-Lynn-Shacham signature scheme
- Uses pairing-based cryptography (PBC) library
- Supports signature generation and verification

### 4. HEAAN (Homomorphic Encryption)
Located in [HEAAN/](HEAAN/) directory
- Implementation of homomorphic encryption for approximate arithmetic
- Supports fixed-point arithmetic operations
- Based on the "Homomorphic Encryption for Arithmetic of Approximate Numbers" paper
- Uses NTL library for polynomial operations

### 5. DES (Data Encryption Standard)
Located in [DES/](DES) directory
- Python implementation of DES algorithm
- Supports both encryption and decryption
- Includes key schedule generation
- Shows intermediate steps during encryption/decryption

## Dependencies

- GMP (GNU Multiple Precision Arithmetic Library)
- PBC (Pairing-Based Cryptography Library)
- NTL (Number Theory Library)
- Python 3.x for DES implementation

## Contributors
* Original HEAAN implementation by CryptoLab inc. (my code is in the run subdirectory)
* All other implementations by Antonio Karam