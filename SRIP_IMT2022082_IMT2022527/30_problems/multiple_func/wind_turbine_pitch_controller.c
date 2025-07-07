// wind_turbine_pitch_controller.c
// Controls the pitch of turbine blades to optimize power generation based on wind speed. An extreme grid frequency deviation triggers a safety override, pitching the blades to support the grid, not to maximize power. The wind speed, used for power optimization, is a CRV as it is ignored during a grid stability event.
// blade_pitch_rate_dps >= -5 && blade_pitch_rate_dps <= 5
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <stdlib.h> // For rand()

// --- Constants and Definitions ---
#define MAX_PITCH_RATE_DPS 5
#define GRID_FREQ_LOW_HZ 49.5f
#define GRID_FREQ_HIGH_HZ 50.5f
#define PITCH_OPTIMAL_POWER 10.0f
#define PITCH_FREQ_SUPPORT 45.0f

// --- Simulated Hardware Inputs ---
volatile float grid_frequency_hz;
volatile float wind_speed_ms;
volatile float current_blade_pitch_deg;
volatile bool power_generation_mode_active; // (CRV candidate)

// --- Function Prototypes ---
void read_turbine_sensors();
void log_turbine_state(const char* reason, int rate);
bool is_grid_frequency_unstable();
float get_target_pitch();
int calculate_pitch_rate(float target_pitch);
int apply_rate_saturation(int rate);
int step(int last_pitch_rate);

/**
 * @brief Simulates reading data from sensors.
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
 * @brief Checks for a critical grid frequency deviation.
 */
bool is_grid_frequency_unstable() {
    return (grid_frequency_hz < GRID_FREQ_LOW_HZ || grid_frequency_hz > GRID_FREQ_HIGH_HZ);
}

/**
 * @brief Determines the target blade pitch based on operating conditions.
 */
float get_target_pitch() {
    if (is_grid_frequency_unstable()) {
        log_turbine_state("GRID STABILITY SUPPORT", 0);
        return PITCH_FREQ_SUPPORT;
    }
    if (power_generation_mode_active) {
        log_turbine_state("Optimal Power Generation", 0);
        return PITCH_OPTIMAL_POWER;
    } else {
        log_turbine_state("Generation Inactive", 0);
        return 90.0f; // Feathered
    }
}

/**
 * @brief Calculates the required rate to move to the target pitch.
 */
int calculate_pitch_rate(float target_pitch) {
    float pitch_error = target_pitch - current_blade_pitch_deg;
    return (int)round(pitch_error * 0.5f);
}

/**
 * @brief Clamps the pitch rate to its physical limits.
 */
int apply_rate_saturation(int rate) {
    if (rate > MAX_PITCH_RATE_DPS) return MAX_PITCH_RATE_DPS;
    if (rate < -MAX_PITCH_RATE_DPS) return -MAX_PITCH_RATE_DPS;
    return rate;
}

/**
 * @brief Main turbine pitch control step.
 */
int step(int last_pitch_rate) {
    float target_pitch = get_target_pitch();
    int new_rate = calculate_pitch_rate(target_pitch);
    int saturated_rate = apply_rate_saturation(new_rate);
    log_turbine_state("Rate Calculation", saturated_rate);
    return saturated_rate;
}

/**
 * @brief Main execution loop.
 */
int main() {
    int pitch_rate = 0;
    printf("--- Wind Turbine Pitch Control Simulation (Multi-Function) ---\n");

    for(int i = 0; i < 200; ++i) {
        read_turbine_sensors();
        pitch_rate = step(pitch_rate);
        
        if (pitch_rate < -MAX_PITCH_RATE_DPS || pitch_rate > MAX_PITCH_RATE_DPS) {
            printf("!!! SAFETY VIOLATION !!!\n");
            return -1;
        }

        if (i == 1) grid_frequency_hz = 50.0f;
    }

    return 0;
}

