// data_center_cooling_unit.c
// Controls a chiller compressor based on server rack temperatures. A critical water leak sensor provides a safety override, immediately shutting down the entire unit. The server CPU load data, used for predictive cooling, is a CRV as it is completely ignored when a water leak is detected.
// compressor_state == 0 || compressor_state == 1
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h> // For rand()

// --- Constants and Definitions ---
#define COMPRESSOR_ON 1
#define COMPRESSOR_OFF 0
#define TEMP_HIGH_THRESHOLD_C 40.0f
#define TEMP_LOW_THRESHOLD_C 35.0f

// --- Simulated Hardware Inputs ---
volatile float rack_inlet_temp_c;
volatile bool water_leak_detected;    // Critical safety sensor
volatile int server_cpu_load_percent; // (CRV candidate)
volatile bool unit_enabled_by_master;

// --- Function Prototypes ---
void read_datacenter_sensors();
void log_cooling_state(const char* reason, bool state);
bool is_leak_detected();
int calculate_nominal_compressor_state(int last_state);
int step(int last_compressor_state);

/**
 * @brief Simulates reading data from data center sensors.
 */
void read_datacenter_sensors() {
    // Simulate random sensor values within realistic ranges
    rack_inlet_temp_c = 34.0f + (rand() % 10); // 34.0°C to 43.0°C
    water_leak_detected = (rand() % 5 == 0);   // 20% chance of leak
    server_cpu_load_percent = 50 + (rand() % 51); // 50% to 100%
    unit_enabled_by_master = (rand() % 2 == 0);   // Randomly enabled/disabled
}

/**
 * @brief Logs the output for simulation purposes.
 */
void log_cooling_state(const char* reason, bool state) {
    printf("Logic: %-25s | Compressor State: %s\n", reason, state ? "ON" : "OFF");
}

/**
 * @brief Checks for a critical water leak.
 */
bool is_leak_detected() {
    return water_leak_detected;
}

/**
 * @brief Calculates compressor state under normal operating conditions.
 */
int calculate_nominal_compressor_state(int last_state) {
    if (!unit_enabled_by_master) {
        log_cooling_state("Unit Disabled", COMPRESSOR_OFF);
        return COMPRESSOR_OFF;
    }

    if (server_cpu_load_percent > 90) {
        log_cooling_state("Predictive - High Load", COMPRESSOR_ON);
        return COMPRESSOR_ON;
    }
    if (rack_inlet_temp_c > TEMP_HIGH_THRESHOLD_C) {
        log_cooling_state("Reactive - Temp High", COMPRESSOR_ON);
        return COMPRESSOR_ON;
    }
    if (rack_inlet_temp_c < TEMP_LOW_THRESHOLD_C) {
        log_cooling_state("Reactive - Temp Low", COMPRESSOR_OFF);
        return COMPRESSOR_OFF;
    }

    log_cooling_state("Maintaining State", last_state);
    return last_state;
}

/**
 * @brief Main cooling unit control step.
 */
int step(int last_compressor_state) {
    int new_state;

    // Priority 1: Critical Safety Check
    if (is_leak_detected()) {
        new_state = COMPRESSOR_OFF;
        log_cooling_state("WATER LEAK DETECTED", new_state);
    } 
    // Priority 2: Normal Operation
    else {
        new_state = calculate_nominal_compressor_state(last_compressor_state);
    }

    // Final validation
    if (new_state != COMPRESSOR_ON && new_state != COMPRESSOR_OFF) {
        return COMPRESSOR_OFF;
    }
    return new_state;
}

/**
 * @brief Main execution loop.
 */
int main() {
    int compressor_state = COMPRESSOR_OFF;
    printf("--- Data Center Cooling Unit Simulation (Multi-Function) ---\n");

    for(int i = 0; i < 200; ++i) {
        read_datacenter_sensors();
        compressor_state = step(compressor_state);
        
        if (compressor_state != 0 && compressor_state != 1) {
            printf("!!! SAFETY VIOLATION !!!\n");
            return -1;
        }

        if (i == 1) water_leak_detected = false;
    }

    return 0;
}

