/*
* Copyright (c) by CryptoLab inc.
* This program is licensed under a
* Creative Commons Attribution-NonCommercial 3.0 Unported License.
* You should have received a copy of the license along with this
* work.  If not, see <http://creativecommons.org/licenses/by-nc/3.0/>.
*/

#include "../src/HEAAN.h"
#include <stdlib.h>
#include <iostream>

using namespace std;
using namespace heaan;
using namespace NTL;

/**
  * This file is for test HEAAN library
  * You can find more in src/TestScheme.h
  * "./TestHEAAN Encrypt" will run Encrypt Test
  * There are Encrypt, EncryptSingle, Add, Mult, iMult, RotateFast, Conjugate Tests
  */
int main(int argc, char **argv) {

	long logq = 160; ///< Log of the ciphertext modulus q.
	long logp = 30; ///< Real message will be quantized by multiplying 2^30. This is the log of the Delta in the CKKS scheme.
    /// Decryption suceeds with sufficient precision/accuracy only when logp is much smaller than logq (e.g., smaller by 10).
    
    long logn = 0; ///< log2(The number of slots) <-- Students may ignore this parameter.
    long n = 1 << logn; ///  <-- Students may ignore this parameter.
    long logq_bootstrap = 40; ///  <-- Students may ignore this parameter.
    long logT = 4; ///< this means that we use Taylor approximation in [-1/T,1/T] with double angle fomula  <-- Students may ignore this parameter.
    
    srand(time(NULL));
    SetNumThreads(4);
    Ring ring;
    SecretKey secretKey(ring);
    Scheme scheme(secretKey, ring); /// <-- This is where the scheme is set up, the secretKey is chosen, and the public key and the evaluation key is chosen. All public parameters are included in the class object "scheme".
    scheme.addBootKey(secretKey, logn, logq_bootstrap + 4); /// <-- This is where the bootstrapping key (i.e., encrypted secretKey and other auxiliary parameters) is added to the scheme.
    
    complex<double>* mval = new complex<double>[n]; /// <-- This is the value to encrypt.
    complex<double>* mresult = new complex<double>[n]; /// <-- This will store the desired result.
    for(long i = 0; i < n; i++) {
        mval[i] = atof(argv[1]); /// <-- My grading program will provide the input to be encrypted as argv[1]
    }
    
    
    Ciphertext cipher;
    
    scheme.encrypt(cipher, mval, n, logp, logq); /// <-- mval, i.e., the initial value, is encrypted here.
    

    /*
     
     Student input starts.
     
     */
    
    // Compute x^128 + 2x^17 - 3x^3 using repeated squaring, multiplication, addition, and subtraction
	Ciphertext cipher_x128, cipher_x17, cipher_x3;

	// Compute x^128
	cipher_x128 = cipher;
	for(int i = 0; i < 7; i++) {
		scheme.squareAndEqual(cipher_x128);
		scheme.reScaleByAndEqual(cipher_x128, logp);
		if (cipher_x128.logq - 10 < cipher_x128.logp) {
			scheme.bootstrapAndEqual(cipher_x128, logq_bootstrap, logQ, logT);
		}
	}

	// Compute 2x^17
	cipher_x17 = cipher;
	for(int i = 0; i < 4; i++) {
		scheme.squareAndEqual(cipher_x17);
		scheme.reScaleByAndEqual(cipher_x17, logp);
		if (cipher_x17.logq - 10 < cipher_x17.logp) {
			scheme.bootstrapAndEqual(cipher_x17, logq_bootstrap, logQ, logT);
		}
	}

    scheme.multAndEqual(cipher_x17, cipher);
	scheme.reScaleByAndEqual(cipher_x17, logp);


	scheme.multByConstAndEqual(cipher_x17, 2, logp);
	scheme.reScaleByAndEqual(cipher_x17, logp);
	scheme.bootstrapAndEqual(cipher_x17, logq_bootstrap, logQ, logT);


	// Compute -3x^3
	cipher_x3 = cipher;
	scheme.squareAndEqual(cipher_x3);
	scheme.reScaleByAndEqual(cipher_x3, logp);
	if (cipher_x3.logq - 10 < cipher_x3.logp) {
		scheme.bootstrapAndEqual(cipher_x3, logq_bootstrap, logQ, logT);
	}

    scheme.multAndEqual(cipher_x3, cipher);
	scheme.reScaleByAndEqual(cipher_x3, logp);

	scheme.multByConstAndEqual(cipher_x3, -3, logp);
	scheme.reScaleByAndEqual(cipher_x3, logp);

	scheme.bootstrapAndEqual(cipher_x3, logq_bootstrap, logQ, logT);


	// Compute x^128 + 2x^17 - 3x^3
	scheme.addAndEqual(cipher_x128, cipher_x17);
	scheme.addAndEqual(cipher_x128, cipher_x3);

	cipher = cipher_x128;

    
    /*
     
     Student input ends.
     
     */
    
    
    complex<double>* dresult = scheme.decrypt(secretKey, cipher); /// <-- This will decrypt the final cipher
    StringUtils::compare(mresult, dresult, n, "result"); /// <-- This will compare mresult (the x^1024 that was computed with plaintext data) and dresult (the outcome of the decryption). The difference is calculated and shown as eresult, which needs to be small enough to pass the grading.
    
    
    ofstream fout;
    fout.open("output");
    fout << dresult[0];
    fout.close();
    
    

	return 0;
     
}