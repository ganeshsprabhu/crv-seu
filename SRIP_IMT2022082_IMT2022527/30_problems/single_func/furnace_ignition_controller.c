// furnace_ignition_controller.c
// Controls the gas valve in a home furnace. Normal operation opens the valve when the thermostat calls for heat (a CRV). A critical safety feature is a flame sensor; if it fails to detect a flame within a few seconds of the valve opening, it forces the valve to shut off to prevent gas leaks, overriding the thermostat's call for heat.
// gas_valve_state == 0 || gas_valve_state == 1
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#define VALVE_CLOSED 0
#define VALVE_OPEN 1
#define IGNITION_TIMEOUT 5 // seconds

// Simulated hardware inputs
volatile bool thermostat_calls_for_heat; // (CRV candidate)
volatile bool flame_detected;
volatile int time_since_valve_open;

void read_furnace_sensors(bool valve_is_open) {
    thermostat_calls_for_heat = true; // Assume thermostat is always calling for heat
    // If valve is open, there's a small chance of ignition failure
    if (valve_is_open) {
        flame_detected = (rand() % 10 != 0); // 90% success rate
    } else {
        flame_detected = false;
    }
}

void log_valve_state(const char* reason, int state) {
    printf("Reason: %-25s | Gas Valve: %s\n", reason, state == VALVE_OPEN ? "OPEN" : "CLOSED");
}

int step_control_logic(int current_valve_state) {
    int new_valve_state = current_valve_state;

    if (current_valve_state == VALVE_OPEN) {
        time_since_valve_open++;
        // 1. Critical Safety Override: Ignition Failure
        // This path overrides the 'thermostat_calls_for_heat' CRV.
        if (time_since_valve_open > IGNITION_TIMEOUT && !flame_detected) {
            new_valve_state = VALVE_CLOSED;
            log_valve_state("IGNITION TIMEOUT LOCKOUT", new_valve_state);
        } else {
            log_valve_state("Valve Open, Flame OK", VALVE_OPEN);
        }
    } 
    // 2. Standard Operational Logic
    else { // Valve is closed
        if (thermostat_calls_for_heat) {
            new_valve_state = VALVE_OPEN;
            time_since_valve_open = 0;
            log_valve_state("Thermostat Call", new_valve_state);
        }
    }
    return new_valve_state;
}

int main() {
    srand(time(NULL));
    int gas_valve_state = VALVE_CLOSED;
    time_since_valve_open = 0;
    printf("--- Furnace Ignition Control Simulation (Single Function) ---\n");

    for (int i = 0; i < 10; i++) {
        read_furnace_sensors(gas_valve_state == VALVE_OPEN);
        gas_valve_state = step_control_logic(gas_valve_state);
        
        if (gas_valve_state < 0 || gas_valve_state > 1) {
            printf("!!! SAFETY VIOLATION: Invalid valve state !!!\n");
            return -1;
        }
        if (gas_valve_state == VALVE_CLOSED && time_since_valve_open > IGNITION_TIMEOUT) {
            printf("Furnace in safety lockout. Halting.\n");
            break;
        }
    }

    return 0;
}

