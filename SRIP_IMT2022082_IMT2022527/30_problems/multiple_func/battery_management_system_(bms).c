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
volatile int charge_profile; // (CRV candidate)
volatile bool charger_connected;

// --- Function Prototypes ---
void read_bms_sensors();
void log_bms_state(const char* reason, int current);
bool is_cell_voltage_critical();
int calculate_nominal_charge_current();
int apply_current_saturation(int current);
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
 * @brief Checks for a critical cell over-voltage condition.
 */
bool is_cell_voltage_critical() {
    return max_cell_voltage_mv > CELL_OVERVOLTAGE_MV;
}

/**
 * @brief Calculates the charge current under normal operating conditions.
 */
int calculate_nominal_charge_current() {
    if (!charger_connected) {
        log_bms_state("Charger Disconnected", 0);
        return 0;
    }

    int target_current = (charge_profile == 0) ? PROFILE_LONGEVITY_MA : PROFILE_FAST_CHARGE_MA;
    const char* reason = (charge_profile == 0) ? "Longevity Profile" : "Fast Charge Profile";
    log_bms_state(reason, target_current);

    if (pack_temp_c > 45.0f) {
        target_current /= 2;
        log_bms_state("Temp Tapering", target_current);
    }
    return target_current;
}

/**
 * @brief Clamps the current to its absolute physical limits.
 */
int apply_current_saturation(int current) {
    if (current > MAX_CHARGE_CURRENT_MA) return MAX_CHARGE_CURRENT_MA;
    if (current < 0) return 0;
    return current;
}

/**
 * @brief Main BMS control step.
 */
int step(int last_charge_current) {
    int new_current;

    // Priority 1: Critical Safety Check
    if (is_cell_voltage_critical()) {
        new_current = 0;
        log_bms_state("CELL OVER-VOLTAGE ALARM", new_current);
    } 
    // Priority 2: Normal Operation
    else {
        new_current = calculate_nominal_charge_current();
    }

    return apply_current_saturation(new_current);
}

/**
 * @brief Main execution loop.
 */
int main() {
    int charge_current = 0;
    printf("--- Battery Management System Simulation (Multi-Function) ---\n");

    for(int i = 0; i < 200; ++i) {
        read_bms_sensors();
        charge_current = step(charge_current);
        
        if (charge_current < 0 || charge_current > MAX_CHARGE_CURRENT_MA) {
            printf("!!! SAFETY VIOLATION !!!\n");
            return -1;
        }

        if (i == 1) max_cell_voltage_mv = 4100;
    }

    return 0;
}

