#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include <time.h>

//> gcc pa01.c -lgmp

#define MAXLEN 1000
#define RSA_BITS 1024

void randomPrime(mpz_t, int);
void encode_message(mpz_t, mpz_t, mpz_t, mpz_t, mpz_t);
void decode_message(mpz_t, mpz_t, mpz_t, mpz_t, mpz_t);

void getExp(mpz_t e, mpz_t phi, mpz_t d) {
    gmp_randstate_t rndstate;
	gmp_randinit_default(rndstate);
	gmp_randseed_ui(rndstate, (unsigned long)time(NULL));

    mpz_urandomm(e, rndstate, phi);
	while(mpz_invert(d,e,phi) == 0){
		mpz_urandomm(e, rndstate, phi);
	}

    gmp_randclear(rndstate);
}


void randomPrime(mpz_t num, int bits) {

    // Initialize Variables
    gmp_randstate_t rndstate;
	gmp_randinit_default(rndstate);
	gmp_randseed_ui(rndstate, (unsigned long)time(NULL));

    // Generate large number
    mpz_urandomb(num, rndstate, 520);
    while(mpz_sizeinbase(num,2) < bits/2){
        mpz_urandomb(num, rndstate, 520);
    }

    gmp_randclear(rndstate);
}

void encode_message(mpz_t m, mpz_t c, mpz_t e, mpz_t d, mpz_t n){

    // Declare the prime numbers, and phi
    mpz_t p, q, phi;

    mpz_t test;
    mpz_init(test);

    // Initialize the prime numbers
    mpz_inits(p, q, phi, NULL);

    // Get the random prime
    randomPrime(p, RSA_BITS);
    mpz_nextprime(p, p);
    mpz_nextprime(q, p);


    // Get n = p * q
    mpz_mul(n, p, q);

    // Get p-1 and q-1
    mpz_sub_ui(p, p, 1);
    mpz_sub_ui(q, q, 1);

    // Get phi = (p-1)*(q-1)
    mpz_mul(phi, p, q);

    // Set e to RSA standard 65537
    getExp(e, phi, d);

    // Get secret key
    mpz_invert(d, e, phi);

    // Encrypt message
    mpz_powm(c, m, e, n);

    // Decrypt
    mpz_powm(test, c, d, n);

    gmp_printf("TEST: %Zd\n", test);
    

    // Clear vars
    mpz_clears(p, q, phi, NULL);
    
}

void decode_message(mpz_t m, mpz_t c, mpz_t d, mpz_t p, mpz_t q){

    // Declare n and phi
    mpz_t n, phi;

    // Initialize variables
    mpz_inits(n, phi, NULL);

    // Get n = p * q
    mpz_mul(n, p, q);

    // Get p-1 and q-1
    mpz_sub_ui(p, p, 1);
    mpz_sub_ui(q, q, 1);

    // Get phi = (p-1)*(q-1)
    mpz_mul(phi, p, q);

    // Decrypt message
    mpz_powm(m, c, d, n);

    // Clean vars
    mpz_clears(n, phi, NULL);
}


int main(int argc, char** argv) {

    // Open the input file
    FILE *fp = fopen("./input", "r");
    if (fp == NULL) {
        perror("Error opening file");
        return 1;
    }

    // Encode variables
    mpz_t m, n, e, d, c;

    // Decode variables
    mpz_t m_prime, p_prime, q_prime, n_prime, e_prime, d_prime, c_prime;

    // Initialize all variables to NULL
    mpz_inits(m, n, e, d, c,
              m_prime, p_prime, q_prime, n_prime, e_prime, d_prime, c_prime,
              NULL);
    
    // Read values from the file
    char string[MAXLEN];
    while (fgets(string, MAXLEN, fp) != NULL) {
        if (gmp_sscanf(string, "%Zd,%Zd,%Zd,%Zd,%Zd", m, c_prime, d_prime, p_prime, q_prime) < 0)
            fprintf(stderr, "Error parsing line: %s\n", string);
    }

    // Encode message
    encode_message(m, c, e, d, n);

    // Decode message
    decode_message(m_prime, c_prime, d_prime, p_prime, q_prime);

    // // DEBUG
    gmp_printf("Encryption:\n");
    gmp_printf("\tMessage: %Zd\n", m);
    gmp_printf("\tNumber: %Zd\n", n);
    gmp_printf("\tExponent: %Zd\n", e);
    gmp_printf("\tPrivate: %Zd\n", d);
    gmp_printf("\tCypher: %Zd\n", c);

    // Open the output file
    FILE *fp2 = fopen("./output", "w");
    if (fp2 == NULL) {
        perror("Error creating file");
        return 1;
    }

    // Write to file
    gmp_fprintf(fp2, "%Zd,%Zd,%Zd,%Zd,%Zd", c, e, d, n, m_prime);
    
    // Clear variables
    mpz_clears(m, n, e, d, c,
              m_prime, p_prime, q_prime, n_prime, e_prime, d_prime, c_prime,
              NULL);

    return 0;
}
