#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include <time.h>
#include <unistd.h>

//> gcc pa01.c -lgmp

#define MAXLEN 1000
#define ELGML_BITS 2048
#define PK "1234567890123456789012345678901234567890"

void randomPrime(mpz_t, mpz_t, mpz_t, mpz_t);

void randomPrime(mpz_t p, mpz_t q, mpz_t one, mpz_t two) {

    // Initialize Variables
    gmp_randstate_t rndstate;
	gmp_randinit_default(rndstate);
	gmp_randseed_ui(rndstate, (unsigned long)time(NULL));


    // Generate large prime number
    do {
        // Generate a q
        mpz_urandomb(q, rndstate, ELGML_BITS);
        mpz_nextprime(q, q);

        // Get p = 2q + 1
        mpz_mul(p, q, two);
        mpz_add(p, p, one);

    } while(mpz_sizeinbase(p,2) < 2000 || mpz_probab_prime_p(p, 25) < 1);

    // Cleanup
    gmp_randclear(rndstate);
    
}

int main(int argc, char** argv) {

    // Open the input file
    FILE *fp = fopen("./input", "r");
    if (fp == NULL) {
        perror("Error opening file");
        return 1;
    }

    // Declare variables
    mpz_t m, p, q, x;
    mpz_t g, r, h, c1, c2;
    mpz_t ui_one, ui_two, g_test1, g_test2, qr_exp1, qr_exp2;
    gmp_randstate_t g_rndstate;
	gmp_randstate_t r_rndstate;

    // Initialize variables
    mpz_inits(m, p, q, x,g, r, h, c1, c2,ui_one, ui_two, g_test1, g_test2, qr_exp1, qr_exp2, NULL);
    gmp_randinit_default(g_rndstate);
    gmp_randinit_default(r_rndstate);

    // Set constants
    mpz_set_ui(ui_one, 1);
    mpz_set_ui(ui_two, 2);
    mpz_set_str(x, PK, 10);
    
    // Get p and q
    printf("Getting p and q\n\n");
	randomPrime(p, q, ui_one, ui_two);
    gmp_printf("P:\t %Zd\n\nQ:\t %Zd\n", p, q);
    
    // Get (p-1)/2 and (p-1)/q
    mpz_set(qr_exp1, p);
    mpz_sub(qr_exp1, qr_exp1, ui_one);
    mpz_tdiv_q(qr_exp2, qr_exp1, q);
    mpz_tdiv_q(qr_exp1, qr_exp1, ui_two);

    // Read values from the file
    char string[MAXLEN];
    while (fgets(string, MAXLEN, fp) != NULL) {
        if (gmp_sscanf(string, "%Zd", m) < 0)
            fprintf(stderr, "Error parsing line: %s\n", string);
    }

    // Open the output file
    FILE *fp2 = fopen("./output", "w");
    if (fp2 == NULL) {
        perror("Error creating file");
        return 1;
    }

    // Encrypt 3 times
    for (int i = 0; i < 3; i++) {

        // Initialize random states for g and r
	    gmp_randseed_ui(g_rndstate, (unsigned long)time(NULL));
        gmp_randseed_ui(r_rndstate, (unsigned long)time(NULL));

        // Find QR/QNR-safe generator
        do {
            // Get random g in Zp*
            mpz_urandomb(g, g_rndstate, 2000);
            
            // Get g^(p-1)/2
            mpz_powm(g_test1, g, qr_exp1, p);

            // Get g^(p-1)/q
            mpz_powm(g_test2, g, qr_exp2, p);

        } while (mpz_cmp_ui(g_test1, 1) == 0 || mpz_cmp_ui(g_test2, 1) == 0);

        // Square g to create group of QRs
        mpz_powm(g, g, ui_two, p);

        // Get h = g^x
        mpz_powm(h, g, x, p);

        // Find random r
        mpz_urandomb(r, r_rndstate, 2000);

        // Get c1 = g^r
        mpz_powm(c1, g, r, p);

        // Get h^r
        mpz_powm(h, h, r, p);

        // Get c2 = m * h^r
        mpz_mul(c2, m, h);
        mpz_mod(c2, c2, p);

        // Write to file
        gmp_fprintf(fp2, "%Zd,%Zd,%Zd\n", c1,c2,p);
        
        // Ensure time seed is changed
        sleep(1);

    }
    
    // Clear variables
    mpz_clears(m, p, q, x,g, r, h, c1, c2,ui_one, ui_two, g_test1, g_test2, qr_exp1, qr_exp2,
              NULL);

    gmp_randclear(r_rndstate);
	gmp_randclear(g_rndstate);
    
    fclose(fp);
    fclose(fp2);

    return 0;
}
