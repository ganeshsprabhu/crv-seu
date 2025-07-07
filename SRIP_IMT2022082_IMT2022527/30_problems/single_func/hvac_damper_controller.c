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
volatile bool call_for_cooling;    // From thermostat (CRV candidate)
volatile bool system_on;           // HVAC system master switch

// --- Function Prototypes ---
void read_hvac_sensors();
void log_hvac_state(const char* reason, int position);
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
 * @brief Main HVAC damper control logic step.
 * @param last_damper_position The previous damper position.
 * @return The new calculated damper position.
 */
int step(int last_damper_position) {
    int new_position = last_damper_position;

    // 1. CRITICAL SAFETY OVERRIDE: Fire Alarm
    // This path makes 'call_for_cooling' and temperature readings irrelevant.
    if (fire_alarm_active) {
        // Force damper to the fire-safe position
        new_position = DAMPER_FIRE_SAFE_POSITION;
        log_hvac_state("FIRE ALARM OVERRIDE", new_position);
    } 
    // 2. STANDARD OPERATIONAL LOGIC
    else if (system_on) {
        // Logic depends on the CRV 'call_for_cooling'
        if (call_for_cooling) {
            // Open the damper to allow cool air
            new_position = DAMPER_MAX_OPEN;
            log_hvac_state("Call for Cooling", new_position);
        } else {
            // Close the damper as target temp is met
            new_position = DAMPER_MIN_OPEN;
            log_hvac_state("Target Temp Met", new_position);
        }
    } 
    // 3. SYSTEM IS OFF
    else {
        new_position = DAMPER_MIN_OPEN;
        log_hvac_state("System Off", new_position);
    }

    // 4. FINAL SAFETY SATURATION
    if (new_position > DAMPER_MAX_OPEN) {
        new_position = DAMPER_MAX_OPEN;
    }
    if (new_position < DAMPER_MIN_OPEN) {
        new_position = DAMPER_MIN_OPEN;
    }

    return new_position;
}

/**
 * @brief Main execution loop.
 */
int main() {
    int damper_position = 0;
    printf("--- HVAC Damper Control Simulation ---\n");

    // Main control loop
    for(int i = 0; i < 200; ++i) {
        current_temp_f = 65.0f + (rand() % 21); // 65 to 85 F
        target_temp_f = 68.0f + (rand() % 7);   // 68 to 74 F
        fire_alarm_active = (rand() % 5 == 0);  // 20% chance fire alarm is active
        call_for_cooling = (current_temp_f > target_temp_f);
        system_on = (rand() % 2 == 0);          // 50% chance system is on
        damper_position = step(damper_position);
        
        // Safety property check
        if (damper_position < DAMPER_MIN_OPEN || damper_position > DAMPER_MAX_OPEN) {
            printf("!!! SAFETY VIOLATION !!!\n");
            return -1;
        }
        
        // Simulate a change for the next iteration
        if (i == 1) fire_alarm_active = false;
    }

    return 0;
}

