# RTDSP
Real Time Digital Signal Processing

##Section 1
###Answers to questions
1.  Why do some NOP statements occur? 
    1, To allow pipelining 
    2, to avoid various data OR control hazards. i.e. structural/ data hazards
    3, To make code length a multiple of 8 

2.  what is the purpose of ++ and -- operators in the following code? 
    To increment the pointers to input and output after each multiplication with constant "gain"

3. What are the two purposes of * operator? 
    1, dereference pointer to access the correct values 
    2, multiplication 

4. what is the difference between how the ++ operator works with an integer and a pointer?
    1, For an integer, this means increment of variable value
    2, For a pointer, this means increament of addrerss pointed 

5. what is the size of I/O buffers in hex and denary?
    0x64 in hex and 100 in denary 

6. Ny what gain value will the input signal be amplified when the progeram is initially run?
    gain>1 
    
7. If they cycle time is 4.4ns, how much time is taken for each call to load() if the argument is set to 30?
    Time = 1000*30*4.4