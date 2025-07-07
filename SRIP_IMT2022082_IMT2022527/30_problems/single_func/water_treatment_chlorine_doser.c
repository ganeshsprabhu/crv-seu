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
volatile float water_flow_rate_lpm; // Liters per minute
volatile float post_treatment_chlorine_ppm;
volatile bool system_enabled;
volatile bool dosing_enabled_by_schedule; // (CRV candidate)

// --- Function Prototypes ---
void read_plant_sensors();
void log_doser_state(const char* reason, int rate);
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
 * @brief Main dosing pump control logic step.
 * @param last_pump_rate The previous pump rate.
 * @return The new calculated pump rate.
 */
int step(int last_pump_rate) {
    int new_rate = 0;

    // 1. CRITICAL SAFETY OVERRIDE: High Chlorine Level
    // This path makes 'dosing_enabled_by_schedule' and flow rate irrelevant.
    if (post_treatment_chlorine_ppm > HIGH_CHLORINE_ALARM_PPM) {
        new_rate = MIN_PUMP_RATE;
        log_doser_state("HIGH CHLORINE ALARM", new_rate);
    } 
    // 2. STANDARD OPERATIONAL LOGIC
    else if (system_enabled) {
        // Logic depends on the CRV 'dosing_enabled_by_schedule'
        if (dosing_enabled_by_schedule) {
            // Feed-forward control: pump rate is proportional to flow rate
            new_rate = (int)(water_flow_rate_lpm * 0.1f); // Simplified proportional constant
            log_doser_state("Feed-forward Control", new_rate);
        } else {
            new_rate = MIN_PUMP_RATE;
            log_doser_state("Dosing Inactive by Schedule", new_rate);
        }
    } 
    // 3. SYSTEM DISABLED
    else {
        new_rate = MIN_PUMP_RATE;
        log_doser_state("System Disabled", new_rate);
    }

    // 4. FINAL SAFETY SATURATION
    if (new_rate > MAX_PUMP_RATE) {
        new_rate = MAX_PUMP_RATE;
    }
    if (new_rate < MIN_PUMP_RATE) {
        new_rate = MIN_PUMP_RATE;
    }

    return new_rate;
}

/**
 * @brief Main execution loop.
 */
int main() {
    int pump_rate = 0;
    printf("--- Chlorine Doser Control Simulation ---\n");

    // Main control loop
    for(int i = 0; i < 200; ++i) {
        read_plant_sensors();
        pump_rate = step(pump_rate);
        
        // Safety property check
        if (pump_rate < MIN_PUMP_RATE || pump_rate > MAX_PUMP_RATE) {
            printf("!!! SAFETY VIOLATION !!!\n");
            return -1;
        }
        
        // Simulate a change for the next iteration
        if (i == 1) post_treatment_chlorine_ppm = 2.1f;
    }

    return 0;
}

