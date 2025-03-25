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

	int output, x, y;
	output = p(x, y); // OriginalProgram
	return 0; 
}




