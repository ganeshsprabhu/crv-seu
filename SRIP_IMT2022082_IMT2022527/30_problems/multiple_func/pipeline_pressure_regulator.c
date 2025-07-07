// pipeline_pressure_regulator.c
// Controls a valve to regulate pressure in a natural gas pipeline. The valve position is normally determined by downstream flow demand (a CRV). An upstream over-pressure sensor provides a critical safety override, forcing the valve to close to prevent a pipeline rupture, regardless of the flow demand.
// valve_position >= 0 && valve_position <= 100
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#define MAX_VALVE_POSITION 100
#define MIN_VALVE_POSITION 0
#define OVERPRESSURE_THRESHOLD 800 // PSI

// Simulated hardware inputs
volatile int upstream_pressure;
volatile int downstream_demand; // (CRV candidate) flow rate

void read_pipeline_sensors() {
    upstream_pressure = 750 + (rand() % 100);
    downstream_demand = 50 + (rand() % 50);
}

void log_valve_state(const char* reason, int position) {
    printf("Reason: %-25s | Valve Position: %d%%\n", reason, position);
}

bool is_overpressure_detected() {
    return upstream_pressure > OVERPRESSURE_THRESHOLD;
}

int calculate_demand_based_position() {
    log_valve_state("Matching Demand", downstream_demand);
    return downstream_demand;
}

int apply_valve_limits(int position) {
    if (position > MAX_VALVE_POSITION) return MAX_VALVE_POSITION;
    if (position < MIN_VALVE_POSITION) return MIN_VALVE_POSITION;
    return position;
}

int step_control_logic() {
    int new_valve_position;
    if (is_overpressure_detected()) {
        new_valve_position = MIN_VALVE_POSITION;
        log_valve_state("OVERPRESSURE SHUTDOWN", new_valve_position);
    } else {
        new_valve_position = calculate_demand_based_position();
    }
    return apply_valve_limits(new_valve_position);
}

int main() {
    srand(time(NULL));
    int valve_position = 50;
    printf("--- Pipeline Pressure Regulator Simulation (Multi Function) ---\n");

    for (int i = 0; i < 5; ++i) {
        read_pipeline_sensors();
        valve_position = step_control_logic();
        
        if (valve_position < MIN_VALVE_POSITION || valve_position > MAX_VALVE_POSITION) {
            printf("!!! SAFETY VIOLATION: Valve position out of bounds !!!\n");
            return -1;
        }
    }

    return 0;
}

