#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include <string.h>
#include <pbc.h>

// arch -x86_64 gcc pa04.c -o pa04.o -lgmp -lpbc -I /usr/local/Cellar/pbc/0.5.14/include/pbc
#define MAX_BUF_SIZE 5000

void remove_spaces(char* s) {
    char* d = s;
    do {
        while (*d == ' ') {
            ++d;
        }
    } while ((*s++ = *d++));
}

int main() {
    FILE* fp = fopen("./input", "r");
    if (!fp) {
        printf("Error opening input file.\n");
        return 1;
    }


    char buf[MAX_BUF_SIZE];
    element_t g1, x, h, pk, sigma;

    pairing_t pairing;
    char param[] = "type a\nq 8780710799663312522437781984754049815806883199414208211028653399266475630880222957078625179422662221423155858769582317459277713367317481324925129998224791\nh 12016012264891146079388821366740534204802954401251311822919615131047207289359704531102844802183906537786776\nr 730750818665451621361119245571504901405976559617\nexp2 159\nexp1 107\nsign1 1\nsign0 1";
    pairing_init_set_buf(pairing, param, strlen(param));

    // Read g1 from input
    fgets(buf, MAX_BUF_SIZE, fp);
    element_init_G1(g1, pairing);
    element_set_str(g1, buf, 10);

    // Initialize h
    element_init_G1(h, pairing);

    // Read message from input
    fgets(buf, MAX_BUF_SIZE, fp);
    element_from_hash(h, buf, strlen(buf));

    // Read private key from input
    fgets(buf, MAX_BUF_SIZE, fp);
    element_init_Zr(x, pairing);
    element_set_str(x, buf, 10);

    // Compute public key
    element_init_G1(pk, pairing);
    element_pow_zn(pk, g1, x);

    // Sign the hash of the message
    element_init_G1(sigma, pairing);
    element_pow_zn(sigma, h, x);

    // Write signature to output
    FILE* output_fp = fopen("./output", "w+");
    if (!output_fp) {
        printf("Error opening output file.\n");
        return 1;
    }
    element_fprintf(output_fp, "%B\n", sigma);
    fclose(output_fp);

    // Clean up
    element_clear(g1);
    element_clear(x);
    element_clear(h);
    element_clear(pk);
    element_clear(sigma);
    pairing_clear(pairing);

    fclose(fp);
    return 0;
}
