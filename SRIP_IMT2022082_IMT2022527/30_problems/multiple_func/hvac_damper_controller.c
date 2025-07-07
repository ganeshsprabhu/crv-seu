// hvac_damper_controller.c
// Controls an air damper's position to regulate airflow based on temperature. A fire alarm signal from the building's safety system is a critical override that forces the damper to a predefined safe position (e.g., closed). The thermostat's call for cooling, which normally controls the damper, is a CRV as it is ignored during a fire alarm.
// damper_position >= 0 && damper_position <= 100
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h> // For rand()

// --- Constants and Definitions ---
#define DAMPER_MAX_OPEN 100
#define DAMPER_MIN_OPEN 0
#define DAMPER_FIRE_SAFE_POSITION 0 // Fully closed

// --- Simulated Hardware Inputs ---
volatile float current_temp_f;
volatile float target_temp_f;
volatile bool fire_alarm_active;   // Critical safety input
volatile bool call_for_cooling;    // (CRV candidate)
volatile bool system_on;

// --- Function Prototypes ---
void read_hvac_sensors();
void log_hvac_state(const char* reason, int position);
bool is_fire_alarm_triggered();
int calculate_nominal_damper_position();
int apply_position_saturation(int position);
int step(int last_damper_position);

/**
 * @brief Simulates reading data from building control system.
 */
void read_hvac_sensors() {
    // Simulate random sensor values and system states
    current_temp_f = 65.0f + (rand() % 21); // 65 to 85 F
    target_temp_f = 68.0f + (rand() % 7);   // 68 to 74 F
    fire_alarm_active = (rand() % 5 == 0);  // 20% chance fire alarm is active
    call_for_cooling = (current_temp_f > target_temp_f);
    system_on = (rand() % 2 == 0);          // 50% chance system is on
}

/**
 * @brief Logs the output for simulation purposes.
 */
void log_hvac_state(const char* reason, int position) {
    printf("Logic: %-25s | Damper Position: %d%%\n", reason, position);
}

/**
 * @brief Checks for a critical fire alarm state.
 */
bool is_fire_alarm_triggered() {
    return fire_alarm_active;
}

/**
 * @brief Calculates damper position based on normal HVAC logic.
 */
int calculate_nominal_damper_position() {
    if (!system_on) {
        log_hvac_state("System Off", DAMPER_MIN_OPEN);
        return DAMPER_MIN_OPEN;
    }

    if (call_for_cooling) {
        log_hvac_state("Call for Cooling", DAMPER_MAX_OPEN);
        return DAMPER_MAX_OPEN;
    } else {
        log_hvac_state("Target Temp Met", DAMPER_MIN_OPEN);
        return DAMPER_MIN_OPEN;
    }
}

/**
 * @brief Clamps the damper position to its physical limits.
 */
int apply_position_saturation(int position) {
    if (position > DAMPER_MAX_OPEN) return DAMPER_MAX_OPEN;
    if (position < DAMPER_MIN_OPEN) return DAMPER_MIN_OPEN;
    return position;
}

/**
 * @brief Main HVAC damper control step.
 */
int step(int last_damper_position) {
    int new_position;

    // Priority 1: Critical Safety Check
    if (is_fire_alarm_triggered()) {
        new_position = DAMPER_FIRE_SAFE_POSITION;
        log_hvac_state("FIRE ALARM OVERRIDE", new_position);
    } 
    // Priority 2: Normal Operation
    else {
        new_position = calculate_nominal_damper_position();
    }

    return apply_position_saturation(new_position);
}

/**
 * @brief Main execution loop.
 */
int main() {
    int damper_position = 0;
    printf("--- HVAC Damper Control Simulation (Multi-Function) ---\n");

    for(int i = 0; i < 200; ++i) {
        read_hvac_sensors();
        damper_position = step(damper_position);
        
        if (damper_position < DAMPER_MIN_OPEN || damper_position > DAMPER_MAX_OPEN) {
            printf("!!! SAFETY VIOLATION !!!\n");
            return -1;
        }

        if (i == 1) fire_alarm_active = false;
    }

    return 0;
}

