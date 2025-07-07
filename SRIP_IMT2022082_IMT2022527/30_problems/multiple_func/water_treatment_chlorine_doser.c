// water_treatment_chlorine_doser.c
// Controls a dosing pump to add chlorine to water based on flow rate. A post-treatment chlorine sensor provides a critical safety override, halting the pump if chlorine levels are too high. The water flow rate, used for feed-forward control, is a CRV as it is ignored during a high-chlorine alarm.
// dosing_pump_rate >= 0 && dosing_pump_rate <= 100
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h> // For rand()

// --- Constants and Definitions ---
#define MAX_PUMP_RATE 100
#define MIN_PUMP_RATE 0
#define HIGH_CHLORINE_ALARM_PPM 4.0f
#define TARGET_CHLORINE_PPM 2.0f

// --- Simulated Hardware Inputs ---
volatile float water_flow_rate_lpm;
volatile float post_treatment_chlorine_ppm;
volatile bool system_enabled;
volatile bool dosing_enabled_by_schedule; // (CRV candidate)

// --- Function Prototypes ---
void read_plant_sensors();
void log_doser_state(const char* reason, int rate);
bool is_chlorine_level_critical();
int calculate_nominal_pump_rate();
int apply_rate_saturation(int rate);
int step(int last_pump_rate);

/**
 * @brief Simulates reading data from plant instrumentation.
 */
void read_plant_sensors() {
    // Simulate random sensor values within realistic ranges
    water_flow_rate_lpm = 400.0f + (rand() % 201); // 400 to 600 LPM
    post_treatment_chlorine_ppm = 1.0f + ((rand() % 60) / 10.0f); // 1.0 to 6.9 ppm
    system_enabled = (rand() % 2) == 0 ? true : false;
    dosing_enabled_by_schedule = (rand() % 2) == 0 ? true : false;
}

/**
 * @brief Logs the output for simulation purposes.
 */
void log_doser_state(const char* reason, int rate) {
    printf("Logic: %-25s | Dosing Pump Rate: %d%%\n", reason, rate);
}

/**
 * @brief Checks for a critical high chlorine reading.
 */
bool is_chlorine_level_critical() {
    return post_treatment_chlorine_ppm > HIGH_CHLORINE_ALARM_PPM;
}

/**
 * @brief Calculates pump rate under normal operating conditions.
 */
int calculate_nominal_pump_rate() {
    if (!system_enabled) {
        log_doser_state("System Disabled", MIN_PUMP_RATE);
        return MIN_PUMP_RATE;
    }
    if (dosing_enabled_by_schedule) {
        int rate = (int)(water_flow_rate_lpm * 0.1f);
        log_doser_state("Feed-forward Control", rate);
        return rate;
    } else {
        log_doser_state("Dosing Inactive by Schedule", MIN_PUMP_RATE);
        return MIN_PUMP_RATE;
    }
}

/**
 * @brief Clamps the pump rate to its physical limits.
 */
int apply_rate_saturation(int rate) {
    if (rate > MAX_PUMP_RATE) return MAX_PUMP_RATE;
    if (rate < MIN_PUMP_RATE) return MIN_PUMP_RATE;
    return rate;
}

/**
 * @brief Main dosing pump control step.
 */
int step(int last_pump_rate) {
    int new_rate;

    // Priority 1: Critical Safety Check
    if (is_chlorine_level_critical()) {
        new_rate = MIN_PUMP_RATE;
        log_doser_state("HIGH CHLORINE ALARM", new_rate);
    } 
    // Priority 2: Normal Operation
    else {
        new_rate = calculate_nominal_pump_rate();
    }

    return apply_rate_saturation(new_rate);
}

/**
 * @brief Main execution loop.
 */
int main() {
    int pump_rate = 0;
    printf("--- Chlorine Doser Control Simulation (Multi-Function) ---\n");

    for(int i = 0; i < 200; ++i) {
        read_plant_sensors();
        pump_rate = step(pump_rate);
        
        if (pump_rate < MIN_PUMP_RATE || pump_rate > MAX_PUMP_RATE) {
            printf("!!! SAFETY VIOLATION !!!\n");
            return -1;
        }

        if (i == 1) post_treatment_chlorine_ppm = 2.1f;
    }

    return 0;
}

