# README
## GraphColoringUsingLocks
Implementing Graph Coloring Algorithm using locks


### DESCRIPTION
-----------
The OBJECTIVE is to implement the graph colouring algorithm in parallel using threads and synchronise them with the help of locks, in C++. Greedy coloring is used for this method.

### USAGE(HOW TO COMPILE AND RUN)
-----------------------------
The cpp files take input from a file named "input_params.txt" so make sure it is in the same folder as the source files. For this example, we'll show how to compile SrcAssgn5_coarse.cpp. Same steps could be used to create output for SrcAssgn5_fine.cpp.

For compiling the program, open the terminal. Make sure you are in the directory containing "SrcAssgn5_coarse.cpp" & "input_params.txt". You should also have the "gcc g++ compiler" installed(look into build-essential if you are on Linux). Then enter the following command -  
						g++ -std=c++11 -pthread SrcAssgn5_coarse.cpp -o out
An output file by the name of "out" is created in the same directory. To run it, enter the following command - 
						./out
This will run the program and also create an output text file named "output.txt". You can check the total number of colors used, color for each vertex and the time taken in microseconds in the output.txt file.

### THE INPUT FILE
-------------------
The input is taken in "adjacency list" format, with the first vertex marked as 0, then 1, then 2 upto v-1. The number of vertices taken for generating report is 1x1000, and in increments of 1000 respectively. Time is calculated in microseconds. A sample input file is provided for reference.
