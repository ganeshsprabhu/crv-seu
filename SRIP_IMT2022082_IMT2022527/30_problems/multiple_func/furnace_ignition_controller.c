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
    thermostat_calls_for_heat = true;
    if (valve_is_open) {
        flame_detected = (rand() % 10 != 0);
    } else {
        flame_detected = false;
    }
}

void log_valve_state(const char* reason, int state) {
    printf("Reason: %-25s | Gas Valve: %s\n", reason, state == VALVE_OPEN ? "OPEN" : "CLOSED");
}

bool has_ignition_timed_out() {
    return (time_since_valve_open > IGNITION_TIMEOUT && !flame_detected);
}

int handle_open_valve_logic() {
    time_since_valve_open++;
    if (has_ignition_timed_out()) {
        log_valve_state("IGNITION TIMEOUT LOCKOUT", VALVE_CLOSED);
        return VALVE_CLOSED;
    } else {
        log_valve_state("Valve Open, Flame OK", VALVE_OPEN);
        return VALVE_OPEN;
    }
}

int handle_closed_valve_logic() {
    if (thermostat_calls_for_heat) {
        time_since_valve_open = 0;
        log_valve_state("Thermostat Call", VALVE_OPEN);
        return VALVE_OPEN;
    }
    return VALVE_CLOSED;
}

int step_control_logic(int current_valve_state) {
    if (current_valve_state == VALVE_OPEN) {
        return handle_open_valve_logic();
    } else {
        return handle_closed_valve_logic();
    }
}

int main() {
    srand(time(NULL));
    int gas_valve_state = VALVE_CLOSED;
    time_since_valve_open = 0;
    printf("--- Furnace Ignition Control Simulation (Multi Function) ---\n");

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

