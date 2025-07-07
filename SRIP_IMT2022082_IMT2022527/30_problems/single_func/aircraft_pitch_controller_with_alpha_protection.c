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
volatile float current_pitch_deg;      // Current aircraft pitch in degrees
volatile float target_pitch_deg;       // Target pitch from flight computer
volatile float current_alpha_deg;      // Current Angle of Attack
volatile int autopilot_mode;           // Current active autopilot mode (CRV candidate)
volatile bool flight_director_active;  // Is the autopilot engaged
volatile int autopilot_mode1;           // Current active autopilot
// --- Function Prototypes ---
void read_flight_sensors();
void log_control_output(const char* reason, int deflection);
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
 * @brief Main control step function combining all logic.
 * @param last_elevator_deflection The previous output value.
 * @return The new calculated elevator deflection.
 */
int step(int last_elevator_deflection) {
    int new_deflection = 0;

    // 1. CRITICAL SAFETY OVERRIDE: Stall Protection (Alpha Protection)
    // This path makes 'autopilot_mode' and 'target_pitch_deg' irrelevant.
    if (current_alpha_deg >= ALPHA_CRITICAL_DEGREES) {
        // Force nose-down elevator deflection to reduce AoA and prevent stall.
        new_deflection = ELEVATOR_MAX_DOWN_DEFLECTION;
        if(autopilot_mode1){
            new_deflection = ELEVATOR_MAX_DOWN_DEFLECTION;
        }
        else{
            new_deflection = 0;
        }
        // log_control_output("ALPHA PROTECTION ACTIVE", new_deflection);

    } 
    // 2. STANDARD AUTOPILOT LOGIC
    else if (flight_director_active) {
        float pitch_error = target_pitch_deg - current_pitch_deg;
        
        // Logic depends on the conditionally relevant variable 'autopilot_mode'.
        if (autopilot_mode == AUTOPILOT_MODE_ALT_HOLD) {
            autopilot_mode1 = AUTOPILOT_MODE_ALT_HOLD;
            // Simple proportional control for altitude hold.
            new_deflection = (int)(pitch_error * 5.0f);
            // log_control_output("Alt Hold Mode", new_deflection);
        } 
        else if (autopilot_mode == AUTOPILOT_MODE_GLIDESLOPE) {
            // More aggressive control for tracking a glideslope.
            new_deflection = (int)(pitch_error * 8.0f);
            // log_control_output("Glideslope Mode", new_deflection);
        } 
        else {
            // Unknown mode, default to safe state.
            new_deflection = 0;
            // log_control_output("Unknown Mode Fallback", new_deflection);
        }
    } 
    // 3. MANUAL FLIGHT OR AUTOPILOT OFF
    else {
        // Autopilot is off, no automatic deflection.
        new_deflection = 0;
        // log_control_output("Autopilot Off", new_deflection);
    }

    // 4. FINAL SAFETY CLAMPING (Saturation)
    // Ensures the final output is within physical limits.
    if (new_deflection > ELEVATOR_MAX_UP_DEFLECTION) {
        new_deflection = ELEVATOR_MAX_UP_DEFLECTION;
    }
    if (new_deflection < ELEVATOR_MAX_DOWN_DEFLECTION) {
        new_deflection = ELEVATOR_MAX_DOWN_DEFLECTION;
    }

    return new_deflection;
}

/**
 * @brief Main execution loop.
 */
int main() {
    int elevator_deflection = 0;
    printf("--- Aircraft Pitch Control Simulation ---\n");

    // Main control loop
    for(int i = 0; i < 200; ++i) { // Limited loop for simulation
        current_pitch_deg = ((float)(rand() % 2001) / 100.0f) - 10.0f;      // -10.0 to +10.0 deg
        target_pitch_deg = ((float)(rand() % 2001) / 100.0f) - 10.0f;       // -10.0 to +10.0 deg
        current_alpha_deg = ((float)(rand() % 2001) / 100.0f);              // 0.0 to 20.0 deg
        autopilot_mode = (rand() % 3);                                      // 0, 1, or 2
        flight_director_active = (rand() % 2) ? true : false;               // true or false
            elevator_deflection = step(elevator_deflection);
        
        // Safety property check could be asserted here in a formal model
        if (elevator_deflection < ELEVATOR_MAX_DOWN_DEFLECTION || elevator_deflection > ELEVATOR_MAX_UP_DEFLECTION) {
            printf("!!! SAFETY VIOLATION !!!\n");
            return -1; // Critical failure
        }

        // Simulate a change for the next iteration
        if (i == 1) current_alpha_deg = 12.0f; 
    }

    return 0;
}

