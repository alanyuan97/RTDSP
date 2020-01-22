# RTDSP
Real Time Digital Signal Processing

## Section 1
### Answers to questions
1.  Why do some NOP statements occur?
    1. To allow pipelining
    2. to avoid various data OR control hazards. i.e. structural/ data hazards
    3. To make code length a multiple of 8

2.  What is the purpose of ++ and -- operators in the following code?
    To increment the pointers to input and output after each multiplication with constant "gain"

3. What are the two purposes of * operator?
    1. dereference pointer to access the correct values
    2. multiplication

4. What is the difference between how the ++ operator works with an integer and a pointer?
    1. For an integer, this means increment of variable value
    2. For a pointer, this means increament of addrerss pointed

5. What is the size of I/O buffers in hex and denary?
    0x64 in hex and 100 in denary

6. What gain value will the input signal be amplified when the progeram is initially run?
    gain>1

7. If they cycle time is 4.4ns, how much time is taken for each call to load() if the argument is set to 30?
    Time = 1000*30*4.4
8. Always build the code before executing.
9. #define GRAVITY 10, DO NOT ";" AND "="
10. TODO ?

## Section 2 
### Notes
1.  #include DSO/BIOS config file & AIC config file => works for ADC & DAC convertors
2.  #include file to help configure hardware

### Answers to questions
1. 8 samples per period, tested using variable watching
2. The sampling frequency is set to 8khz and the number of samples per period is 8. 8k/8 = 1 k. The hardware(periphal) constaints the clock rate at 1 kHz.
3. 32 bits
