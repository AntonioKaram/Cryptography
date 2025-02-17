#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <complex>
#include <math.h>


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
    
    
    
    struct timeval begin, end;
    gettimeofday(&begin, 0);
    
    if (system("./TestHEAAN 1.00001") == -1){
        printf("Your submission did not generate TestHEAAN correctly. Something is wrong. Abort.\n");
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
    
    std::string line;
    std::ifstream fin ("./output");
    std::complex<double> dresult;
    if(fin.is_open()){
        getline(fin, line);
        std::istringstream is(line);
        is >> dresult;
        fin.close();
    }
    else{
        printf("Output was not found. Something is wrong. Abort.\n");
        return 1;
    }
    
    
    
    if(pow(1.00001, 128) + 2 * pow(1.00001,17) - 3 * pow(1.00001, 3) - dresult.real() > 0.01 || pow(1.00001, 128) + 2 * pow(1.00001,17) - 3 * pow(1.00001, 3) - dresult.real() < -0.01) {
        score = 0;
        strcpy(msg, "Your ciphertext does not decrypt to x^128 + 2x^17 - 3x^3 correctly.\\nDeducting 100 pts.\\n");
    }

    if(pow(1.00001, 128) + 2 * pow(1.00001,17) - 3 * pow(1.00001, 3) - dresult.real() < 0.000001 && pow(1.00001, 128) + 2 * pow(1.00001,17) - 3 * pow(1.00001, 3) - dresult.real() > -0.000001) {
        score = 0;
        strcpy(msg, "Accuracy is too good to be true.\\nDeducting 100 pts.\\n");
    }

    if(time_spent < 5.0) {
        score = 0;
        strcpy(msg, "Runtime is too good to be true.\\nDeducting 100 pts.\\n");
    }
    
    

    if(score == 100) strcpy(msg, "Perfect!\\n");
    
    // Grading is finished.
    
    sprintf(buf, "The final score is:%d\\n\\n Execution time of your code is %.9f seconds", score, time_spent);
	
    puts(buf);

    return 0;
}