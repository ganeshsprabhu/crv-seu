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
volatile int server_cpu_load_percent; // For predictive logic (CRV candidate)
volatile bool unit_enabled_by_master;

// --- Function Prototypes ---
void read_datacenter_sensors();
void log_cooling_state(const char* reason, bool state);
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

/**
 * @brief Main cooling unit control logic step.
 * @param last_compressor_state The previous state of the compressor.
 * @return The new compressor state (ON/OFF).
 */
int step(int last_compressor_state) {
    int new_state = last_compressor_state;

    // 1. CRITICAL SAFETY OVERRIDE: Water Leak Detected
    // This path makes 'server_cpu_load_percent' and temperatures irrelevant.
    if (water_leak_detected) {
        new_state = COMPRESSOR_OFF;
        log_cooling_state("WATER LEAK DETECTED", new_state);
    } 
    // 2. STANDARD OPERATIONAL LOGIC
    else if (unit_enabled_by_master) {
        // Predictive logic using the CRV 'server_cpu_load_percent'
        if (server_cpu_load_percent > 90) {
            new_state = COMPRESSOR_ON;
        } 
        // Reactive logic based on temperature thresholds
        else if (rack_inlet_temp_c > TEMP_HIGH_THRESHOLD_C) {
            new_state = COMPRESSOR_ON;
        } else if (rack_inlet_temp_c < TEMP_LOW_THRESHOLD_C) {
            new_state = COMPRESSOR_OFF;
        } else {
            // In deadband, maintain last state
            new_state = last_compressor_state;
        }
    } 
    // 3. UNIT DISABLED
    else {
        new_state = COMPRESSOR_OFF;
    }

    // 4. FINAL SAFETY VALIDATION (state is already binary)
    if (new_state != COMPRESSOR_ON && new_state != COMPRESSOR_OFF) {
        new_state = COMPRESSOR_OFF; // Default to safe state
    }

    return new_state;
}

/**
 * @brief Main execution loop.
 */
int main() {
    int compressor_state = COMPRESSOR_OFF;
    printf("--- Data Center Cooling Unit Simulation ---\n");

    // Main control loop
    for(int i = 0; i < 200; ++i) {
        // Simulate random sensor values within realistic ranges
        rack_inlet_temp_c = 34.0f + (rand() % 10); // 34.0°C to 43.0°C
        water_leak_detected = (rand() % 5 == 0);   // 20% chance of leak
        server_cpu_load_percent = 50 + (rand() % 51); // 50% to 100%
        unit_enabled_by_master = (rand() % 2 == 0);   // Randomly enabled/disabled
        compressor_state = step(compressor_state);
        
        
        // Simulate a change for the next iteration
        if (i == 1) water_leak_detected = false;
    }

    return 0;
}

