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
volatile int target_rate_ml_hr;
volatile bool air_in_line_detected;
volatile int drug_profile_id; // (CRV candidate)
volatile bool pump_enabled;

// --- Function Prototypes ---
void read_pump_sensors();
void log_pump_state(const char* reason, int rate);
bool is_air_bubble_present();
int get_profile_max_rate();
int calculate_nominal_rate();
int apply_rate_saturation(int rate);
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
 * @brief Checks for the critical air-in-line condition.
 */
bool is_air_bubble_present() {
    return air_in_line_detected;
}

/**
 * @brief Determines the max allowed rate based on the drug profile.
 */
int get_profile_max_rate() {
    if (drug_profile_id == 1) return DRUG_PROFILE_A_MAX_RATE;
    if (drug_profile_id == 2) return DRUG_PROFILE_B_MAX_RATE;
    return 0; // Safe default
}

/**
 * @brief Calculates the nominal pump rate under normal conditions.
 */
int calculate_nominal_rate() {
    if (!pump_enabled) {
        log_pump_state("Pump disabled", 0);
        return 0;
    }

    int profile_max = get_profile_max_rate();
    int new_rate = target_rate_ml_hr;

    if (new_rate > profile_max) {
        new_rate = profile_max;
        log_pump_state("Rate limited by profile", new_rate);
    } else {
        log_pump_state("Normal operation", new_rate);
    }
    return new_rate;
}

/**
 * @brief Clamps the rate to the absolute physical limits of the pump.
 */
int apply_rate_saturation(int rate) {
    if (rate > MAX_PUMP_RATE_ML_HR) return MAX_PUMP_RATE_ML_HR;
    if (rate < 0) return 0;
    return rate;
}

/**
 * @brief Main pump control step.
 */
int step(int last_pump_rate) {
    int new_rate;

    // Priority 1: Critical Safety Check
    if (is_air_bubble_present()) {
        new_rate = 0;
        log_pump_state("AIR-IN-LINE ALARM", new_rate);
    } 
    // Priority 2: Normal Operation
    else {
        new_rate = calculate_nominal_rate();
    }

    return apply_rate_saturation(new_rate);
}

/**
 * @brief Main execution loop.
 */
int main() {
    int pump_rate = 0;
    printf("--- Medical Infusion Pump Simulation (Multi-Function) ---\n");

    for(int i = 0; i < 200; ++i) {
        read_pump_sensors();
        pump_rate = step(pump_rate);
        
        if (pump_rate < 0 || pump_rate > MAX_PUMP_RATE_ML_HR) {
            printf("!!! SAFETY VIOLATION !!!\n");
            return -1;
        }
        if (i == 1) air_in_line_detected = false;
    }

    return 0;
}

