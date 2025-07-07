// battery_management_system_(bms).c
// Controls the charging current for a battery pack. A critical cell over-voltage sensor provides a safety override, immediately cutting charging current. The user-selected charging profile (e.g., 'fast' vs 'longevity'), which sets the target current, is a CRV as it is ignored during an over-voltage event.
// charge_current_ma >= 0 && charge_current_ma <= 5000
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h> // For rand()
// --- Constants and Definitions ---
#define MAX_CHARGE_CURRENT_MA 5000
#define CELL_OVERVOLTAGE_MV 4250
#define PROFILE_FAST_CHARGE_MA 4000
#define PROFILE_LONGEVITY_MA 1500

// --- Simulated Hardware Inputs ---
volatile int max_cell_voltage_mv;
volatile float pack_temp_c;
volatile int charge_profile; // 0: Longevity, 1: Fast (CRV candidate)
volatile bool charger_connected;

// --- Function Prototypes ---
void read_bms_sensors();
void log_bms_state(const char* reason, int current);
int step(int last_charge_current);

/**
 * @brief Simulates reading data from BMS hardware.
 */
void read_bms_sensors() {
    // Simulate sensor readings with random values within realistic ranges
    max_cell_voltage_mv = 4000 + rand() % 300; // 4000-4299 mV
    pack_temp_c = 20.0f + (rand() % 400) / 10.0f; // 20.0-59.9 C
    charge_profile = rand() % 2; // 0 or 1
    charger_connected = (rand() % 2) == 0 ? false : true;
}

/**
 * @brief Logs the output for simulation purposes.
 */
void log_bms_state(const char* reason, int current) {
    printf("Logic: %-25s | Charge Current: %d mA\n", reason, current);
}

/**
 * @brief Main BMS control logic step.
 * @param last_charge_current The previous charge current command.
 * @return The new calculated charge current.
 */
int step(int last_charge_current) {
    int new_current = 0;

    // 1. CRITICAL SAFETY OVERRIDE: Cell Over-voltage
    // This path makes 'charge_profile' and temperature irrelevant to the output.
    if (max_cell_voltage_mv > CELL_OVERVOLTAGE_MV) {
        new_current = 0; // Immediately stop charging
        log_bms_state("CELL OVER-VOLTAGE ALARM", new_current);
    } 
    // 2. STANDARD OPERATIONAL LOGIC
    else if (charger_connected) {
        int target_current;
        // Logic depends on the CRV 'charge_profile'
        if (charge_profile == 0) {
            target_current = PROFILE_LONGEVITY_MA;
            log_bms_state("Longevity Profile", target_current);
        } else {
            target_current = PROFILE_FAST_CHARGE_MA;
            log_bms_state("Fast Charge Profile", target_current);
        }

        // Additional logic, e.g., temperature tapering
        if (pack_temp_c > 45.0f) {
            target_current /= 2;
            log_bms_state("Temp Tapering", target_current);
        }
        new_current = target_current;
    } 
    // 3. CHARGER DISCONNECTED
    else {
        new_current = 0;
        log_bms_state("Charger Disconnected", new_current);
    }

    // 4. FINAL SAFETY SATURATION
    if (new_current > MAX_CHARGE_CURRENT_MA) {
        new_current = MAX_CHARGE_CURRENT_MA;
    }
    if (new_current < 0) {
        new_current = 0;
    }

    return new_current;
}

/**
 * @brief Main execution loop.
 */
int main() {
    int charge_current = 0;
    printf("--- Battery Management System Simulation ---\n");

    // Main control loop
    for(int i = 0; i < 200; ++i) {
        max_cell_voltage_mv = 4000 + rand() % 300; // 4000-4299 mV
        pack_temp_c = 20.0f + (rand() % 400) / 10.0f; // 20.0-59.9 C
        charge_profile = rand() % 2; // 0 or 1
        charger_connected = (rand() % 2) == 0 ? false : true;
        charge_current = step(charge_current);
        
        // Safety property check
        if (charge_current < 0 || charge_current > MAX_CHARGE_CURRENT_MA) {
            printf("!!! SAFETY VIOLATION !!!\n");
            return -1;
        }
        
        // Simulate a change for the next iteration
        if (i == 1) max_cell_voltage_mv = 4100;
    }

    return 0;
}

