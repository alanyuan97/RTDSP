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
float input[filter_size]={0};
float input2[filter_size*2]={0};
float* cir_ptr = input;
float* cir_ptr2 = input2;
float* coef_ptr = &b[0]; // Head pointer to coef array

int index=0;
int index2= filter_size;
int offset=0;
float output=0;
// Codec handle:- a variable used to identify audio interface
DSK6713_AIC23_CodecHandle H_Codec;

 /******************************* Function prototypes ********************************/
void init_hardware(void);
void init_HWI(void);
void opt_cir();
void InterupptSR();
void test_cir();
void ptr_cir();
void processing(double* input);
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

void base_cir(){
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








//symmetry
//void ptr_cir(){
//    input[offset] = mono_read_16Bit();
//    int i;
//    output = 0;
//    // First case offset less than half buffer size
//    if(offset<((filter_size-1)/2)){
//        for(i=0;i<offset;i++){
//                output+=(input[offset+i]+input[offset-i-1])*b[i];
//            }
//        for(i = offset;i<(filter_size-1)/2;i++){
//            output+=(input[offset-i-1+filter_size]+input[offset+i])*b[i];
//            }
//        output+=b[(filter_size-1)/2]*input[(filter_size-1)/2+offset];
//    }
//    // Second case larger than buffer size
//    else if(offset>(filter_size-1)/2){
//        for(i=0;i<filter_size - offset;i++){
//            output+=(input[offset+i]+input[offset-i-1])*b[i];
//        }
//        for(i = filter_size-offset;i<(filter_size-1)/2;i++){
//            output+=(input[offset-filter_size+i]+input[offset-1-i])*b[i];
//        }
//        output+=b[(filter_size-1)/2]*input[offset-(filter_size-1)/2 -1];
//    }
//    offset++;
//    if (offset==filter_size){offset=0;}
//    mono_write_16Bit(output);
//}

//pointer for float type (based on base circular buffer)
//void ptr_cir(){
//    int i;
//    float* iter_val = coef_ptr;
//    float* buf_val;
//    output = 0;
//    ++offset;
//    offset = offset % filter_size;//prevent offset from out-of-range
//    buf_val  = cir_ptr + offset;
//
//    *buf_val = mono_read_16Bit();
//   while(iter_val - coef_ptr <filter_size - offset -1){
//       //-1 because the pointer need to be within range
//       output += (*buf_val++)* (*iter_val++);
//   }
//   output += (*buf_val)*(*iter_val); // compensate for the -1 above
//
//   buf_val-= filter_size;//wrap around to continue for elements before offset
//
//   while(iter_val - coef_ptr <filter_size-2){
//
//       output += (*buf_val++)* (*iter_val++);
//   }
//   output += (*buf_val)*(*iter_val);
//   mono_write_16Bit(output);
//}


//double memory + symmetry
void ptr_cir (){
    float temp=mono_read_16Bit();
    input2[index]=temp;
    input2[index2]=temp; // index2 is always index+filter_size
    output = 0;
    int i;
    for (i=0;i<(filter_size-1)/2;i++){//only loop for half of the size of filter coefficients
        output+=(input2[index+i]+
                input2[index2-i-1])*b[i]; //using symmetry property
    }
    output+= input2[index+(filter_size-1)/2]*b[(filter_size-1)/2];//compensate for the middle coefficient
    index++;
    if (index==filter_size) index=0;
    index2=index+filter_size;
    mono_write_16Bit(output);
}


// double + symmetry + pointer
//void ptr_cir(){
//    float* iter_val = coef_ptr;
//    float* buf_val1;
//    float* buf_val2;
//    float* des_ptr = coef_ptr+((filter_size -1)/2)-1;
//
//    output = 0;
//
//    offset = ++offset % filter_size;
//    buf_val1  = cir_ptr2 + offset;
//    buf_val2 = cir_ptr2 + filter_size+offset-1;
//    float temp = mono_read_16Bit();
//    *buf_val1 = temp;
//    *buf_val2 = temp;
//
//   while(iter_val <des_ptr){
//
//       output += ((*buf_val1++)+(*buf_val2--))* (*iter_val++);
//   }
//   output += ((*buf_val1))* (*iter_val);
//   mono_write_16Bit(output);
//}
