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
    tilt_angle = 20 + (rand() % 15);
    follow_gps_path = true;
}

void log_blade_state(const char* reason, int state) {
    printf("Reason: %-25s | Blade Motor: %s\n", reason, state == BLADE_ON ? "ON" : "OFF");
}

bool is_tilt_hazard_detected() {
    return tilt_angle > MAX_TILT_ANGLE;
}

int calculate_normal_blade_state() {
    if (follow_gps_path) {
        log_blade_state("Mowing GPS Path", BLADE_ON);
        return BLADE_ON;
    } else {
        log_blade_state("Path Complete", BLADE_OFF);
        return BLADE_OFF;
    }
}

int step_control_logic() {
    if (is_tilt_hazard_detected()) {
        log_blade_state("TILT HAZARD - SHUTDOWN", BLADE_OFF);
        return BLADE_OFF;
    } else {
        return calculate_normal_blade_state();
    }
}

int main() {
    srand(time(NULL));
    int blade_motor_state = BLADE_OFF;
    printf("--- Autonomous Mower Control Simulation (Multi Function) ---\n");

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

