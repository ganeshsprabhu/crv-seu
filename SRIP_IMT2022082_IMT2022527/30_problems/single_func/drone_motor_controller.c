// drone_motor_controller.c
// Manages motor throttle commands based on pilot input and flight mode. A critical low battery voltage triggers a 'Return to Land' (RTL) mode, which provides a fixed, gentle descent command. The pilot's selected flight mode (e.g., 'Acro' vs. 'Angle'), which affects throttle response, is a CRV as it is ignored during critical battery RTL.
// motor_throttle_cmd >= 0 && motor_throttle_cmd <= 1000
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h> // For rand()

// --- Constants and Definitions ---
#define MAX_THROTTLE 1000
#define CRITICAL_BATTERY_VOLTAGE 3300 // 3.3V per cell
#define RTL_THROTTLE_COMMAND 400 // Gentle descent throttle
#define FLIGHT_MODE_ACRO 0
#define FLIGHT_MODE_ANGLE 1

// --- Simulated Hardware Inputs ---
volatile int pilot_throttle_input; // 0-1000 from RC receiver
volatile int battery_voltage_mv;
volatile int flight_mode; // (CRV candidate)
volatile bool armed;

// --- Function Prototypes ---
void read_drone_sensors();
void log_drone_state(const char* reason, int throttle);
int step(int last_throttle_cmd);

/**
 * @brief Simulates reading data from drone sensors and RC receiver.
 */
void read_drone_sensors() {
    pilot_throttle_input = rand() % (MAX_THROTTLE + 1); // 0 to 1000
    battery_voltage_mv = 3200 + rand() % 801; // 3200 to 4000 mV
    flight_mode = rand() % 2; // 0 (Acro) or 1 (Angle)
    armed = rand() % 2; // true or false
}

/**
 * @brief Logs the output for simulation purposes.
 */
void log_drone_state(const char* reason, int throttle) {
    printf("Logic: %-25s | Motor Command: %d\n", reason, throttle);
}

/**
 * @brief Main drone motor control logic step.
 * @param last_throttle_cmd The previous motor command.
 * @return The new calculated motor throttle command.
 */
int step(int last_throttle_cmd) {
    int new_throttle = 0;

    // 1. CRITICAL SAFETY OVERRIDE: Low Battery Failsafe
    // This path makes 'flight_mode' and pilot input irrelevant.
    if (battery_voltage_mv < CRITICAL_BATTERY_VOLTAGE) {
        new_throttle = RTL_THROTTLE_COMMAND;
        log_drone_state("CRITICAL BATTERY - RTL", new_throttle);
    } 
    // 2. STANDARD OPERATIONAL LOGIC
    else if (armed) {
        // Logic depends on the CRV 'flight_mode'
        if (flight_mode == FLIGHT_MODE_ACRO) {
            // Acro mode has a direct, more aggressive response
            new_throttle = pilot_throttle_input;
            log_drone_state("Acro Mode", new_throttle);
        } else { // Angle Mode
            // Angle mode might have a smoothed/dampened response
            new_throttle = (last_throttle_cmd + pilot_throttle_input) / 2;
            log_drone_state("Angle Mode", new_throttle);
        }
    } 
    // 3. DRONE IS DISARMED
    else {
        new_throttle = 0;
        log_drone_state("Disarmed", new_throttle);
    }

    // 4. FINAL SAFETY SATURATION
    if (new_throttle > MAX_THROTTLE) {
        new_throttle = MAX_THROTTLE;
    }
    if (new_throttle < 0) {
        new_throttle = 0;
    }

    return new_throttle;
}

/**
 * @brief Main execution loop.
 */
int main() {
    int motor_throttle = 0;
    printf("--- Drone Motor Control Simulation ---\n");

    // Main control loop
    for(int i = 0; i < 200; ++i) {
        pilot_throttle_input = rand() % (MAX_THROTTLE + 1); // 0 to 1000
        battery_voltage_mv = 3200 + rand() % 801; // 3200 to 4000 mV
        flight_mode = rand() % 2; // 0 (Acro) or 1 (Angle)
        armed = rand() % 2; // true or false
        motor_throttle = step(motor_throttle);
        
        // Safety property check
        if (motor_throttle < 0 || motor_throttle > MAX_THROTTLE) {
            printf("!!! SAFETY VIOLATION !!!\n");
            return -1;
        }
        
        // Simulate a change for the next iteration
        if (i == 1) battery_voltage_mv = 3800;
    }

    return 0;
}

