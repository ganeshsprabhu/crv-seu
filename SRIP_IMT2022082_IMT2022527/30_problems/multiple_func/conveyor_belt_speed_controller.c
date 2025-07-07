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

void read_conveyor_sensors() {
    jam_detected = (rand() % 15 == 0);
    item_weight = rand() % 50;
}

void log_motor_speed(const char* reason, int speed) {
    printf("Reason: %-20s | Motor Speed: %d%%\n", reason, speed);
}

int calculate_normal_speed() {
    if (item_weight > 30) {
        log_motor_speed("Heavy Item", 50);
        return 50;
    } else {
        log_motor_speed("Light Item", 90);
        return 90;
    }
}

int apply_speed_limits(int speed) {
    if (speed > MAX_SPEED) return MAX_SPEED;
    if (speed < MIN_SPEED) return MIN_SPEED;
    return speed;
}

int step_control_logic() {
    int new_motor_speed;
    if (jam_detected) {
        new_motor_speed = STOP_SPEED;
        log_motor_speed("JAM DETECTED", new_motor_speed);
    } else {
        new_motor_speed = calculate_normal_speed();
    }
    return apply_speed_limits(new_motor_speed);
}

int main() {
    srand(time(NULL));
    int motor_speed = 0;
    printf("--- Conveyor Belt Control Simulation (Multi Function) ---\n");

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

