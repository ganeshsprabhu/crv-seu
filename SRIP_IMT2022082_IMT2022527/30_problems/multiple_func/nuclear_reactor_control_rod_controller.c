// nuclear_reactor_control_rod_controller.c
// Adjusts the position of control rods in a nuclear reactor. The rod position is normally managed to meet power grid demand (a CRV). A critical safety override is triggered by a seismic sensor or high coolant temperature, initiating a SCRAM (inserting all rods fully) to shut down the reactor, ignoring power demand.
// rod_insertion_depth >= 0 && rod_insertion_depth <= 100
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#define ROD_FULLY_INSERTED 100
#define ROD_FULLY_WITHDRAWN 0
#define HIGH_TEMP_THRESHOLD 800 // Celsius

// Simulated hardware inputs
volatile int coolant_temperature;
volatile bool seismic_event_detected;
volatile int power_grid_demand; // (CRV candidate) in MW

void read_reactor_sensors() {
    coolant_temperature = 750 + (rand() % 100);
    seismic_event_detected = (rand() % 20 == 0);
    power_grid_demand = 500 + (rand() % 100);
}

void log_rod_position(const char* reason, int depth) {
    printf("Reason: %-25s | Rod Insertion: %d%%\n", reason, depth);
}

bool is_scram_condition_met() {
    return (coolant_temperature > HIGH_TEMP_THRESHOLD || seismic_event_detected);
}

int calculate_normal_rod_depth() {
    int depth = 100 - (power_grid_demand / 10);
    log_rod_position("Meeting Power Demand", depth);
    return depth;
}

int apply_rod_limits(int depth) {
    if (depth > ROD_FULLY_INSERTED) return ROD_FULLY_INSERTED;
    if (depth < ROD_FULLY_WITHDRAWN) return ROD_FULLY_WITHDRAWN;
    return depth;
}

int step_control_logic() {
    int new_rod_depth;
    if (is_scram_condition_met()) {
        new_rod_depth = ROD_FULLY_INSERTED;
        log_rod_position("SCRAM ACTIVATED", new_rod_depth);
    } else {
        new_rod_depth = calculate_normal_rod_depth();
    }
    return apply_rod_limits(new_rod_depth);
}

int main() {
    srand(time(NULL));
    int rod_depth = ROD_FULLY_INSERTED;
    printf("--- Reactor Control Rod Simulation (Multi Function) ---\n");

    for (int i = 0; i < 5; ++i) {
        read_reactor_sensors();
        rod_depth = step_control_logic();
        
        if (rod_depth < ROD_FULLY_WITHDRAWN || rod_depth > ROD_FULLY_INSERTED) {
            printf("!!! SAFETY VIOLATION: Rod position out of bounds !!!\n");
            return -1;
        }
    }

    return 0;
}

