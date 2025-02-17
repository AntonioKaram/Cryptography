#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include <sys/time.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

int main(int argc, const char* argv[]){
    
    mpz_t p,q,n,e,d,m_original,m2,m,c,c2,e2,d2,p2,q2,n2,psub1,qsub1,phi;

    mpz_inits(p,q,n,e,d,m_original,m2,m,c,c2,e2,d2,p2,q2,n2,psub1,qsub1,phi,NULL);
    
    
    FILE *fp;
    char buf[5000];
    fp = fopen("./input", "w+");
    if (fp == NULL){
        printf("\"./input\" was not opened correctly.\n");
        return 1;
    }
    
    gmp_randstate_t rndstate;
    gmp_randinit_default(rndstate);
    
    mpz_set_ui(m_original, (unsigned long)time(NULL) % 10000);

    gmp_randseed(rndstate, m_original);

    mpz_urandomb(p2, rndstate, 512);

    mpz_nextprime(p2, p2);
    mpz_nextprime(q2, p2);

    mpz_mul(n2, p2, q2);
    mpz_sub_ui(psub1,p2,1);
    mpz_sub_ui(qsub1,q2,1);

    mpz_mul(phi,psub1,qsub1);
    

    mpz_urandomm(e2, rndstate, phi);
    while(mpz_invert(d2,e2,phi) == 0){
        mpz_urandomm(e2, rndstate, phi);
    }
    

    // input generation

    mpz_urandomm(m2, rndstate, n2);

    
    mpz_powm(c2, m2, e2, n2);
    
    
    gmp_fprintf(fp, "%Zd,%Zd,%Zd,%Zd,%Zd\n", m_original,c2,d2,p2,q2);
    
    fclose(fp);
    
    gmp_randclear(rndstate);
    
    
    
    double sum = 0;
    double add = 1;
    
    struct timeval begin, end;
    gettimeofday(&begin, 0);
    if (system("./pa01.o") == -1){
        printf("./pa01.o is not found.\n");
        return 1;
    }
    gettimeofday(&end, 0);
    long seconds = end.tv_sec - begin.tv_sec;
    long microseconds = end.tv_usec - begin.tv_usec;
    double time_spent = seconds + microseconds*1e-6;
    // Running the student's program and measuring the execution time
    
    
    
    //
    
    fp = fopen("./output", "r");
    
    if (fp == NULL){
        printf("\"./output\" does not exist. Make sure you are following the instruction to generate the output file.\n");
        return 1;
    }
    
    fgets(buf, 5000, (FILE*)fp);
	gmp_sscanf(buf, "%Zd,%Zd,%Zd,%Zd,%Zd", c, e, d, n, m2);
    fclose(fp);
    // Reading in the output of the student's program

    
    
    // below is the portion of actual grading
	int score = 100;
    
    char msg[3000]; // This stores the message shown to the students
    strcpy(msg, "");
    
	if(mpz_sizeinbase(n,2) < 1000){
		score -= 15;
		strcat(msg, "n is smaller than 1000 bits, which means either p or q (or both) is smaller than 500 bits.\\n");
		sprintf(buf, "Your n only has %d bit.\\nDeducting 15 points.\\n\\n", (int)mpz_sizeinbase(n,2));
        strcat(msg, buf);
	}


	if(mpz_cmp(c,n) > 0){
		score -= 15;
		strcat(msg, "c is larger than n (invalid cipher text format).\\n");
		gmp_sprintf(buf, "Your n is: %Zd\\n", n);
        strcat(msg, buf);
		gmp_sprintf(buf, "Your c is: %Zd\\nDeducting 15 points.\\n\\n", c);
        strcat(msg, buf);
		mpz_mod(c, c, n); // correct the issue for further grading.
	}
	mpz_t c_temp;
	mpz_init(c_temp);

	mpz_powm(c_temp,m_original,e,n);
	
	if(mpz_cmp(c,c_temp) != 0){
		score -= 15;
		strcat(msg, "The original m and the student-provided e cannot be used to generate the given c.\\n");
		gmp_sprintf(buf, "%Zd\\nwas supposed to be generated, but\\n%Zd\\nis generated instead.\\nDeducting 15 points.\\n\\n", c,c_temp);
        strcat(msg, buf);
	}

	
	mpz_powm(m,c,d,n);

	if(mpz_cmp(m_original,m) != 0){
		score -= 15;
		strcat(msg, "The student-provided d cannot be used to decrypt c and get the correct m.\\n");
		gmp_sprintf(buf, "%Zd was originally encrypted, but %Zd is decrypted from c.\\nDeducting 15 points.\\n\\n", m_original,m);
        strcat(msg, buf);
	}
    
	
	mpz_t m_temp;
	mpz_init(m_temp);
	

	mpz_mul(n2, p2, q2);
	mpz_powm(m_temp, c2, d2, n2);

	if(mpz_cmp(m2, m_temp) != 0){
		score -= 15;
		strcat(msg, "The student-provided m' is not equal to the originally encrypted message.\\nDeducting 15 points.\\n\\n");
	}
    
    

    if(score == 100) strcpy(msg, "Perfect!\\n");
    
    
    
    
    sprintf(buf, "The final score is:%d\\nExecution time of your code is %.9f seconds", score, time_spent);
	strcat(msg, buf);
    
    
    char str[5000];
    
    
    sprintf(str, "{\
            \"score\": %d,\
              \"output\": \"%s\",\
              \"visibility\": \"visible\",\
              \"stdout_visibility\": \"visible\",\
              \"leaderboard\": [ \
                  {\"name\": \"Execution Time (seconds)\", \"value\": %.9f, \"order\": \"asc\"}\
                ] \
            }", score, msg, time_spent);
    
    
    FILE *jsonp;
    jsonp = fopen("../results/results.json", "w+");
    fprintf(jsonp, "%s\n", str);
    fclose(jsonp);
    
    // Writing the JSON file for the grading and the leaderboard -- specification defined by the Gradescope Autograding.
     
    
	mpz_clears(p,q,n,e,d,m_original,m2,m,c,c2,e2,d2,p2,q2,n2,psub1,qsub1,phi,m_temp,c_temp,NULL);

    return 0;
}
		
