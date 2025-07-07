#include"simulate_seu.h"
#include"queue.h"
#include <stdio.h>
#include <stdbool.h>

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
int main() {

	Queue q1;
	initQueue(&q1);
	Queue q2;
	initQueue(&q2);
	int output, x, y;
	int output2, x2, y2;
	x2=x;
	y2=y;

	int condition_broken = 0;
	int i=0;
	while(i<10)
	{
		output = p(x, y); // OriginalProgram
		output2 = p_prime_y(x2,y2);
		i++;

		int safety_output1 = output<=10;
		int safety_output2 = output2<=10;
		enqueue(&q1,!safety_output1);
		enqueue(&q2,!safety_output2);
		int phi=1;
		int phi_prime=1;
		
		if(i<5)continue;
		for(int j=0;j<5;j++)
		{
			phi=phi & q1.data[j];
			phi_prime=phi_prime & q2.data[j];
		}
		phi=!phi;
		phi_prime=!phi_prime;
		if((phi ^ phi_prime)==1){
			condition_broken=1;
			break;
		}

		
	}

	__CPROVER_assert(!condition_broken, "The variable you've instrumented is a CRV");
	return 0; 
}



// ----- Renamed Instrumented Function -----

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
