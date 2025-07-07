// aircraft_pitch_controller_with_alpha_protection.c
// Controls elevator deflection to maintain a target pitch angle based on the active autopilot mode. A critical Angle of Attack (Alpha) protection system provides a safety override to prevent stalls, making the autopilot mode conditionally irrelevant.
// elevator_deflection >= -25 && elevator_deflection <= 15
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h> // For rand()

// --- Constants and Definitions ---
#define ELEVATOR_MAX_UP_DEFLECTION 15
#define ELEVATOR_MAX_DOWN_DEFLECTION -25
#define ALPHA_CRITICAL_DEGREES 16.0f
#define AUTOPILOT_MODE_ALT_HOLD 1
#define AUTOPILOT_MODE_GLIDESLOPE 2

// --- Simulated Hardware Inputs (Volatile) ---
volatile float current_pitch_deg;
volatile float target_pitch_deg;
volatile float current_alpha_deg;
volatile int autopilot_mode; // (CRV candidate)
volatile bool flight_director_active;

// --- Function Prototypes ---
void read_flight_sensors();
void log_control_output(const char* reason, int deflection);
bool is_stall_imminent();
int calculate_autopilot_deflection();
int apply_safety_saturation(int deflection);
int step(int last_elevator_deflection);

/**
 * @brief Simulates reading data from various flight sensors.
 */
void read_flight_sensors() {
    // In a real system, this would read from ADC/bus
    // Simulate sensor readings with random values within realistic ranges
    current_pitch_deg = ((float)(rand() % 2001) / 100.0f) - 10.0f;      // -10.0 to +10.0 deg
    target_pitch_deg = ((float)(rand() % 2001) / 100.0f) - 10.0f;       // -10.0 to +10.0 deg
    current_alpha_deg = ((float)(rand() % 2001) / 100.0f);              // 0.0 to 20.0 deg
    autopilot_mode = (rand() % 3);                                      // 0, 1, or 2
    flight_director_active = (rand() % 2) ? true : false;               // true or false
}

/**
 * @brief Logs the output for simulation purposes.
 */
void log_control_output(const char* reason, int deflection) {
    printf("Control Logic: %-25s | Elevator Deflection: %d\n", reason, deflection);
}

/**
 * @brief Checks for critical stall condition.
 * @return True if Angle of Attack is critical, false otherwise.
 */
bool is_stall_imminent() {
    return current_alpha_deg >= ALPHA_CRITICAL_DEGREES;
}

/**
 * @brief Calculates elevator deflection based on active autopilot mode.
 * This function's logic is only executed if the primary safety check passes.
 * @return The calculated deflection based on pitch error and mode.
 */
int calculate_autopilot_deflection() {
    if (!flight_director_active) {
        log_control_output("Autopilot Off", 0);
        return 0;
    }

    float pitch_error = target_pitch_deg - current_pitch_deg;
    int deflection;

    switch(autopilot_mode) {
        case AUTOPILOT_MODE_ALT_HOLD:
            deflection = (int)(pitch_error * 5.0f);
            log_control_output("Alt Hold Mode", deflection);
            break;
        case AUTOPILOT_MODE_GLIDESLOPE:
            deflection = (int)(pitch_error * 8.0f);
            log_control_output("Glideslope Mode", deflection);
            break;
        default:
            deflection = 0;
            log_control_output("Unknown Mode Fallback", deflection);
            break;
    }
    return deflection;
}

/**
 * @brief Clamps the deflection value to within physical limits.
 * @param deflection The calculated deflection.
 * @return The saturated deflection value.
 */
int apply_safety_saturation(int deflection) {
    if (deflection > ELEVATOR_MAX_UP_DEFLECTION) return ELEVATOR_MAX_UP_DEFLECTION;
    if (deflection < ELEVATOR_MAX_DOWN_DEFLECTION) return ELEVATOR_MAX_DOWN_DEFLECTION;
    return deflection;
}

/**
 * @brief Main control step function orchestrating calls.
 * @param last_elevator_deflection The previous output value.
 * @return The new calculated and saturated elevator deflection.
 */
int step(int last_elevator_deflection) {
    int new_deflection;

    // Priority 1: Critical Safety Logic
    if (is_stall_imminent()) {
        new_deflection = ELEVATOR_MAX_DOWN_DEFLECTION;
        log_control_output("ALPHA PROTECTION ACTIVE", new_deflection);
    } 
    // Priority 2: Standard Operational Logic
    else {
        new_deflection = calculate_autopilot_deflection();
    }

    // Final saturation is always applied
    return apply_safety_saturation(new_deflection);
}

/**
 * @brief Main execution loop.
 */
int main() {
    int elevator_deflection = 0;
    printf("--- Aircraft Pitch Control Simulation (Multi-Function) ---\n");

    for(int i = 0; i < 200; ++i) {
        read_flight_sensors();
        elevator_deflection = step(elevator_deflection);
        
        if (elevator_deflection < ELEVATOR_MAX_DOWN_DEFLECTION || elevator_deflection > ELEVATOR_MAX_UP_DEFLECTION) {
            printf("!!! SAFETY VIOLATION !!!\n");
            return -1;
        }
        if (i == 1) current_alpha_deg = 12.0f; 
    }

    return 0;
}

