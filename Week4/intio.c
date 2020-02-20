/*************************************************************************************
			       DEPARTMENT OF ELECTRICAL AND ELECTRONIC ENGINEERING
					   		     IMPERIAL COLLEGE LONDON

 				      EE 3.19: Real Time Digital Signal Processing
					       Dr Paul Mitcheson and Daniel Harvey

				        		  LAB 3: Interrupt I/O

 				            ********* I N T I O. C **********

  Demonstrates inputing and outputing data from the DSK's audio port using interrupts.

 *************************************************************************************
 				Updated for use on 6713 DSK by Danny Harvey: May-Aug 2006
				Updated for CCS V4 Sept 10
 ************************************************************************************/
/*
 *	You should modify the code so that interrupts are used to service the
 *  audio port.
 */
/**************************** Pre-processor statements ******************************/

#include <stdlib.h>
//  Included so program can make use of DSP/BIOS configuration tool.
#include "dsp_bios_cfg.h"

/* The file dsk6713.h must be included in every program that uses the BSL.  This
   example also includes dsk6713_aic23.h because it uses the
   AIC23 codec module (audio interface). */
#include "dsk6713.h"
#include "dsk6713_aic23.h"

// math library (trig functions)
#include <math.h>
#include <fir_cof.txt>
// Some functions to help with writing/reading the audio ports when using interrupts.
#include <helper_functions_ISR.h>


#define PI 3.141592653589793
#define SINE_TABLE_SIZE 256
#define filter_size 249
#define N 249
/******************************* Global declarations ********************************/

/* Audio port configuration settings: these values set registers in the AIC23 audio
   interface to configure it. See TI doc SLWS106D 3-3 to 3-10 for more info. */
DSK6713_AIC23_Config Config = { \
			 /**********************************************************************/
			 /*   REGISTER	            FUNCTION			      SETTINGS         */
			 /**********************************************************************/\
    0x0017,  /* 0 LEFTINVOL  Left line input channel volume  0dB                   */\
    0x0017,  /* 1 RIGHTINVOL Right line input channel volume 0dB                   */\
    0x01f9,  /* 2 LEFTHPVOL  Left channel headphone volume   0dB                   */\
    0x01f9,  /* 3 RIGHTHPVOL Right channel headphone volume  0dB                   */\
    0x0011,  /* 4 ANAPATH    Analog audio path control       DAC on, Mic boost 20dB*/\
    0x0000,  /* 5 DIGPATH    Digital audio path control      All Filters off       */\
    0x0000,  /* 6 DPOWERDOWN Power down control              All Hardware on       */\
    0x0043,  /* 7 DIGIF      Digital audio interface format  16 bit                */\
    0x008d,  /* 8 SAMPLERATE Sample rate control             8 KHZ                 */\
    0x0001   /* 9 DIGACT     Digital interface activation    On                    */\
			 /**********************************************************************/
};

int sample;
float table[SINE_TABLE_SIZE];
double input[filter_size]={0};


double* cir_ptr = &input[0]; // Head pointer to input array
//double* cir_ptr_end = cir_ptr + filter_size ; // End pointer to input array
double* coef_ptr = &b[0]; // Head pointer to coef array
//double* coef_ptr_end = b +filter_size; // End pointer to coef array

int index=0;
int offset=0;
double output=0;
// Codec handle:- a variable used to identify audio interface
DSK6713_AIC23_CodecHandle H_Codec;

 /******************************* Function prototypes ********************************/
void init_hardware(void);
void init_HWI(void);
void opt_cir();
void InterupptSR();
void test_cir();
void ptr_cir();
/********************************** Main routine ************************************/
void main(){


	// initialize board and the audio port
  init_hardware();

  /* initialize hardware interrupts */
  init_HWI();
//  sine_init();


  /* loop indefinitely, waiting for interrupts */
  while(1)
  {};

}

/********************************** init_hardware() **********************************/
void init_hardware()
{
    // Initialize the board support library, must be called first
    DSK6713_init();

    // Start the AIC23 codec using the settings defined above in config
    H_Codec = DSK6713_AIC23_openCodec(0, &Config);

	/* Function below sets the number of bits in word used by MSBSP (serial port) for
	receives from AIC23 (audio port). We are using a 32 bit packet containing two
	16 bit numbers hence 32BIT is set for  receive */
	MCBSP_FSETS(RCR1, RWDLEN1, 32BIT);

	/* Configures interrupt to activate on each consecutive available 32 bits
	from Audio port hence an interrupt is generated for each L & R sample pair */
	MCBSP_FSETS(SPCR1, RINTM, FRM);

	/* These commands do the same thing as above but applied to data transfers to
	the audio port */
	MCBSP_FSETS(XCR1, XWDLEN1, 32BIT);
	MCBSP_FSETS(SPCR1, XINTM, FRM);


}

/********************************** init_HWI() **************************************/
void init_HWI(void)
{
	IRQ_globalDisable();			// Globally disables interrupts
	IRQ_nmiEnable();				// Enables the NMI interrupt (used by the debugger)
	IRQ_map(IRQ_EVT_RINT1,4);		// Maps an event to a physical interrupt
	IRQ_enable(IRQ_EVT_RINT1);		// Enables the event
	IRQ_globalEnable();				// Globally enables interrupts

}

/******************** WRITE YOUR INTERRUPT SERVICE ROUTINE HERE***********************/
void InteruptSR(){
    int i;
    int j;


    input[0]=mono_read_16Bit();//read a new sample and place it into buffer

    double temp=0;// initialise the temporary variable as a accumulator
    for(j=0;j<filter_size;j++){ // loop through all the elements needed to compute convolution
      temp+=input[j]*b[j]; // compute element-wise multiplication for convolution
    }
    mono_write_16Bit(temp);//write the convolution result to codec

    for (i=filter_size-1;i>0;i--){
      input[i]=input[i-1]; // shift the buffer by one element in order to make a new space for next incoming input
    }
}

