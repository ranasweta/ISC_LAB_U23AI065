# **Lab 8: DES Round Function Implementation**

This project contains the C++ implementation of the DES round function f as required by the lab assignment.

## **Files**

* des.hpp: The header file defining the interface for the DES functions.  
* des.cpp: The main implementation file containing the logic for the feistel round function and its required data tables (E, P, S-boxes).  
* main.cpp: A test program to verify the correctness of the feistel function using the specific test case provided in the lab document.

## **How to Compile and Run**

You can compile the program using a standard C++ compiler like g++. Open your terminal and run the following command:

g++ main.cpp des.cpp \-o des\_test

After compiling, run the executable:

./des\_test

## **Test Output**

The program will execute the test case from the lab instructions and print the result. The expected output is:

Testing DES f function...  
Input R:         0xF0AAF0AA  
Input Subkey:    0x1B02EFFC7072  
Actual output:   0x234AA9BB  
Expected output: 0x234AA9BB

Test PASSED\!  
