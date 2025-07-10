#include <stdio.h>
#include <stdlib.h>
#include"queue.h"
#include"simulate_seu.h"
#define MAX_SPEED 1500 
#define MIN_SPEED 0 
double step(double aircraft_speed,int old_turbine_speed, int turbine_speed,double air_pressure, double air_temperature, double air_density) {

    double aircraft_speed_change= 0.0;//
    
    aircraft_speed_change = (turbine_speed * air_pressure) / (air_temperature * air_density);

    aircraft_speed += aircraft_speed_change;

    return aircraft_speed;
}
int main(){
    Queue q1;
    initQueue(&q1);
    Queue q2;
    initQueue(&q2);
    int turbine_speed=0;
    int old_turbine_speed=0;
    double aircraft_speed=0;
    // other environmental variables
    double air_pressure = 101325; 
    double air_temperature = 288.15; 
    double air_density = 1.225; 
    int iteration_cnt=0;
    while(1){
        iteration_cnt++;
        // Simulate reading the current turbine speed
        turbine_speed = rand() % (MAX_TURBINE_SPEED+1);
        // aircraft speed needs to be greater than the value 4 iterations ago
        air_pressure = 101325 + (rand() % 1000 - 500); // Simulate pressure variation
        air_temperature = 288.15 + (rand() % 100 - 50);
        air_density = 1.225 + (rand() % 100 - 50) / 100.0;
        aircraft_speed = step(aircraft_speed,old_turbine_speed,turbine_speed, air_pressure, air_temperature, air_density);
        old_turbine_speed = turbine_speed;

        enqueue(&q1, aircraft_speed);
        enqueue(&q2, aircraft_speed_prime);
        int phi = 1;
        int phi_prime = 1;
        if(iteration_cnt<5)
            continue;
            
        int normal_sum=0;
        int prime_sum=0;
        for(int j=0;j<q1->count;j++)
        {
            int idx1 = (q1->rear - j + SIZE) % SIZE;
            int idx2 = (q2->rear - j + SIZE) % SIZE;

            int val1 = q1->data[idx1];
            int val2 = q2->data[idx2];

            normal_sum+=val1;
            prime_sum+=val2;
        }
        int normal_avg = normal_sum / q1->count;
        int prime_avg = prime_sum / q2->count;
        int phi=normal_avg >= MIN_SPEED && normal_avg <= MAX_SPEED;
        int phi_prime= prime_avg >= MIN_SPEED && prime_avg <= MAX_SPEED;

        if(phi^phi_prime)
        {
            __CPROVER_assert(0, "SUCCESS:NON CRV, FAILURE: CRV");
            return -1; 
        }
    }
    return 0;
}