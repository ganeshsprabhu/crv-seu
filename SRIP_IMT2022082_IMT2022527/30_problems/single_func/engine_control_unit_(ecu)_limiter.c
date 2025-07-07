// engine_control_unit_(ecu)_limiter.c
// Manages the engine's RPM by cutting fuel injection. A critical engine over-temperature sensor forces a very low RPM limit to protect the engine. The driver-selected mode (Eco, Sport), which sets different performance RPM limits, is a CRV as it is ignored during an overheat condition.
// fuel_injection_ms >= 0
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h> // For rand()

// --- Constants and Definitions ---
#define MAX_RPM_HARD_LIMIT 9000
#define RPM_LIMIT_ECO 6000
#define RPM_LIMIT_SPORT 8500
#define TEMP_CRITICAL_C 120
#define TEMP_OVERRIDE_RPM_LIMIT 3000

// --- Simulated Hardware Inputs ---
volatile int engine_rpm;
volatile int engine_temp_c;
volatile int driver_mode; // 0: Eco, 1: Sport (CRV candidate)
volatile int throttle_position; // 0-100

// --- Function Prototypes ---
void read_engine_sensors();
void log_ecu_state(const char* reason, int rpm_limit);
int step(int last_fuel_injection_ms);

/**
 * @brief Simulates reading data from the engine's CAN bus.
 */
void read_engine_sensors() {
    engine_rpm = 5000 + rand() % 5000;           // 5000 to 9999
    engine_temp_c = 90 + rand() % 50;            // 90 to 139
    driver_mode = rand() % 2;                    // 0 (Eco) or 1 (Sport)
    throttle_position = rand() % 101;            // 0 to 100
}

/**
 * @brief Logs the output for simulation purposes.
 */
void log_ecu_state(const char* reason, int rpm_limit) {
    printf("Logic: %-25s | Active RPM Limit: %d\n", reason, rpm_limit);
}

/**
 * @brief Main ECU control step for fuel injection.
 * @param last_fuel_injection_ms The previous fuel injection duration.
 * @return The new fuel injection duration in ms (0 means fuel cut).
 */
int step(int last_fuel_injection_ms) {
    int new_fuel_injection = 10; // Default injection
    int active_rpm_limit;

    // 1. CRITICAL SAFETY OVERRIDE: Engine Over-temperature
    // This path makes 'driver_mode' irrelevant.
    if (engine_temp_c >= TEMP_CRITICAL_C) {
        active_rpm_limit = TEMP_OVERRIDE_RPM_LIMIT;
        log_ecu_state("OVERHEAT PROTECTION", active_rpm_limit);
    } 
    // 2. STANDARD OPERATIONAL LOGIC
    else {
        // Determine RPM limit based on the CRV 'driver_mode'
        if (driver_mode == 0) {
            active_rpm_limit = RPM_LIMIT_ECO;
            log_ecu_state("Eco Mode Active", active_rpm_limit);
        } else {
            active_rpm_limit = RPM_LIMIT_SPORT;
            log_ecu_state("Sport Mode Active", active_rpm_limit);
        }
    }

    // 3. APPLY RPM LIMIT
    // This logic is separate from determining the limit.
    if (engine_rpm > active_rpm_limit || engine_rpm > MAX_RPM_HARD_LIMIT) {
        new_fuel_injection = 0; // Cut fuel
    } else {
        // Calculate normal fuel injection based on throttle
        new_fuel_injection = throttle_position / 10;
    }

    // 4. FINAL SAFETY SATURATION
    if (new_fuel_injection < 0) {
        new_fuel_injection = 0;
    }

    return new_fuel_injection;
}

/**
 * @brief Main execution loop.
 */
int main() {
    int fuel_injection_ms = 0;
    printf("--- ECU RPM Limiter Simulation ---\n");

    // Main control loop
    for(int i = 0; i < 200; ++i) {
        engine_rpm = 5000 + rand() % 5000;           // 5000 to 9999
        engine_temp_c = 90 + rand() % 50;            // 90 to 139
        driver_mode = rand() % 2;                    // 0 (Eco) or 1 (Sport)
        throttle_position = rand() % 101;            // 0 to 100
        fuel_injection_ms = step(fuel_injection_ms);
        
        // Safety property check
        if (fuel_injection_ms < 0) {
            printf("!!! SAFETY VIOLATION !!!\n");
            return -1;
        }
        printf("Output Fuel Injection: %d ms\n\n", fuel_injection_ms);

        // Simulate a change for the next iteration
        if (i == 1) engine_temp_c = 95;
    }

    return 0;
}

