// traffic_light_system_with_emergency_preemption.c
// Controls a traffic light intersection based on a timed cycle. The current state in the timing cycle (a CRV) determines whether the light is red, yellow, or green. An emergency vehicle preemption signal (from an ambulance) provides a critical safety override, forcing the intersection to a safe state (e.g., all red) to clear a path, ignoring the standard timing cycle.
// light_state >= 0 && light_state <= 2
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#define STATE_RED 0
#define STATE_GREEN 1
#define STATE_YELLOW 2

// Simulated hardware inputs
volatile bool emergency_preemption_active;
volatile int cycle_timer; // (CRV candidate) in seconds

void read_traffic_sensors() {
    emergency_preemption_active = (rand() % 10 == 0);
    cycle_timer = rand() % 60;
}

const char* state_to_string(int state) {
    if (state == STATE_RED) return "RED";
    if (state == STATE_GREEN) return "GREEN";
    if (state == STATE_YELLOW) return "YELLOW";
    return "INVALID";
}

void log_light_state(const char* reason, int state) {
    printf("Reason: %-25s | Light State: %s\n", reason, state_to_string(state));
}

int step_control_logic() {
    int new_light_state;

    // 1. Critical Safety Override: Emergency Vehicle
    // This path makes 'cycle_timer' irrelevant.
    if (emergency_preemption_active) {
        new_light_state = STATE_RED;
        log_light_state("EMERGENCY PREEMPTION", new_light_state);
    } 
    // 2. Standard Operational Logic
    else {
        // Logic depends on the CRV 'cycle_timer'
        if (cycle_timer < 30) {
            new_light_state = STATE_GREEN;
        } else if (cycle_timer < 35) {
            new_light_state = STATE_YELLOW;
        } else {
            new_light_state = STATE_RED;
        }
        log_light_state("Normal Cycle", new_light_state);
    }
    return new_light_state;
}

int main() {
    srand(time(NULL));
    int light_state = STATE_RED;
    printf("--- Traffic Light Controller Simulation (Single Function) ---\n");

    for (int i = 0; i < 10; ++i) {
        read_traffic_sensors();
        light_state = step_control_logic();
        
        if (light_state < 0 || light_state > 2) {
            printf("!!! SAFETY VIOLATION: Invalid light state !!!\n");
            return -1;
        }
    }

    return 0;
}

