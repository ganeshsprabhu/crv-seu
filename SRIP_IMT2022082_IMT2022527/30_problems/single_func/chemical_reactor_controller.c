// chemical_reactor_controller.c
// Manages a heating element and cooling valve to maintain reaction temperature. A critical pressure sensor provides a safety override, making the reaction phase profile conditionally irrelevant.
// heater_duty_cycle >= 0 && heater_duty_cycle <= 100
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h> // For rand()

// --- Constants and Definitions ---
#define HEATER_MAX_DUTY_CYCLE 100
#define COOLING_MAX_PERCENT 100
#define PRESSURE_CRITICAL_PSI 500
#define PHASE_1_TEMP_TARGET 80.0f
#define PHASE_2_TEMP_TARGET 120.0f

// --- Simulated Hardware Inputs ---
volatile float current_temp_c;          // Current reactor temperature
volatile float current_pressure_psi;   // Current reactor pressure
volatile int reaction_phase;          // 1 or 2 (CRV candidate)
volatile bool system_enabled;         // Master enable switch

// --- Function Prototypes ---
void read_reactor_sensors();
void log_reactor_state(const char* reason, int heater, int cooler);
int step_heater(int last_heater_duty);

/**
 * @brief Simulates reading data from reactor sensors.
 */
void read_reactor_sensors() {
    // Simulate sensor readings with random values within realistic ranges
    current_temp_c = 70.0f + (rand() % 21);           // 70 to 90 C
    current_pressure_psi = 400 + (rand() % 201);      // 400 to 600 PSI
    reaction_phase = 1 + (rand() % 2);                // 1 or 2
    system_enabled = (rand() % 2) == 0 ? false : true;
}

/**
 * @brief Logs the output for simulation purposes.
 */
void log_reactor_state(const char* reason, int heater, int cooler) {
    printf("Logic: %-25s | Heater: %3d%% | Cooler: %3d%%\n", reason, heater, cooler);
}

/**
 * @brief Main control step function for the heater.
 * @param last_heater_duty The previous heater duty cycle.
 * @return The new calculated heater duty cycle.
 */
int step_heater(int last_heater_duty) {
    int new_heater_duty = last_heater_duty; 
    int cooler_output = 0; // For logging purposes
    float target_temp;

    // 1. CRITICAL SAFETY OVERRIDE: Over-pressure condition
    // This path makes 'reaction_phase' and its target temp irrelevant.
    if (current_pressure_psi >= PRESSURE_CRITICAL_PSI) {
        new_heater_duty = 0; // Immediately cut heat
        cooler_output = COOLING_MAX_PERCENT; // Maximize cooling
        log_reactor_state("OVER-PRESSURE OVERRIDE", new_heater_duty, cooler_output);

    } 
    // 2. STANDARD OPERATIONAL LOGIC
    else if (system_enabled) {
        // Determine target temperature based on the CRV 'reaction_phase'.
        if (reaction_phase == 1) {
            target_temp = PHASE_1_TEMP_TARGET;
        } else {
            target_temp = PHASE_2_TEMP_TARGET;
        }

        float temp_error = target_temp - current_temp_c;

        // Simple Proportional-Integral (PI) like control logic
        if (temp_error > 1.0f) {
            // Need more heat
            new_heater_duty = last_heater_duty + (int)(temp_error * 2.0f);
            cooler_output = 0;
        } else if (temp_error < -1.0f) {
            // Need cooling
            new_heater_duty = 0;
            cooler_output = (int)(-temp_error * 5.0f);
        } else {
            // In deadband, hold steady
            new_heater_duty = last_heater_duty;
            cooler_output = 0;
        }
        char reason[50];
    } 
    // 3. SYSTEM DISABLED
    else {
        new_heater_duty = 0;
        cooler_output = 0;
    }

    // 4. FINAL SAFETY SATURATION
    

    return new_heater_duty;
}

/**
 * @brief Main execution loop.
 */
int main() {
    int heater_duty_cycle = 0;
    printf("--- Chemical Reactor Control Simulation ---\n");

    // Main control loop
    for(int i = 0; i < 200; ++i) {
        current_temp_c = 70.0f + (rand() % 21);           // 70 to 90 C
        current_pressure_psi = 400 + (rand() % 201);      // 400 to 600 PSI
        reaction_phase = 1 + (rand() % 2);                // 1 or 2
        system_enabled = (rand() % 2) == 0 ? false : true;
        heater_duty_cycle = step_heater(heater_duty_cycle);
        
        // Safety property check
        if (heater_duty_cycle < 0 || heater_duty_cycle > HEATER_MAX_DUTY_CYCLE) {
            printf("!!! SAFETY VIOLATION !!!\n");
            return -1;
        }
        // Simulate a change for the next iteration
        if (i == 1) current_pressure_psi = 200;
    }

    return 0;
}

