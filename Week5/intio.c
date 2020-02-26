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
#define N 2
#define Na 5
#define Nb 5
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

double sample;
double b_coefficient[2]= {17,17};
double input[2]={0,0};
double a_coefficient = -15;
double last_output=0;
double temp;
//double v[5]={0,0,0,0,0};
double* x_ptr;
double* this_ptr;
double* next_ptr;
double* x_end;
double* v;
double A[]={1.000000e+00, -3.622738e+00, 5.063888e+00, -3.234610e+00, 7.984165e-01};
double B[]={9.756494e-02, -3.428674e-01, 4.911005e-01, -3.428674e-01, 9.756494e-02};
int index=0;
int index2= filter_size;
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
void processing(double* input);
void base_irr();
void ISR_iir();
double irr_1();
double irr_2();
double irr_3();
/********************************** Main routine ************************************/
void main(){
   v=(double*)calloc(Na,sizeof(double));


	// initialize board and the audio port
  init_hardware();

  /* initialize hardware interrupts */
  init_HWI();
//  sine_init();
//  x_ptr=(double*)calloc(Na,sizeof(double));
//  this_ptr=x_ptr;
//  next_ptr=x_ptr+1;
//  x_end=x+Na-1;


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
void ISR_iir(){
    sample =mono_read_16Bit();
    temp = irr_3();
    //base_irr();
    mono_write_16Bit(temp);
}

void base_irr(){
    input[0] =sample;
    temp= input[0]/b_coefficient[0];
    temp+= input[1]/b_coefficient[1];
    temp-=a_coefficient*last_output/17;
    last_output = temp;
    input[1]=input[0];
    mono_write_16Bit(temp);
}


double irr_1(){
      v[0]=sample;
      output=0;
      int i;
      for (i=Na-1;i>0;i--){
          v[0]-= A[i]*v[i];
          output+=B[i]*v[i];
          v[i]=v[i-1];
      }
      output+=B[0]*v[0];
      return output;
}

//double irr_1(){
//      v[0]=sample;
//      output=0;
//      int i;
//      for (i=Na-1;i>0;i--){
//          v[0]-= A[i]*v[i];
//          output+=B[i]*v[i];
//          v[i]=v[i-1];
//      }
//      output+=B[0]*v[0];
//      return output;
//}

//dynamic circular buffer implementation
//double irr_2(){
//    double* v_current = v_ptr+index;
//    double* v_next = v_current+1;
//    double* v_end = v+Na;
//    double* A_ptr = &A[0] + 1; //skip the first element
//    double* A_end = &A[0] + Na;
//    double* B_ptr = &B[0];
//    double* B_end = &B[0]+Na;
//    double output = 0;
//
//   // begin
//    *v_current = sample;
//
//    for (;v_next<v_end;++v_next,++A_ptr){
//        *v_current -= (*v_next)*(*A_ptr);
//    }
//    v_next = v_ptr;//warp around the index
//    for (;A_ptr<A_end;++v_next,++A_ptr){
//         *v_current -= (*v_next)*(*A_ptr); // finish the remaining calculations
//        }
//
//    // now all intermediate values are initialised
//
//    for (;v_current<v_end;++v_current, ++B_ptr){
//        output += (*v_current)*(*B_ptr);
//    }
//
//    v_current = v_ptr;
//    for (;B_ptr<B_end;++v_current, ++B_ptr){
//        output += (*v_current)*(*B_ptr);
//    }
//    if (index == 0){
//        index = Na-1;
//    }
//    else {
//        index-=1;
//    }
//    return output;
//}
//
double irr_3(){
    double output = 0;
    output = v[0] + b[0]*sample;
    int i=0;
    for (i = 0;i<Na-1;i++){
        v[i] = v[i+1]+B[i+1]*sample - A[i+1]*output;
    }
    v[Na-1] = B[Na]*sample - A[Na]* output;
    return output;
}

