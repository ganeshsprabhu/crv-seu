// medical_infusion_pump.c
// Controls the fluid delivery rate of an infusion pump. An air-in-line sensor provides a critical safety override, halting the pump immediately. The selected drug profile from a library, which defines rate limits, is a CRV because it is ignored when an air bubble is detected.
// pump_rate_ml_hr >= 0 && pump_rate_ml_hr <= 500
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h> // For rand()
// --- Constants and Definitions ---
#define MAX_PUMP_RATE_ML_HR 500
#define DRUG_PROFILE_A_MAX_RATE 100
#define DRUG_PROFILE_B_MAX_RATE 250

// --- Simulated Hardware Inputs ---
volatile int target_rate_ml_hr;      // Desired rate from user input
volatile bool air_in_line_detected;  // Critical safety sensor
volatile int drug_profile_id;        // 1 for A, 2 for B (CRV candidate)
volatile bool pump_enabled;          // Master switch

// --- Function Prototypes ---
void read_pump_sensors();
void log_pump_state(const char* reason, int rate);
int step(int last_pump_rate);

/**
 * @brief Simulates reading data from pump hardware.
 */
void read_pump_sensors() {
    target_rate_ml_hr = rand() % (MAX_PUMP_RATE_ML_HR + 1); // 0 to 500
    air_in_line_detected = rand() % 2; // 0 or 1
    drug_profile_id = (rand() % 2) + 1; // 1 or 2
    pump_enabled = rand() % 2; // 0 or 1
}

/**
 * @brief Logs the output for simulation purposes.
 */
void log_pump_state(const char* reason, int rate) {
    printf("Logic: %-25s | Pump Rate: %d ml/hr\n", reason, rate);
}

/**
 * @brief Main pump control logic step.
 * @param last_pump_rate The previous commanded rate.
 * @return The new calculated pump rate.
 */
int step(int last_pump_rate) {
    int new_rate = 0;

    // 1. CRITICAL SAFETY OVERRIDE: Air in line detected
    // This path makes 'drug_profile_id' and 'target_rate_ml_hr' irrelevant.
    if (air_in_line_detected) {
        new_rate = 0; // Immediately stop the pump
        log_pump_state("AIR-IN-LINE ALARM", new_rate);
    } 
    // 2. STANDARD OPERATIONAL LOGIC
    else if (pump_enabled) {
        int profile_max_rate;
        // Logic depends on the CRV 'drug_profile_id'
        if (drug_profile_id == 1) {
            profile_max_rate = DRUG_PROFILE_A_MAX_RATE;
        } else if (drug_profile_id == 2) {
            profile_max_rate = DRUG_PROFILE_B_MAX_RATE;
        } else {
            profile_max_rate = 0; // Unknown profile, default safe
        }

        // Set the rate but cap it at the profile's limit
        new_rate = target_rate_ml_hr;
        if (new_rate > profile_max_rate) {
            new_rate = profile_max_rate;
            log_pump_state("Rate limited by profile", new_rate);
        } else {
            log_pump_state("Normal operation", new_rate);
        }
    } 
    // 3. PUMP DISABLED
    else {
        new_rate = 0;
        log_pump_state("Pump disabled", new_rate);
    }

    // 4. FINAL SAFETY SATURATION
    // Absolute maximum rate check, regardless of profile.
    if (new_rate > MAX_PUMP_RATE_ML_HR) {
        new_rate = MAX_PUMP_RATE_ML_HR;
    }
    if (new_rate < 0) {
        new_rate = 0;
    }

    return new_rate;
}

/**
 * @brief Main execution loop.
 */
int main() {
    int pump_rate = 0;
    printf("--- Medical Infusion Pump Simulation ---\n");

    // Main control loop
    for(int i = 0; i < 200; ++i) {
        target_rate_ml_hr = rand() % (MAX_PUMP_RATE_ML_HR + 1); // 0 to 500
        air_in_line_detected = rand() % 2; // 0 or 1
        drug_profile_id = (rand() % 2) + 1; // 1 or 2
        pump_enabled = rand() % 2; // 0 or 1
        pump_rate = step(pump_rate);
        
        // Safety property check
        if (pump_rate < 0 || pump_rate > MAX_PUMP_RATE_ML_HR) {
            printf("!!! SAFETY VIOLATION !!!\n");
            return -1;
        }
        // Simulate a change for the next iteration
        if (i == 1) air_in_line_detected = false;
    }

    return 0;
}

