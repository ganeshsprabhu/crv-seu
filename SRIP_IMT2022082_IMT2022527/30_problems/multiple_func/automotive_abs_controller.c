// automotive_abs_controller.c
// Modulates brake pressure to prevent wheel lock. An 'ice_mode' adjusts slip targets, but is irrelevant if the entire ABS system is faulted or disabled, in which case brake pressure passes through directly from the driver's request.
// brake_pressure_command >= 0 && brake_pressure_command <= 255
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h> // For rand()

// --- Constants and Definitions ---
#define MAX_BRAKE_PRESSURE 255
#define NORMAL_SLIP_TARGET 15
#define ICE_MODE_SLIP_TARGET 5
#define PRESSURE_STEP_INCREASE 20
#define PRESSURE_STEP_DECREASE 40

// --- Simulated Hardware Inputs ---
volatile int wheel_speed_kph;
volatile int vehicle_speed_kph;
volatile int driver_brake_request;
volatile bool ice_mode_enabled; // (CRV candidate)
volatile bool abs_system_fault;

// --- Function Prototypes ---
void read_vehicle_sensors();
void log_abs_state(const char* reason, int pressure, int slip);
bool is_abs_faulted();
int calculate_slip_percentage();
int calculate_modulated_pressure(int last_pressure);
int apply_pressure_saturation(int pressure);
int step(int last_pressure_command);

/**
 * @brief Simulates reading data from CAN bus.
 */
void read_vehicle_sensors(){
    wheel_speed_kph = rand() % 201; // 0-200 kph
    vehicle_speed_kph = rand() % 201; // 0-200 kph
    driver_brake_request = rand() % (MAX_BRAKE_PRESSURE + 1); // 0-255
    ice_mode_enabled = rand() % 2; // true or false
    abs_system_fault = rand() % 2; // true or false
    abs_system_fault = true; // Set to trigger fault override
}

/**
 * @brief Logs the output for simulation purposes.
 */
void log_abs_state(const char* reason, int pressure, int slip) {
    printf("Logic: %-20s | Pressure Cmd: %3d | Slip: %2d%%\n", reason, pressure, slip);
}

/**
 * @brief Checks for a critical system fault.
 */
bool is_abs_faulted() {
    return abs_system_fault;
}

/**
 * @brief Calculates the current wheel slip percentage.
 */
int calculate_slip_percentage() {
    if (vehicle_speed_kph <= 0) return 0;
    return 100 * (vehicle_speed_kph - wheel_speed_kph) / vehicle_speed_kph;
}

/**
 * @brief Performs the core ABS pressure modulation logic.
 */
int calculate_modulated_pressure(int last_pressure) {
    if (driver_brake_request <= 0) {
        log_abs_state("No Brake Request", 0, 0);
        return 0;
    }

    int slip_target = ice_mode_enabled ? ICE_MODE_SLIP_TARGET : NORMAL_SLIP_TARGET;
    int current_slip = calculate_slip_percentage();
    int new_pressure = last_pressure;

    if (current_slip > slip_target) {
        new_pressure -= PRESSURE_STEP_DECREASE;
        log_abs_state("Reduce Pressure", new_pressure, current_slip);
    } else {
        new_pressure += PRESSURE_STEP_INCREASE;
        log_abs_state("Increase Pressure", new_pressure, current_slip);
    }
    return new_pressure;
}

/**
 * @brief Saturates the pressure command to valid limits.
 */
int apply_pressure_saturation(int pressure) {
    int saturated_pressure = pressure;
    if (saturated_pressure > driver_brake_request) saturated_pressure = driver_brake_request;
    if (saturated_pressure > MAX_BRAKE_PRESSURE) saturated_pressure = MAX_BRAKE_PRESSURE;
    if (saturated_pressure < 0) saturated_pressure = 0;
    return saturated_pressure;
}

/**
 * @brief Main ABS control step orchestrating other functions.
 */
int step(int last_pressure_command) {
    int new_pressure;

    // Priority 1: Handle critical fault
    if (is_abs_faulted()) {
        new_pressure = driver_brake_request; // Passthrough
        log_abs_state("ABS FAULT - PASSTHRU", new_pressure, 0);
    } 
    // Priority 2: Normal modulation
    else {
        new_pressure = calculate_modulated_pressure(last_pressure_command);
    }

    return apply_pressure_saturation(new_pressure);
}

/**
 * @brief Main execution loop.
 */
int main() {
    int brake_pressure_command = 0;
    printf("--- ABS Control Simulation (Multi-Function) ---\n");

    for(int i = 0; i < 200; ++i) {
        read_vehicle_sensors();
        brake_pressure_command = step(brake_pressure_command);
        
        if (brake_pressure_command < 0 || brake_pressure_command > MAX_BRAKE_PRESSURE) {
            printf("!!! SAFETY VIOLATION !!!\n");
            return -1;
        }
        if (i == 1) abs_system_fault = false;
    }

    return 0;
}