void test_cir(){
    input[offset] = mono_read_16Bit();
    int i;
    output = 0;
    for(i = 0;i<filter_size-offset;i++){
        output+=input[offset+i]*b[i];
    }
    for(i = filter_size-offset;i<filter_size;i++){
        output+=input[offset+i-filter_size]*b[i];
    }
    offset++;
    if (offset==filter_size){offset=0;}
    mono_write_16Bit(output);
}

void opt_cir(){
    input[offset] = mono_read_16Bit();
    int i;
    output = 0;
    // First case offset less than half buffer size
    if(offset<((filter_size-1)/2)){
        for(i=0;i<offset;i++){
                output+=(input[offset+i]+input[offset-i-1])*b[i];
            }
        for(i = offset;i<(filter_size-1)/2;i++){
            output+=(input[offset-i-1+filter_size]+input[offset+i])*b[i];
            }
        output+=b[(filter_size-1)/2]*input[(filter_size-1)/2+offset];
    }
    // Second case larger than buffer size
    else if(offset>(filter_size-1)/2){
        for(i=0;i<filter_size - offset;i++){
            output+=(input[offset+i]+input[offset-i-1])*b[i];
        }
        for(i = filter_size-offset;i<(filter_size-1)/2;i++){
            output+=(input[offset-filter_size+i]+input[offset-1-i])*b[i];
        }
        output+=b[(filter_size-1)/2]*input[offset-(filter_size-1)/2 -1];
    }
    offset++;
    if (offset==filter_size){offset=0;}
    mono_write_16Bit(output);
}

//void ptr_cir(){
//    output = 0 ; // Output conv
//    double * buf_val = cir_ptr + offset; // Pointing to newest slot
//    *buf_val = mono_read_16Bit(); // Assign newest reading to slot
//    for(;buf_val<cir_ptr+((filter_size-1)*sizeof(double));){
//        output+= (*buf_val) * (*coef_ptr);
//        buf_val +=sizeof(double);
//        coef_ptr +=sizeof(double);
//    }
//    buf_val = cir_ptr;
//
//    for(;coef_ptr<coef_ptr+((filter_size-1)*sizeof(double));){
//        output+= (*buf_val) * (*coef_ptr);
//        buf_val +=sizeof(double);
//        coef_ptr +=sizeof(double);
//    }
//    output+= (*buf_val) * (*coef_ptr); // compensate the first element
//
//    mono_write_16Bit(output);
//
//    if (++offset > filter_size){
//        offset = 0;
//    }
//}
//


//void ptr_cir(){
//    double* coeffptr = coef_ptr;
//        double* coeffEndptr = coef_ptr + N; // points to the element AFTER the coefficient array
//        double* sampleptr = cir_ptr + index; // point to oldest sample initially
//        double* bufferEndptr = cir_ptr + N; // one after last element
//
//        int loopcnt = bufferEndptr - sampleptr; // how many iterations are needed for a single-step loop
//        char modunroll = loopcnt % 4; // non-integral leftover of an unrolled loop
//
//        // accumulators
//        double result = 0;
//        double result2 = 0;
//        double result3 = 0;
//        double result4 = 0;
//
//        *sampleptr = mono_read_16Bit(); // read sample into buffer
//
//        // process samples until the end of the sample buffer is hit
//        while(sampleptr < bufferEndptr-3)
//        {
//            result += (*coeffptr++) * (*sampleptr++);
//            result2 += (*coeffptr++) * (*sampleptr++);
//            result3 += (*coeffptr++) * (*sampleptr++);
//            result4 += (*coeffptr++) * (*sampleptr++);
//        }
//
//        // take care of non-integral leftover iterations
//        if (modunroll>0) result += (*coeffptr++) * (*sampleptr++);
//        if (modunroll>1) result2 += (*coeffptr++) * (*sampleptr++);
//        if (modunroll>2) result3 += (*coeffptr++) * (*sampleptr++);
//
//        sampleptr = cir_ptr; // wrap pointer to beginning of the buffer
//
//        // pass the remainder of the buffer (amount of iterations = how many coefficients there are left to process)
//        while (coeffptr < coeffEndptr-3)
//        {
//            result += (*coeffptr++) * (*sampleptr++);
//            result2 += (*coeffptr++) * (*sampleptr++);
//            result3 += (*coeffptr++) * (*sampleptr++);
//            result4 += (*coeffptr++) * (*sampleptr++);
//        }
//
//        // take care of non-integral leftover iterations
//        if (modunroll==1) result += (*coeffptr++) * (*sampleptr++);
//        if (modunroll==1 || modunroll==2) result2 += (*coeffptr++) * (*sampleptr++);
//        if (modunroll==1 || modunroll==2 || modunroll==3) result3 += (*coeffptr++) * (*sampleptr++);
//
//        // sum the accumulators
//        result = result + result2 + result3 + result4;
//
//        // advance index into circular buffer
//        index = (index == 0) ? N-1 : index-1;
//
//        mono_write_16Bit(result);
//}

void ptr_cir(){
    double *buf_val  = cir_ptr;
    *buf_val++ = mono_read_16Bit();
    if (buf_val>=cir_ptr+filter_size){
        buf_val=cir_ptr;
    }
    int i =0;
    double temp = 0;
    double* iter_val=buf_val-1;
    for (i=0;i<filter_size;i++){
        temp += (*iter_val --)* b[i];
        if(iter_val<cir_ptr) iter_val+= filter_size;
    }
    mono_write_16Bit(temp);
//    if (++offset > filter_size){
//           offset = 0;
//       }

}



