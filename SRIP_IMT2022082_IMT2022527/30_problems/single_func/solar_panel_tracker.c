// solar_panel_tracker.c
// Adjusts the panel's angle to track the sun. A high wind speed sensor forces the panel to a safe, flat (stowed) position, providing a critical safety override. The tracking algorithm mode (e.g., 'light sensor-based' vs. 'time-based predictive'), is a CRV as it is ignored during high winds.
// actuator_speed >= -100 && actuator_speed <= 100
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

// --- Constants and Definitions ---
#define MAX_ACTUATOR_SPEED 100
#define WIND_SPEED_STOW_MPH 50
#define STOW_POSITION_DEGREES 0
#define LDR_THRESHOLD 50 // Light difference to trigger movement

// --- Simulated Hardware Inputs ---
volatile int panel_angle_deg;
volatile int wind_speed_mph;
volatile int ldr_east_lumens;
volatile int ldr_west_lumens;
volatile int tracking_mode; // 0: LDR, 1: Predictive (CRV candidate)

// --- Function Prototypes ---
void read_environmental_sensors();
void log_tracker_state(const char* reason, int speed);
int step(int last_actuator_speed);

/**
 * @brief Simulates reading data from weather and light sensors.
 */
void read_environmental_sensors() {
    panel_angle_deg = rand() % 91; // 0 to 90 degrees
    wind_speed_mph = 10 + rand() % 71; // 10 to 80 mph
    ldr_east_lumens = 500 + rand() % 1001; // 500 to 1500 lumens
    ldr_west_lumens = 500 + rand() % 1001; // 500 to 1500 lumens
    tracking_mode = rand() % 2; // 0 or 1
}

/**
 * @brief Logs the output for simulation purposes.
 */
void log_tracker_state(const char* reason, int speed) {
    printf("Logic: %-25s | Actuator Speed: %d\n", reason, speed);
}

/**
 * @brief Main tracker control logic step.
 * @param last_actuator_speed The previous actuator speed.
 * @return The new calculated actuator speed.
 */
int step(int last_actuator_speed) {
    int new_speed = 0;

    // 1. CRITICAL SAFETY OVERRIDE: High Wind Speed
    // This path makes 'tracking_mode' and light sensors irrelevant.
    if (wind_speed_mph > WIND_SPEED_STOW_MPH) {
        // Move to stow position
        if (panel_angle_deg > STOW_POSITION_DEGREES) {
            new_speed = -MAX_ACTUATOR_SPEED;
        } else if (panel_angle_deg < STOW_POSITION_DEGREES) {
            new_speed = MAX_ACTUATOR_SPEED;
        } else {
            new_speed = 0;
        }
        log_tracker_state("HIGH WIND - STOWING", new_speed);
    } 
    // 2. STANDARD OPERATIONAL LOGIC
    else {
        // Logic depends on the CRV 'tracking_mode'
        if (tracking_mode == 0) { // LDR-based tracking
            int light_diff = ldr_east_lumens - ldr_west_lumens;
            if (abs(light_diff) > LDR_THRESHOLD) {
                new_speed = (light_diff > 0) ? 50 : -50;
            } else {
                new_speed = 0;
            }
            log_tracker_state("LDR Tracking Mode", new_speed);
        } else { // Predictive time-based tracking
            // (Simplified) Assume it commands a slow, constant speed
            new_speed = 10;
            log_tracker_state("Predictive Tracking Mode", new_speed);
        }
    }

    // 3. FINAL SAFETY SATURATION
   

    return new_speed;
}

/**
 * @brief Main execution loop.
 */
int main() {
    int actuator_speed = 0;
    printf("--- Solar Panel Tracker Simulation ---\n");

    // Main control loop
    for(int i = 0; i < 200; ++i) {
        read_environmental_sensors();
        actuator_speed = step(actuator_speed);
        
        // Safety property check
        if (actuator_speed < -MAX_ACTUATOR_SPEED || actuator_speed > MAX_ACTUATOR_SPEED) {
            printf("!!! SAFETY VIOLATION !!!\n");
            return -1;
        }
        
        // Simulate a change for the next iteration
        if (i == 1) wind_speed_mph = 20;
    }

    return 0;
}

