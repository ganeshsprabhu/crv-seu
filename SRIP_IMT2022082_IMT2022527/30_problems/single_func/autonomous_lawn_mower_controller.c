// autonomous_lawn_mower_controller.c
// Controls the drive motors and blades of an autonomous lawn mower. It normally follows a GPS-based path to cover a lawn (the path data is a CRV). A critical safety override from a tilt sensor will shut down the drive motors and blades if the mower is about to tip over, ignoring its current GPS path.
// blade_motor_state == 0 || blade_motor_state == 1
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#define BLADE_OFF 0
#define BLADE_ON 1
#define MAX_TILT_ANGLE 30 // degrees

// Simulated hardware inputs
volatile int tilt_angle;
volatile bool follow_gps_path; // (CRV candidate)

void read_mower_sensors() {
    tilt_angle = 20 + (rand() % 15); // Angle near the threshold
    follow_gps_path = true; // Always try to follow path
}

void log_blade_state(const char* reason, int state) {
    printf("Reason: %-25s | Blade Motor: %s\n", reason, state == BLADE_ON ? "ON" : "OFF");
}

int step_control_logic() {
    int new_blade_state;

    // 1. Critical Safety Override: Tilt Sensor
    // This path makes 'follow_gps_path' irrelevant.
    if (tilt_angle > MAX_TILT_ANGLE) {
        new_blade_state = BLADE_OFF;
        log_blade_state("TILT HAZARD - SHUTDOWN", new_blade_state);
    } 
    // 2. Standard Operational Logic
    else {
        // Logic depends on the CRV 'follow_gps_path'
        if (follow_gps_path) {
            new_blade_state = BLADE_ON;
            log_blade_state("Mowing GPS Path", new_blade_state);
        } else {
            new_blade_state = BLADE_OFF;
            log_blade_state("Path Complete", new_blade_state);
        }
    }
    return new_blade_state;
}

int main() {
    srand(time(NULL));
    int blade_motor_state = BLADE_OFF;
    printf("--- Autonomous Mower Control Simulation (Single Function) ---\n");

    for (int i = 0; i < 5; i++) {
        read_mower_sensors();
        blade_motor_state = step_control_logic();
        
        if (blade_motor_state < 0 || blade_motor_state > 1) {
            printf("!!! SAFETY VIOLATION: Invalid blade state !!!\n");
            return -1;
        }
    }

    return 0;
}

