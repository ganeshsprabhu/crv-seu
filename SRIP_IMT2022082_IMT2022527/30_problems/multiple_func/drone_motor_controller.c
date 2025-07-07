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
volatile int pilot_throttle_input;
volatile int battery_voltage_mv;
volatile int flight_mode; // (CRV candidate)
volatile bool armed;

// --- Function Prototypes ---
void read_drone_sensors();
void log_drone_state(const char* reason, int throttle);
bool is_battery_critical();
int calculate_normal_throttle(int last_throttle);
int apply_throttle_saturation(int throttle);
int step(int last_throttle_cmd);

/**
 * @brief Simulates reading data from drone sensors.
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
 * @brief Checks for a critical low battery condition.
 */
bool is_battery_critical() {
    return battery_voltage_mv < CRITICAL_BATTERY_VOLTAGE;
}

/**
 * @brief Calculates motor throttle under normal flight conditions.
 */
int calculate_normal_throttle(int last_throttle) {
    if (!armed) {
        log_drone_state("Disarmed", 0);
        return 0;
    }

    if (flight_mode == FLIGHT_MODE_ACRO) {
        log_drone_state("Acro Mode", pilot_throttle_input);
        return pilot_throttle_input;
    } else {
        int smoothed_throttle = (last_throttle + pilot_throttle_input) / 2;
        log_drone_state("Angle Mode", smoothed_throttle);
        return smoothed_throttle;
    }
}

/**
 * @brief Clamps the throttle command to its valid range.
 */
int apply_throttle_saturation(int throttle) {
    if (throttle > MAX_THROTTLE) return MAX_THROTTLE;
    if (throttle < 0) return 0;
    return throttle;
}

/**
 * @brief Main drone motor control step.
 */
int step(int last_throttle_cmd) {
    int new_throttle;

    // Priority 1: Critical Safety Check
    if (is_battery_critical()) {
        new_throttle = RTL_THROTTLE_COMMAND;
        log_drone_state("CRITICAL BATTERY - RTL", new_throttle);
    } 
    // Priority 2: Normal Operation
    else {
        new_throttle = calculate_normal_throttle(last_throttle_cmd);
    }

    return apply_throttle_saturation(new_throttle);
}

/**
 * @brief Main execution loop.
 */
int main() {
    int motor_throttle = 0;
    printf("--- Drone Motor Control Simulation (Multi-Function) ---\n");

    for(int i = 0; i < 200; ++i) {
        read_drone_sensors();
        motor_throttle = step(motor_throttle);
        
        if (motor_throttle < 0 || motor_throttle > MAX_THROTTLE) {
            printf("!!! SAFETY VIOLATION !!!\n");
            return -1;
        }

        if (i == 1) battery_voltage_mv = 3800;
    }

    return 0;
}

