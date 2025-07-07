// submarine_ballast_tank_controller.c
// Controls the pumps for a submarine's ballast tanks to manage depth. Normal operation adjusts the ballast based on a target depth command (a CRV). A critical hull pressure sensor, indicating the submarine is approaching its crush depth, triggers an emergency blow of the ballast tanks to surface immediately, ignoring the target depth command.
// pump_action >= -1 && pump_action <= 1
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#define PUMP_ACTION_FILL 1
#define PUMP_ACTION_EMPTY -1
#define PUMP_ACTION_HOLD 0
#define CRUSH_DEPTH_PRESSURE 1000 // in PSI

// Simulated hardware inputs
volatile int hull_pressure;
volatile int current_depth;
volatile int target_depth; // (CRV candidate)

void read_sub_sensors() {
    hull_pressure = 800 + (rand() % 250); // Pressure near critical limit
    current_depth = hull_pressure / 10; // Simplified relationship
    target_depth = 120; // Dive deeper command
}

void log_pump_action(const char* reason, int action) {
    const char* action_str = "HOLD";
    if (action == PUMP_ACTION_FILL) action_str = "FILLING";
    if (action == PUMP_ACTION_EMPTY) action_str = "EMPTYING (SURFACE)";
    printf("Reason: %-25s | Pump Action: %s\n", reason, action_str);
}

int step_control_logic() {
    int pump_action;

    // 1. Critical Safety Override: Crush Depth
    // This path makes 'target_depth' irrelevant.
    if (hull_pressure > CRUSH_DEPTH_PRESSURE) {
        pump_action = PUMP_ACTION_EMPTY; // Emergency surface
        log_pump_action("CRUSH DEPTH IMMINENT", pump_action);
    } 
    // 2. Standard Operational Logic
    else {
        // Logic depends on the CRV 'target_depth'
        if (current_depth < target_depth) {
            pump_action = PUMP_ACTION_FILL;
            log_pump_action("Diving to Target", pump_action);
        } else if (current_depth > target_depth) {
            pump_action = PUMP_ACTION_EMPTY;
            log_pump_action("Ascending to Target", pump_action);
        } else {
            pump_action = PUMP_ACTION_HOLD;
            log_pump_action("Depth Reached", pump_action);
        }
    }
    return pump_action;
}

int main() {
    srand(time(NULL));
    int pump_action = PUMP_ACTION_HOLD;
    printf("--- Submarine Ballast Control Simulation (Single Function) ---\n");

    for (int i = 0; i < 5; ++i) {
        read_sub_sensors();
        pump_action = step_control_logic();
        
        if (pump_action < -1 || pump_action > 1) {
            printf("!!! SAFETY VIOLATION: Invalid pump action !!!\n");
            return -1;
        }
    }

    return 0;
}

