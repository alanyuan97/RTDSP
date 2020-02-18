#include <stdlib.h>
#include <stdio.h>
#define N 249
#include "fir_cof.txt"

float CIRBUFF[N]= {0}; // Initialize array2 0s
// float *cir_ptr = CIRBUFF; // Start pointer of cir_array
// float *cir_ptr_end = CIRBUFF[CIRBUFFER-1]; // End pointer of cir_array
// float *coef_ptr = b; // Start of coef array 
// float *coef_end = b[CIRBUFFER-1]; // End of coef array
int offset = 0;

float test_cir(float testin);

int main(){
    FILE *myfile;
    myfile = fopen("output.txt","a");
    fprintf(myfile,"Output: ");
    int i;
    for(i=0;i<sizeof(a);i++){
        fprintf(myfile,"%f ",test_cir(a[i]));
    }
    fclose(myfile);
    return 0;
}

// float cir_fir(float testin){
//     float *buf_val = cir_ptr + offset;
//     *buf_val = testin;
//     float output = 0;
//     while(buf_val>=cir_ptr){
//         output+= (*buf_val) * (*coef_ptr);
//         coef_ptr++;
//         if (buf_val==cir_ptr){
//             break;
//         }
//         else{
//             buf_val--;
//         }
//     }
//     buf_val = cir_ptr + CIRBUFFER -1;

//     while(coef_ptr<=coef_ptr+CIRBUFFER){
//         output+= (*buf_val) * (*coef_ptr);
//         buf_val--;
//         if (coef_ptr==coef_ptr+CIRBUFFER){
//             break;
//         }
//         else{
//             coef_ptr++;
//         }
//     }
//     if (++offset>=CIRBUFFER){
//         offset = 0;
//     }
//     return output;
// }

// float cir_fir(float testin){
//     int sidx,i;
//     int j = 0;
//     float output = 0;

//     sidx = 0 + offset;
//     CIRBUFF[sidx] = testin;

//     for(i=sidx;i>=0;i--){
//         output += b[j]*CIRBUFF[i];
//         j++;
//     }

//     i = CIRBUFFER-1;

//     for(;j<=CIRBUFFER-1;j++){
//         output+=CIRBUFF[i]*b[j];
//         i--;
//     }
//     if (++offset>=CIRBUFFER){
//         offset =0;
//     }

//     return output;
// }

float test_cir(float testin){
	CIRBUFF[offset] = testin;
    int i;
	float output = 0;
	for(i = 0;i<N-offset;i++){
		output+=CIRBUFF[offset+i]*b[i];
	}
	for(i = N-offset;i<N;i++){
		output+=CIRBUFF[offset+i-N]*b[i];
	}
	offset++;
	if (offset==N){offset=0;}
    return output;
}