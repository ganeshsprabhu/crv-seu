# Automating the self-developed concept of catching CRV variables
This repository contains a shell file that works by taking the C source code (the one for which CRV are meant to be found) and a condition.txt file (with the safety condition present in it).

## Pre-Requisites
- Finish Setting up (refer to the 'setup' folder)
- Ensure that the 'instrument_seu', 'simulate_seu.h', 'setup.sh' files are all present in this directory itself.

## Running The Concept
The 'automate_catch_crv.sh' is a shell file that takes a few inputs, including the source file under consideration, the function under consideration, the output variable for which the safety condition is defined, and the variable whose conditional relevance you want to check.

The following is done by the shell file in order:

1. Gets the source C file
2. Performs static analysis on a function (the entry point) with respect to a variable that are given as input by the user and writes the sliced code into a '{user_input_file}_sliced.c' file
3. Uses the above sliced code and the 'instrument_seu' to insert simulate_seu statements into the sliced code with respect to a variable (given by the user) and writes this instrumented code into '{user_input_file}_instru.c' file.
4. Cleans the '{user_input_file}_instru.c' file and writes it to '{user_input_file}_instru_clean.c'.
5. Creates a new file '{user_input_file}_instru_clean_renamed.c' with the function named modified by adding '_prime_{variable_name}' to the function indicating that it simulates seu events for that particular variable in the function.
6. Now, creates the cbmc ready file '{user_input_file}_cbmc_ready.c' which contains the modified function.

Now, in addition to adding ```#include"simulate_seu.h"``` at the top of the file, the user needs to make a change in this file by adding a call to the modified function just below the original function call. After this, the user needs to add statements to check the output of the safety condition specified.
For instance, consider the 'cs1_org.c' file. The function under consideration is the 'p' function and the variable with respect to the safety condition is 'output' and the variable in consideration for checking it's conditional relevance is 'x'.
```
int main() {

	int output, x, y;
	output = p(x, y); // OriginalProgram
	return 0; 
}
```

After adding the new function call and checking the outputs of safety condition, it would look like:
```
int main() {

	int output, x, y;
    
	output = p(x, y); // OriginalProgram
	int x_output = p_prime_x(x, y); // p'(x): Instrumented program, x is the variable under investigation

	//Safety Conditions assignment :  tracks whether the safety property (output <= 10) holds after the SEU is introduced for x/y
	int phi = output <= 10;
	int phi_prime_x = x_output <= 10;


	// Check CRV for x: We need to find and Ix such that (phi XOR phi_prime_x) is true
	__CPROVER_assert(!(phi ^ phi_prime_x), "The variable you've instrumented is a CRV");

	return 0; 
}
```
