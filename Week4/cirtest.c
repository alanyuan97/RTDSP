#include <stdlib.h>
#include <stdio.h>
#define CIRBUFFER 249
#include "fir_cof.txt"

float CIRBUFF[CIRBUFFER]= {0}; // Initialize array2 0s
float *cir_ptr = CIRBUFF; // Start pointer of cir_array
// float *cir_ptr_end = CIRBUFF[CIRBUFFER-1]; // End pointer of cir_array
float *coef_ptr = b; // Start of coef array 
// float *coef_end = b[CIRBUFFER-1]; // End of coef array
int offset = 0;

float cir_fir(float testin);

int main(){
    FILE *myfile;
    myfile = fopen("output.txt","a");
    fprintf(myfile,"Output: ");
    int i;
    for(i=0;i<sizeof(a);i++){
        fprintf(myfile,"%f,",cir_fir(a[i]));
    }
    fclose(myfile);
    return 0;
}

float cir_fir(float testin){
    float *buf_val = cir_ptr + offset;
    *buf_val = testin;
    float output = 0;
    while(buf_val>=cir_ptr){
        output+= (*buf_val--) * (*coef_ptr++);
    }
    buf_val = cir_ptr + CIRBUFFER -1;

    while(coef_ptr<=coef_ptr+CIRBUFFER){
        output+= (*buf_val--) * (*coef_ptr++);
    }
    if (++offset>=CIRBUFFER){
        offset = 0;
    }
    return output;
}