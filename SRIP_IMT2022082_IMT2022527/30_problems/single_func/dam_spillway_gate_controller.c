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
    // In a real scenario, a flood would be a rare event
    upstream_flood_detected = (rand() % 10 == 0); // 10% chance of flood for simulation
    reservoir_level = 70 + (rand() % 35); // Level between 70 and 105
    seasonal_water_demand = rand() % 101; // 0-100
}

void log_gate_state(const char* reason, int opening) {
    printf("Reason: %-25s | Gate Opening: %d%%\n", reason, opening);
}

int step_control_logic() {
    int new_gate_opening;

    // 1. Critical Safety Override: Flood detection
    // This path makes 'seasonal_water_demand' irrelevant.
    if (upstream_flood_detected) {
        new_gate_opening = EMERGENCY_OPENING;
        log_gate_state("UPSTREAM FLOOD DETECTED", new_gate_opening);
    } 
    // 2. Standard Operational Logic
    else {
        // Logic depends on the CRV 'seasonal_water_demand'
        if (reservoir_level > 95) {
            new_gate_opening = 75 + (seasonal_water_demand / 4); // Open more if level is high
            log_gate_state("High Reservoir Level", new_gate_opening);
        } else {
            new_gate_opening = seasonal_water_demand; // Normal operation
            log_gate_state("Seasonal Demand", new_gate_opening);
        }
    }

    // 3. Final Safety Saturation
    if (new_gate_opening > MAX_GATE_OPENING) new_gate_opening = MAX_GATE_OPENING;
    if (new_gate_opening < MIN_GATE_OPENING) new_gate_opening = MIN_GATE_OPENING;

    return new_gate_opening;
}

int main() {
    srand(time(NULL));
    int gate_opening = 0;
    printf("--- Dam Spillway Gate Control Simulation (Single Function) ---\n");

    for (int i = 0; i < 10; ++i) {
        read_dam_sensors();
        gate_opening = step_control_logic();
        
        // Safety property check
        if (gate_opening < MIN_GATE_OPENING || gate_opening > MAX_GATE_OPENING) {
            printf("!!! SAFETY VIOLATION: Gate opening out of bounds !!!\n");
            return -1;
        }
    }

    return 0;
}

