// automotive_abs_controller.c
// Modulates brake pressure to prevent wheel lock. An 'ice_mode' adjusts slip targets, but is irrelevant if the entire ABS system is faulted or disabled, in which case brake pressure passes through directly from the driver's request.
// brake_pressure_command >= 0 && brake_pressure_command <= 255
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h> // For rand()

// --- Constants and Definitions ---
#define MAX_BRAKE_PRESSURE 255
#define NORMAL_SLIP_TARGET 15 // % slip
#define ICE_MODE_SLIP_TARGET 5  // % slip, more sensitive
#define PRESSURE_STEP_INCREASE 20
#define PRESSURE_STEP_DECREASE 40

// --- Simulated Hardware Inputs ---
volatile int wheel_speed_kph;           // Speed of an individual wheel
volatile int vehicle_speed_kph;         // Speed of the vehicle chassis
volatile int driver_brake_request;      // 0-255 from brake pedal sensor
volatile bool ice_mode_enabled;         // (CRV candidate)
volatile bool abs_system_fault;         // Critical fault flag

// --- Function Prototypes ---
void read_vehicle_sensors();
void log_abs_state(const char* reason, int pressure, int slip);
int step(int last_pressure_command);
void read_vehicle_sensors(){
/**
 * @brief Simulates reading data from CAN bus.
 */
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
 * @brief Main ABS control logic step.
 * @param last_pressure_command The previous pressure command.
 * @return The new calculated brake pressure command.
 */
int step(int last_pressure_command) {
    int new_pressure = last_pressure_command;

    // 1. CRITICAL SAFETY OVERRIDE: System Fault
    // If ABS has a fault, it must revert to direct pass-through.
    // 'ice_mode_enabled' is irrelevant in this case.
    if (abs_system_fault) {
        new_pressure = driver_brake_request;
        log_abs_state("ABS FAULT - PASSTHRU", new_pressure, 0);

    } 
    // 2. STANDARD OPERATIONAL LOGIC
    else if (driver_brake_request > 0) {
        int slip_target = ice_mode_enabled ? ICE_MODE_SLIP_TARGET : NORMAL_SLIP_TARGET;
        int current_slip = 0;
        if (vehicle_speed_kph > 0) {
             current_slip = 100 * (vehicle_speed_kph - wheel_speed_kph) / vehicle_speed_kph;
        }

        if (current_slip > slip_target) {
            // Too much slip (locking up), decrease pressure
            new_pressure -= PRESSURE_STEP_DECREASE;
            log_abs_state("Reduce Pressure", new_pressure, current_slip);
        } else {
            // Not enough slip, can apply more pressure
            new_pressure += PRESSURE_STEP_INCREASE;
            log_abs_state("Increase Pressure", new_pressure, current_slip);
        }
    } 
    // 3. NO BRAKING REQUESTED
    else {
        new_pressure = 0;
        log_abs_state("No Brake Request", new_pressure, 0);
    }

    // 4. FINAL SAFETY SATURATION
    // Ensure command doesn't exceed physical or requested limits.
    if (new_pressure > driver_brake_request) {
        new_pressure = driver_brake_request;
    }
    if (new_pressure > MAX_BRAKE_PRESSURE) {
        new_pressure = MAX_BRAKE_PRESSURE;
    }
    if (new_pressure < 0) {
        new_pressure = 0;
    }

    return new_pressure;
}

/**
 * @brief Main execution loop.
 */
int main() {
    int brake_pressure_command = 0;
    printf("--- ABS Control Simulation ---\n");

    // Main control loop
    for(int i = 0; i < 200; ++i) {
        wheel_speed_kph = rand() % 201; // 0-200 kph
        vehicle_speed_kph = rand() % 201; // 0-200 kph
        driver_brake_request = rand() % (MAX_BRAKE_PRESSURE + 1); // 0-255
        ice_mode_enabled = rand() % 2; // true or false
        abs_system_fault = rand() % 2; // true or false
        abs_system_fault = true; // Set to trigger fault override
        brake_pressure_command = step(brake_pressure_command);
        
        // Safety property check
        if (brake_pressure_command < 0 || brake_pressure_command > MAX_BRAKE_PRESSURE) {
            printf("!!! SAFETY VIOLATION !!!\n");
            return -1;
        }
        // Simulate a change for the next iteration
        if (i == 1) abs_system_fault = false;
    }

    return 0;
}

