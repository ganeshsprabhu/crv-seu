#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<time.h>
#include<unistd.h>

//The program - A simulation of a car trying to cover a total distance of 100km. The driver is simulated through random inputs.

#define TOTAL_DISTANCE_TO_COVER 100.0f		//km
#define TIME_PER_ITERATION 10.0f/3600000.0f	//s

//Car gear speeds (kmph).
#define MIN_GEAR 1.0f				//inclusive.
#define MAX_GEAR 5.0f				//inclusive.
#define GEAR1_SPEED 10.0f			//when speed of car is <=10, the gear has to be 1 or changed to 1.
#define GEAR2_SPEED 25.0f			//when speed of car is >=10 and <=25, then gear can be in 2 or changed to 2.
#define GEAR3_SPEED 45.0f			//when speed of car is >=25 and <=45, then gear can be in 3 or changed to 3.
#define GEAR4_SPEED 60.0f			//when speed of car is >=45 and <=60, then gear can be in 4 or changed to 4.
#define GEAR5_SPEED 80.0f			//when speed of car is >=60 and <=80, then gear can be in 5 or changed to 5.
						//when speed of car is >80, the gear has to be 5.
#define MAX_CAR_SPEED 120			//kmph

typedef struct Car {
	int speed;
	int gear;
	int cruise_on;				//is '-1' if the cruise has not been switched on else it will maintain the speed that was read last time.
	bool alarm;
} Car;

typedef struct Driver {
	float accelarator;			//0.0 to 1.0
	float brake;				//0.0 to 1.0
	float clutch;				//0.0 to 1.0
} Driver;



//Functions.
int read_speed_sensor(Car*, Driver*);		//Reads the speed of the car from the instance variable created.

int main()
{
	srand((unsigned)time(NULL));

	Car* simulationCar = (Car*)malloc(sizeof(Car));
	if(simulationCar==NULL)
	{
		printf("Memory allocation failed for Car!\n");
		return 1;
	}
	simulationCar->gear=1;
	simulationCar->speed=1;
	simulationCar->alarm=false;

	Driver* simulationDriver= (Driver*)malloc(sizeof(Driver));
	if(simulationDriver==NULL)
	{
		printf("Memory allocation failed for Driver!\n");
		return 1;
	}
	simulationDriver->accelarator=0.0f;
	simulationDriver->brake=0.0f;
	simulationDriver->clutch=0.0f;

	float distance_covered=0;
	while(distance_covered<TOTAL_DISTANCE_TO_COVER)
	{
		//Reading the speed from the sensor.
		int speed=read_speed_sensor(simulationCar, simulationDriver);

		printf("speed read: %d gear: %d\n",speed,simulationCar->gear);

		//Getting the distance covered based on the speed read from the sensor.
		distance_covered += ((TIME_PER_ITERATION) * speed);

		//sleeping so that it's easier to read the speed of the car.
		sleep(1);
	}

	return 0;
}


///////HELPER FUNCTIONS
int read_speed_sensor(Car* simulationCar, Driver* simulationDriver)
{
	//to retain information of the overall accelarate, brake and clutch values.
	static float curr_accel_val = 0.0f;
	static float curr_brake_val = 0.0f;
	static float curr_clutch_val = 0.0f;

	//generating random values for the accel, brake and clutch to add to the values retained from the previous iterations.
	float sample_accel= (float) rand() / (float)RAND_MAX;
	float sample_brake;		//15% chance of braking.
	float sample_clutch;		//20% chance of clutch.

	float val_gen1 = (float) rand() / (float)RAND_MAX;
	sample_brake=val_gen1 < 0.05 ?  (0.5f + ((float)rand() / RAND_MAX) * 0.5f) : 0.0f;

	float val_gen2 = (float) rand() / (float)RAND_MAX;
	sample_clutch=val_gen2 < 0.02 ?  (0.5f + ((float)rand() / RAND_MAX) * 0.5f) : 0.0f;

	//if speed goes very high, alert the user. Simulates a decrease in accelaration.
	if(simulationCar->speed >= MAX_CAR_SPEED)
	{
		simulationCar->alarm=true;
		/*
		sample_brake=rare_event(0.8);
		sample_clutch=rare_event(0.1);
		sample_accel=rare_event(0.05);
		*/

		float val_gen = (float) rand() / (float)RAND_MAX;
		sample_brake=val_gen < 0.8 ?  (0.5f + ((float)rand() / RAND_MAX) * 0.5f) : 0.0f;

		val_gen = (float) rand() / (float)RAND_MAX;
		sample_clutch=val_gen < 0.1 ?  (0.5f + ((float)rand() / RAND_MAX) * 0.5f) : 0.0f;

		val_gen = (float) rand() / (float)RAND_MAX;
		sample_accel=val_gen < 0.05 ?  (0.5f + ((float)rand() / RAND_MAX) * 0.5f) : 0.0f;
	}
	else
	{
		simulationCar->alarm=false;
	}
	
	//Smoothening the values of the accel, brake, clutch.
	const float alpha = 0.2f;
	curr_accel_val  = curr_accel_val  * (1 - alpha) + sample_accel  * alpha;
	curr_brake_val  = curr_brake_val  * (1 - alpha) + sample_brake  * alpha;
	curr_clutch_val = curr_clutch_val * (1 - alpha) + sample_clutch * alpha;

	float new_speed = simulationCar->speed;
	if (curr_clutch_val > 0.1f)
	{
		//no change in speed.
	}
	else 
	{
		new_speed += 15.0f * curr_accel_val;
		new_speed -= 30.0f * curr_brake_val;
		if (new_speed < 0.0f) new_speed = 0.0f;
	}
	simulationCar->speed = new_speed;

	int new_gear;
	float sp=simulationCar->speed;
	if(sp<=GEAR1_SPEED)
	{
		new_gear = (int)MIN_GEAR;
	}
	else if(sp<=GEAR2_SPEED)
	{
		new_gear=2;
	}
	else if(sp<=GEAR3_SPEED)
	{
		new_gear=3;
	}
	else if(sp<=GEAR4_SPEED)
	{
		new_gear=4;
	}
	else if(sp<=GEAR5_SPEED)
	{
		new_gear=5;
	}
	else
	{
		new_gear=(int)MAX_GEAR;
	}
	simulationCar->gear = new_gear;

	return simulationCar->speed;
}
