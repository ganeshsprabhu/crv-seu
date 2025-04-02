#include <stdio.h>
#include <stdbool.h>
#include "simulate_seu.h"

int p(int x, int y) {
	int output = 4;
	bool alarm = false;
	int count = 0;
	while (count < 7) {
		if (x > 10) {
			if (y == 1) {
				output = 2;
			} else {
			    output = 1;
		    }
		} else {
			output = output + 1;
		    alarm = true;
		}
		count++;
	}
	printf("alarm = %d\n", alarm);
	return output;
}

int p_prime_x(int x , int y )
{
  int output ;
  int count ;
  {
  output = 4;
  count = 0;
  while (count < 7) {
    {
    simulate_seu_main(& x);
    if (x > 10) {
      if (y == 1) {
        output = 2;
      } else {
        output = 1;
      }
    } else {
      output ++;
    }
    }
    count ++;
  }
  return (output);
}
}


int main() {

	int output, x, y;
    
	output = p(x, y); // OriginalProgram
	int x_output = p_prime_x(x, y); // p'(x): Instrumented program, x is the variable under investigation

	//Safety Conditions assignment :  tracks whether the safety property (output <= 10) holds after the SEU is introduced for x/y
	int phi = output <= 10;
	int phi_prime_x = x_output <= 10;


	// Check CRV for x: We need to find and Ix such that (phi XOR phi_prime_x) is true
	__CPROVER_assert(!(phi ^ phi_prime_x), "CRV Result for x => if,SUCCESS then its not a CRV and if FAILURE then its a CRV!");

	return 0; 
}



