// wind_turbine_pitch_controller.c
// Controls the pitch of turbine blades to optimize power generation based on wind speed. An extreme grid frequency deviation triggers a safety override, pitching the blades to support the grid, not to maximize power. The wind speed, used for power optimization, is a CRV as it is ignored during a grid stability event.
// blade_pitch_rate_dps >= -5 && blade_pitch_rate_dps <= 5
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <stdlib.h> // For rand()

// --- Constants and Definitions ---
#define MAX_PITCH_RATE_DPS 5 // degrees per second
#define GRID_FREQ_LOW_HZ 49.5f
#define GRID_FREQ_HIGH_HZ 50.5f
#define PITCH_OPTIMAL_POWER 10.0f
#define PITCH_FREQ_SUPPORT 45.0f // Pitch to reduce power

// --- Simulated Hardware Inputs ---
volatile float grid_frequency_hz;
volatile float wind_speed_ms;
volatile float current_blade_pitch_deg;
volatile bool power_generation_mode_active; // (CRV candidate)

// --- Function Prototypes ---
void read_turbine_sensors();
void log_turbine_state(const char* reason, int rate);
int step(int last_pitch_rate);

/**
 * @brief Simulates reading data from grid and turbine sensors.
 */
void read_turbine_sensors() {
    // Simulate sensor readings with random values within realistic ranges
    grid_frequency_hz = 49.0f + ((float)rand() / RAND_MAX) * 2.0f; // [49.0, 51.0]
    wind_speed_ms = 5.0f + ((float)rand() / RAND_MAX) * 15.0f;     // [5.0, 20.0]
    current_blade_pitch_deg = 0.0f + ((float)rand() / RAND_MAX) * 90.0f; // [0.0, 90.0]
    power_generation_mode_active = (rand() % 2) == 0 ? true : false;
}

/**
 * @brief Logs the output for simulation purposes.
 */
void log_turbine_state(const char* reason, int rate) {
    printf("Logic: %-25s | Pitch Rate Cmd: %d dps\n", reason, rate);
}

/**
 * @brief Main turbine blade pitch control logic step.
 * @param last_pitch_rate The previous pitch rate command.
 * @return The new calculated blade pitch rate.
 */
int step(int last_pitch_rate) {
    int new_rate = 0;
    float target_pitch;

    // 1. CRITICAL SAFETY OVERRIDE: Grid Frequency Deviation
    // This path makes 'power_generation_mode_active' and 'wind_speed' irrelevant.
    if (grid_frequency_hz < GRID_FREQ_LOW_HZ || grid_frequency_hz > GRID_FREQ_HIGH_HZ) {
        target_pitch = PITCH_FREQ_SUPPORT;
        log_turbine_state("GRID STABILITY SUPPORT", 0);
    } 
    // 2. STANDARD OPERATIONAL LOGIC
    else {
        // Logic depends on the CRV 'power_generation_mode_active'
        if (power_generation_mode_active) {
            // Simplified: Assume a single optimal pitch for a given wind speed range
            target_pitch = PITCH_OPTIMAL_POWER;
            log_turbine_state("Optimal Power Generation", 0);
        } else {
            target_pitch = 90.0f; // Feathered position
            log_turbine_state("Generation Inactive", 0);
        }
    }

    // 3. P-Controller to move to target pitch
    float pitch_error = target_pitch - current_blade_pitch_deg;
    new_rate = (int)round(pitch_error * 0.5f); // Proportional control

    // 4. FINAL SAFETY SATURATION
    if (new_rate > MAX_PITCH_RATE_DPS) {
        new_rate = MAX_PITCH_RATE_DPS;
    }
    if (new_rate < -MAX_PITCH_RATE_DPS) {
        new_rate = -MAX_PITCH_RATE_DPS;
    }

    log_turbine_state("Rate Calculation", new_rate);
    return new_rate;
}

/**
 * @brief Main execution loop.
 */
int main() {
    int pitch_rate = 0;
    printf("--- Wind Turbine Pitch Control Simulation ---\n");

    // Main control loop
    for(int i = 0; i < 200; ++i) {
        read_turbine_sensors();
        pitch_rate = step(pitch_rate);
        
        // Safety property check
        if (pitch_rate < -MAX_PITCH_RATE_DPS || pitch_rate > MAX_PITCH_RATE_DPS) {
            printf("!!! SAFETY VIOLATION !!!\n");
            return -1;
        }
        
        // Simulate a change for the next iteration
        if (i == 1) grid_frequency_hz = 50.0f;
    }

    return 0;
}

