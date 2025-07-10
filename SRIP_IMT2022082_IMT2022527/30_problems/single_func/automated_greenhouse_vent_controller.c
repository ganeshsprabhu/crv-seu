// automated_greenhouse_vent_controller.c
// Manages greenhouse roof vents to regulate temperature. The primary control uses a target temperature setting (a CRV). However, a high-wind-speed anemometer provides a safety override, forcing the vents to close completely to prevent structural damage, regardless of the internal temperature or the target setting.
// vent_opening >= 0 && vent_opening <= 100
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#define MAX_VENT_OPENING 100
#define MIN_VENT_OPENING 0
#define HIGH_WIND_THRESHOLD 50 // km/h

// Simulated hardware inputs
volatile int wind_speed;
volatile int current_temp;
volatile int target_temp; // (CRV candidate)

int step_control_logic(int vent_opening) {
    int new_vent_opening=vent_opening;

    // 1. Critical Safety Override: High Wind
    // This path makes 'target_temp' irrelevant.
    if (wind_speed > HIGH_WIND_THRESHOLD) {
        new_vent_opening = MIN_VENT_OPENING;
    } 
    // 2. Standard Operational Logic
    else {
        // Logic depends on the CRV 'target_temp'
        int temp_diff = current_temp - target_temp;
        if (temp_diff > 0) {
            new_vent_opening = temp_diff * 10; // Open vents to cool down
        } else {
            new_vent_opening = vent_opening;
        }
    }

    // 3. Final Safety Saturation
    if (new_vent_opening > MAX_VENT_OPENING) new_vent_opening = MAX_VENT_OPENING;
    if (new_vent_opening < MIN_VENT_OPENING) new_vent_opening = MIN_VENT_OPENING;

    return new_vent_opening;
}

int main() {
    srand(time(NULL));
    int vent_opening = 0;
    printf("--- Greenhouse Vent Control Simulation (Single Function) ---\n");

    for (int i = 0; i < 5; ++i) {
        wind_speed = 30 + (rand() % 40); // Wind between 30 and 70 km/h
        current_temp = 20 + (rand() % 15); // Temp between 20 and 35 C
        target_temp = 25; // User-set target
        vent_opening = step_control_logic(vent_opening);
    
    }

    return 0;
}

