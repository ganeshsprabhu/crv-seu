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
volatile int driver_mode; // (CRV candidate)
volatile int throttle_position;

// --- Function Prototypes ---
void read_engine_sensors();
void log_ecu_state(const char* reason, int rpm_limit);
bool is_engine_overheating();
int get_active_rpm_limit();
int calculate_fuel_injection(int rpm_limit);
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
 * @brief Checks for critical engine temperature.
 */
bool is_engine_overheating() {
    return engine_temp_c >= TEMP_CRITICAL_C;
}

/**
 * @brief Determines the current RPM limit based on mode or safety.
 */
int get_active_rpm_limit() {
    if (is_engine_overheating()) {
        log_ecu_state("OVERHEAT PROTECTION", TEMP_OVERRIDE_RPM_LIMIT);
        return TEMP_OVERRIDE_RPM_LIMIT;
    }

    if (driver_mode == 0) {
        log_ecu_state("Eco Mode Active", RPM_LIMIT_ECO);
        return RPM_LIMIT_ECO;
    } else {
        log_ecu_state("Sport Mode Active", RPM_LIMIT_SPORT);
        return RPM_LIMIT_SPORT;
    }
}

/**
 * @brief Calculates fuel injection duration based on the active limit.
 */
int calculate_fuel_injection(int rpm_limit) {
    if (engine_rpm > rpm_limit || engine_rpm > MAX_RPM_HARD_LIMIT) {
        return 0; // Fuel cut
    }
    int injection = throttle_position / 10;
    return (injection < 0) ? 0 : injection;
}

/**
 * @brief Main ECU control step.
 */
int step(int last_fuel_injection_ms) {
    // 1. Determine the active RPM limit from all conditions
    int active_limit = get_active_rpm_limit();

    // 2. Calculate fuel injection based on that limit
    int new_injection = calculate_fuel_injection(active_limit);

    return new_injection;
}

/**
 * @brief Main execution loop.
 */
int main() {
    int fuel_injection_ms = 0;
    printf("--- ECU RPM Limiter Simulation (Multi-Function) ---\n");

    for(int i = 0; i < 200; ++i) {
        read_engine_sensors();
        fuel_injection_ms = step(fuel_injection_ms);
        
        if (fuel_injection_ms < 0) {
            printf("!!! SAFETY VIOLATION !!!\n");
            return -1;
        }
        printf("Output Fuel Injection: %d ms\n\n", fuel_injection_ms);

        if (i == 1) engine_temp_c = 95;
    }

    return 0;
}

