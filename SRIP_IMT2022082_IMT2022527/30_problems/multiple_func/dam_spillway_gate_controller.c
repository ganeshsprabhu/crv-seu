// dam_spillway_gate_controller.c
// Controls a dam's spillway gate opening. Normal operation is based on the reservoir level and a seasonal water demand profile (a CRV). An emergency override, triggered by an upstream flood sensor, forces the gate to fully open, completely ignoring the seasonal demand profile.
// gate_opening >= 0 && gate_opening <= 100
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

// Safety constants
#define MAX_GATE_OPENING 100
#define MIN_GATE_OPENING 0
#define EMERGENCY_OPENING 100

// Simulated hardware inputs
volatile bool upstream_flood_detected;
volatile int reservoir_level; // percentage full
volatile int seasonal_water_demand; // (CRV candidate)

void read_dam_sensors() {
    upstream_flood_detected = (rand() % 10 == 0); 
    reservoir_level = 70 + (rand() % 35);
    seasonal_water_demand = rand() % 101;
}

void log_gate_state(const char* reason, int opening) {
    printf("Reason: %-25s | Gate Opening: %d%%\n", reason, opening);
}

int calculate_normal_opening() {
    if (reservoir_level > 95) {
        log_gate_state("High Reservoir Level", 75 + (seasonal_water_demand / 4));
        return 75 + (seasonal_water_demand / 4);
    } else {
        log_gate_state("Seasonal Demand", seasonal_water_demand);
        return seasonal_water_demand;
    }
}

int apply_safety_saturation(int opening) {
    if (opening > MAX_GATE_OPENING) return MAX_GATE_OPENING;
    if (opening < MIN_GATE_OPENING) return MIN_GATE_OPENING;
    return opening;
}

int step_control_logic() {
    int new_gate_opening;

    if (upstream_flood_detected) {
        new_gate_opening = EMERGENCY_OPENING;
        log_gate_state("UPSTREAM FLOOD DETECTED", new_gate_opening);
    } else {
        new_gate_opening = calculate_normal_opening();
    }

    return apply_safety_saturation(new_gate_opening);
}

int main() {
    srand(time(NULL));
    int gate_opening = 0;
    printf("--- Dam Spillway Gate Control Simulation (Multi Function) ---\n");

    for (int i = 0; i < 10; ++i) {
        read_dam_sensors();
        gate_opening = step_control_logic();
        
        if (gate_opening < MIN_GATE_OPENING || gate_opening > MAX_GATE_OPENING) {
            printf("!!! SAFETY VIOLATION: Gate opening out of bounds !!!\n");
            return -1;
        }
    }

    return 0;
}

