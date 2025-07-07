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
    hull_pressure = 800 + (rand() % 250);
    current_depth = hull_pressure / 10;
    target_depth = 120;
}

void log_pump_action(const char* reason, int action) {
    const char* action_str = "HOLD";
    if (action == PUMP_ACTION_FILL) action_str = "FILLING";
    if (action == PUMP_ACTION_EMPTY) action_str = "EMPTYING (SURFACE)";
    printf("Reason: %-25s | Pump Action: %s\n", reason, action_str);
}

bool is_crush_depth_imminent() {
    return hull_pressure > CRUSH_DEPTH_PRESSURE;
}

int calculate_normal_pump_action() {
    if (current_depth < target_depth) {
        log_pump_action("Diving to Target", PUMP_ACTION_FILL);
        return PUMP_ACTION_FILL;
    } else if (current_depth > target_depth) {
        log_pump_action("Ascending to Target", PUMP_ACTION_EMPTY);
        return PUMP_ACTION_EMPTY;
    } else {
        log_pump_action("Depth Reached", PUMP_ACTION_HOLD);
        return PUMP_ACTION_HOLD;
    }
}

int step_control_logic() {
    if (is_crush_depth_imminent()) {
        log_pump_action("CRUSH DEPTH IMMINENT", PUMP_ACTION_EMPTY);
        return PUMP_ACTION_EMPTY;
    } else {
        return calculate_normal_pump_action();
    }
}

int main() {
    srand(time(NULL));
    int pump_action = PUMP_ACTION_HOLD;
    printf("--- Submarine Ballast Control Simulation (Multi Function) ---\n");

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

