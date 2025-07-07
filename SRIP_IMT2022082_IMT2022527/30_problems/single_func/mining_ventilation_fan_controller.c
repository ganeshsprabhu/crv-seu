// mining_ventilation_fan_controller.c
// Controls the speed of ventilation fans in a mine. The fan speed is normally adjusted based on a daily work schedule (a CRV) to save energy during off-hours. A methane gas sensor provides a critical safety override, forcing the fans to maximum speed to clear the hazardous gas, regardless of the time of day or work schedule.
// fan_speed >= 0 && fan_speed <= 100
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#define FAN_SPEED_MAX 100
#define FAN_SPEED_NORMAL 60
#define FAN_SPEED_IDLE 20
#define METHANE_THRESHOLD 500 // ppm

// Simulated hardware inputs
volatile int methane_level_ppm;
volatile bool is_work_shift_active; // (CRV candidate)

void read_mine_sensors() {
    methane_level_ppm = 400 + (rand() % 200); // Methane level around threshold
    is_work_shift_active = (rand() % 2 == 0);
}

void log_fan_speed(const char* reason, int speed) {
    printf("Reason: %-25s | Fan Speed: %d%%\n", reason, speed);
}

int step_control_logic() {
    int new_fan_speed;

    // 1. Critical Safety Override: High Methane
    // This path makes 'is_work_shift_active' irrelevant.
    if (methane_level_ppm > METHANE_THRESHOLD) {
        new_fan_speed = FAN_SPEED_MAX;
        log_fan_speed("HIGH METHANE - PURGE", new_fan_speed);
    } 
    // 2. Standard Operational Logic
    else {
        // Logic depends on the CRV 'is_work_shift_active'
        if (is_work_shift_active) {
            new_fan_speed = FAN_SPEED_NORMAL;
            log_fan_speed("Work Shift Active", new_fan_speed);
        } else {
            new_fan_speed = FAN_SPEED_IDLE;
            log_fan_speed("Off-Shift Idle", new_fan_speed);
        }
    }
    return new_fan_speed;
}

int main() {
    srand(time(NULL));
    int fan_speed = FAN_SPEED_IDLE;
    printf("--- Mining Ventilation Control Simulation (Single Function) ---\n");

    for (int i = 0; i < 10; i++) {
        read_mine_sensors();
        fan_speed = step_control_logic();
        
        if (fan_speed < 0 || fan_speed > 100) {
            printf("!!! SAFETY VIOLATION: Invalid fan speed !!!\n");
            return -1;
        }
    }

    return 0;
}

