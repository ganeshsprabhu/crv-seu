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
volatile float current_temp_c;
volatile float current_pressure_psi;
volatile int reaction_phase; // (CRV candidate)
volatile bool system_enabled;

// --- Function Prototypes ---
void read_reactor_sensors();
void log_reactor_state(const char* reason, int heater, int cooler);
bool is_pressure_critical();
int calculate_nominal_heater_duty(int last_duty);
int apply_heater_saturation(int duty);
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
 * @brief Checks for critical over-pressure condition.
 */
bool is_pressure_critical() {
    return current_pressure_psi >= PRESSURE_CRITICAL_PSI;
}

/**
 * @brief Calculates the nominal heater duty cycle based on temperature and phase.
 */
int calculate_nominal_heater_duty(int last_duty) {
    if (!system_enabled) {
        log_reactor_state("System Disabled", 0, 0);
        return 0;
    }

    float target_temp = (reaction_phase == 1) ? PHASE_1_TEMP_TARGET : PHASE_2_TEMP_TARGET;
    float temp_error = target_temp - current_temp_c;
    int new_duty;

    if (temp_error > 1.0f) {
        new_duty = last_duty + (int)(temp_error * 2.0f);
    } else if (temp_error < -1.0f) {
        new_duty = 0;
    } else {
        new_duty = last_duty;
    }
    
    char reason[50];
    sprintf(reason, "Phase %d Control (Target %.1fC)", reaction_phase, target_temp);
    log_reactor_state(reason, new_duty, 0); // Simplified logging
    return new_duty;
}

/**
 * @brief Clamps the heater duty cycle to its valid range.
 */
int apply_heater_saturation(int duty) {
    if (duty > HEATER_MAX_DUTY_CYCLE) return HEATER_MAX_DUTY_CYCLE;
    if (duty < 0) return 0;
    return duty;
}

/**
 * @brief Main control step function orchestrating calls.
 */
int step_heater(int last_heater_duty) {
    int new_heater_duty;

    // Priority 1: Critical Safety Logic
    if (is_pressure_critical()) {
        new_heater_duty = 0;
        log_reactor_state("OVER-PRESSURE OVERRIDE", 0, COOLING_MAX_PERCENT);
    } 
    // Priority 2: Standard Operational Logic
    else {
        new_heater_duty = calculate_nominal_heater_duty(last_heater_duty);
    }

    return apply_heater_saturation(new_heater_duty);
}

/**
 * @brief Main execution loop.
 */
int main() {
    int heater_duty_cycle = 0;
    printf("--- Chemical Reactor Control Simulation (Multi-Function) ---\n");

    for(int i = 0; i < 200; ++i) {
        read_reactor_sensors();
        heater_duty_cycle = step_heater(heater_duty_cycle);
        
        if (heater_duty_cycle < 0 || heater_duty_cycle > HEATER_MAX_DUTY_CYCLE) {
            printf("!!! SAFETY VIOLATION !!!\n");
            return -1;
        }
        if (i == 1) current_pressure_psi = 200;
    }

    return 0;
}

