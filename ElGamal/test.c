#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>


void remove_spaces(char* s) {
    char* d = s;
    do {
        while (*d == ' ') {
            ++d;
        }
    } while ((*s++ = *d++));
}

int main(int argc, const char* argv[]){
    
    
    FILE *fp;
    char buf[5000];
    
    size_t c1size,c2size,psize;
    mpz_t c1,c2,x,p,q,m,m_original,c1p,c2p,c1pp,c2pp,c1x,c1x_inv,p_sample,h,g,g2,gq,psub1;

    mpz_inits(c1,c2,x,p,q,m,m_original,c1p,c2p,c1pp,c2pp,c1x,c1x_inv,p_sample,h,g,g2,gq,psub1,NULL);
    
    fp = fopen("./input", "w+");

    if (fp == NULL){
        printf("\"./input\" does not exist. Make sure you are following the instruction to generate the input file.\n");
        return 1;
    }
    
    gmp_randstate_t rndstate;
    gmp_randinit_default(rndstate);
    
    gmp_randseed_ui(rndstate, (unsigned long)time(NULL));
    
    mpz_urandomb(m, rndstate, 200);
    mpz_mul(m,m,m);


    gmp_fprintf(fp, "%Zd\n", m);
    mpz_set(m_original, m);
    gmp_printf("m from the grading program: %Zd\n", m);
    gmp_randclear(rndstate);
    
    
    
    fclose(fp);
    
    double sum = 0;
    double add = 1;
    
    
    
    
    
    
    
    struct timeval begin, end;
    gettimeofday(&begin, 0);
    if (system("./pa02.o") == -1){
        printf("You did not generate ./pa02.o as the executable or the ./pa02.o you generated is not an executable.\n");
        return 1;
    }
    gettimeofday(&end, 0);
    long seconds = end.tv_sec - begin.tv_sec;
    long microseconds = end.tv_usec - begin.tv_usec;
    double time_spent = seconds + microseconds*1e-6;
    // Running the student's program and measuring the execution time
    
    
    
    
    
    
    char msg[3000]; // This stores the message shown to the students
    strcpy(msg, "");


    int score = 100;

    //
    
    fp = fopen("./output", "r");
    if (fp == NULL){
        printf("\"./output\" does not exist. Make sure you are following the instruction to generate the output file.\n");
        return 1;
    }
    fgets(buf, 5000, (FILE*)fp);
    gmp_sscanf(buf, "%Zd,%Zd,%Zd", c1, c2, p);
    gmp_printf("%Zd,%Zd,%Zd\n", c1, c2, p);

    mpz_set_ui(c1pp, 0);

    if(fgets(buf, 5000, (FILE*)fp) != NULL){
        gmp_sscanf(buf, "%Zd,%Zd,%Zd", c1p, c2p, p);
    }
    if(fgets(buf, 5000, (FILE*)fp) != NULL){
        gmp_sscanf(buf, "%Zd,%Zd,%Zd", c1pp, c2pp, p);
    }


    fclose(fp);
    // Reading in the output of the student's program. If not enough are provided, deduct points.

    if(mpz_cmp_ui(c1pp, 0) == 0 || mpz_cmp(c1, c1p) == 0 || mpz_cmp(c1, c1pp) == 0 || mpz_cmp(c1p, c1pp) == 0){
        score -= 15;
        strcat(msg, "Did not submit 3 different ciphertexts.\nDeducting 15 points.\n\n");
    }

    
    mpz_set_str(x, "1234567890123456789012345678901234567890", 10);


    mpz_sub_ui(q,p,1);
    mpz_fdiv_q_ui(q,q,2);
    
    
    if(mpz_probab_prime_p(q, 30) == 0){
        score -= 15;
        strcat(msg, "p is not a prime number such that p=2q+1 with a prime q.\nDeducting 15 points.\n\n");
    }

    mpz_powm(c1x, c1, x, p);
    mpz_invert(c1x_inv,c1x,p);

    mpz_mul(m,c2,c1x_inv);
    mpz_mod(m, m, p);

    if(mpz_cmp(m,m_original) != 0){
        gmp_printf("m decrypted from the ciphertext: %Zd\n\nThe original m:%Zd\n", m, m_original);
        strcat(msg, "Decryption failed.\nDeducting 30 points.\n\n");
        score -= 30;
    }    
    
    c1size = mpz_sizeinbase(c1,2);
    c2size = mpz_sizeinbase(c2,2);
    psize = mpz_sizeinbase(p,2);

    if(c1size < 2000 || c2size < 2000 || psize < 2000){
        strcat(msg, "The modulus p is too small.\nDeducting 15 points.\n\n");
        score -= 15;
    }


    if(mpz_legendre(c1,p) < 0 || mpz_legendre(c2,p) < 0){
        strcat(msg, "The encryption is vulnerable to QR/QNR attacks.\nDeducting 15 points.\n\n");
        score -= 15;
    }


    mpz_set_str(p_sample,"4923937588039918804427951299623877970347581287822319855049505696077427996562952587204906321210815567264315450632017525957026015898059295716161235934853556223569141338628396223661909918842259827918309911026651196109890852003702360144939511319968929525354597052211292013975896945103920511165090380311209626097685836008001150284831598722340647556649813332646500494637154924058232807703020059671268436952641494895867455782762321135469053374873337130859698683451654553424001181614287444450762242733560173529424680803115432303795858540001397583822469909768559929259955959430511238719572928763873889370504441785176303642963", 10);
    if(mpz_cmp(p,p_sample) == 0){
        strcat(msg, "Used the p in the sample.\nDeducting 15 points.\n");
        score -= 15;
    }
    
    int usedInsecureRandom = 0;
    int hardcopied = 0;
    fp = fopen("./pa02.c", "r");
    if (fp == NULL){
        fp = fopen("./pa02.cpp", "r");
    }
    while(fgets(buf, 5000, (FILE*)fp) != NULL){
        
        remove_spaces(buf);
        if(strlen(buf) >= 600){
            hardcopied = 1;
        }
        if(strstr(buf, "rand()") != NULL){
            usedInsecureRandom = 1;
        }
    }
    if(usedInsecureRandom == 1){
        score -= 15;
        strcat(msg, "The source code contains rand(), which is an insecure random number generator.\\nDeducting 15 points.\\n\\n");
    }
    if(hardcopied == 1){
        score -= 15;
        strcat(msg, "The source code contains hardcopied p value.\\nDeducting 15 points.\\n\\n");
    }
    fclose(fp);

    if(score == 100) strcpy(msg, "Perfect!\n");
    
    // Grading is finished.
    
    sprintf(buf, "The final score is:%d\n\n Execution time of your code is %.9f seconds", score, time_spent);
	strcat(msg, buf);
    printf("%s\n", msg);

	mpz_clears(c1,c2,x,p,q,m,m_original,c1p,c2p,c1pp,c2pp,c1x,c1x_inv,p_sample,h,g,g2,gq,psub1,NULL);
    return 0;
}
		
