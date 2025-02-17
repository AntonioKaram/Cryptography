#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>
#include <pbc.h>


#define message1 "CSE 40622 is really fun!!!!!!\n"

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
    fp = fopen("./input", "w+");
    
    char s[3000] = "type a\nq 8780710799663312522437781984754049815806883199414208211028653399266475630880222957078625179422662221423155858769582317459277713367317481324925129998224791\nh 12016012264891146079388821366740534204802954401251311822919615131047207289359704531102844802183906537786776\nr 730750818665451621361119245571504901405976559617\nexp2 159\nexp1 107\nsign1 1\nsign0 1\n";

    pairing_t pairing;
    element_t x,pk,g1,h,sigma;

    if (pairing_init_set_buf(pairing,s,strlen(s))) pbc_die("Pairing initialization failed.");
    if (!pairing_is_symmetric(pairing)) pbc_die("pairing must be symmetric");

    element_init_Zr(x, pairing);
    element_init_G1(g1, pairing);
    element_init_G1(pk, pairing);
    element_init_G1(h, pairing);
    element_init_G1(sigma, pairing);
    
    element_random(g1);
    
    element_random(x);
    
    element_fprintf(fp, "%B\n%s%B", g1, message1, x);
    
    
    
    fclose(fp);
    
    double sum = 0;
    double add = 1;
    
    
    
    
    
    
    
    struct timeval begin, end;
    gettimeofday(&begin, 0);
    if (system("./pa04.o") == -1){
        printf("You did not generate ./pa04.o as the executable or the ./pa04.o you generated is not an executable.\n");
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
    fclose(fp);
    element_set_str(sigma, buf, 10);

    
    
    element_from_hash(h, message1, strlen(message1));
    element_pow_zn(pk, g1, x);


    element_t temp1,temp2;

    element_init_GT(temp1, pairing);
    element_init_GT(temp2, pairing);

    element_pairing(temp1, h, pk);
    element_pairing(temp2, sigma, g1);
    element_fprintf(stdout, "%B\n", temp1);
    element_fprintf(stdout, "%B\n", temp2);

    
    
    
    
    // Reading in the output of the student's program. If not enough are provided, deduct points.

    
    if(element_cmp(temp1, temp2) != 0){
        score -= 40;
        strcat(msg, "The signature is not correctly generated.\\nDeducting 40 points.\\n\\n");
        
    }

    
    
    
    int usedTestH = 0;
    fp = fopen("./pa04.c", "r");
    if (fp == NULL){
        fp = fopen("./pa04.cpp", "r");
    }
    while(fgets(buf, 5000, (FILE*)fp) != NULL){
        
        remove_spaces(buf);
        if(strstr(buf, "pbc_test.h") != NULL){
            usedTestH = 1;
            break;
        }
    }
    if(usedTestH == 1){
        score -= 40;
        strcat(msg, "The source code contains pbc_test.h, which is not allowed.\\nDeducting 40 points.\\n\\n");
    }
    fclose(fp);
    

    if(score == 100) strcpy(msg, "Perfect!\\n");
    
    // Grading is finished.
    
    sprintf(buf, "The final score is:%d\\n\\n Execution time of your code is %.9f seconds", score, time_spent);
	strcat(msg, buf);
    puts(msg);
    
    
    
    // Writing the JSON file for the grading and the leaderboard -- specification defined by the Gradescope Autograding.
     
    element_clear(x);
    element_clear(g1);
    element_clear(pk);
    element_clear(h);
    element_clear(sigma);
    pairing_clear(pairing);
    return 0;
}
		
