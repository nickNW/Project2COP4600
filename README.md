The project requirements are described in p2-memsim.pdf. Use memory traces that are stored in two files: 
bzip.trace.zip and sixpack.trace.zip. You are to implement a page replacement algorithm presented in this paper.

Files necessary to compile the program: 
    memsim.c : main interface of the simulator, it takes user input in two forms and Runs algorithms 

To compile the program:    
    Compile using g++ -o  memsim  memsim.cpp


To run the program: 
The program can be run with two types of input format 
1. For fifo and lru use the following format: 
./memsim tracefile nFrames policy quiet/debug 
 
2. For segmented fifo use the following format: 
./memsim tracefile nframes policy percentage quiet/debug 






