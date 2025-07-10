// conveyor_belt_speed_controller.c
// Adjusts the speed of a conveyor belt in a sorting facility. The speed is normally determined by a load cell measuring the weight of items on the belt (the CRV). A jam detection sensor provides a critical safety override, halting the motor immediately to prevent damage and clear the jam, regardless of the item weight.
// motor_speed >= 0 && motor_speed <= 100
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#define MAX_SPEED 100
#define MIN_SPEED 0
#define STOP_SPEED 0

// Simulated hardware inputs
volatile bool jam_detected;
volatile int item_weight; // (CRV candidate) in kg



int step_control_logic() {
    int new_motor_speed;

    // 1. Critical Safety Override: Jam Detection
    // This path makes 'item_weight' irrelevant.
    if (jam_detected) {
        new_motor_speed = STOP_SPEED;
    } 
    // 2. Standard Operational Logic
    else {
        // Logic depends on the CRV 'item_weight'
        if (item_weight > 30) {
            new_motor_speed = 50; // Slower for heavy items
        } else {
            new_motor_speed = 90; // Faster for light items
        }
    }

    // 3. Final Safety Saturation
    if (new_motor_speed > MAX_SPEED) new_motor_speed = MAX_SPEED;
    if (new_motor_speed < MIN_SPEED) new_motor_speed = MIN_SPEED;

    return new_motor_speed;
}

int main() {
    srand(time(NULL));
    int motor_speed = 0;
    printf("--- Conveyor Belt Control Simulation (Single Function) ---\n");

    for (int i = 0; i < 10; ++i) {
        read_conveyor_sensors();
        motor_speed = step_control_logic();
        
        if (motor_speed < MIN_SPEED || motor_speed > MAX_SPEED) {
            printf("!!! SAFETY VIOLATION: Motor speed out of bounds !!!\n");
            return -1;
        }
    }

    return 0;
}

