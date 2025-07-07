// robotic_arm_controller.c
// Controls the motor torque of a robotic arm. A force sensor detects collisions, providing a critical safety override that immediately cuts motor torque. The selected speed profile (e.g., 'fast' vs. 'precise'), which affects target torque, is a CRV as it is ignored during a collision.
// motor_torque_command >= 0 && motor_torque_command <= 100
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h> // For rand()

// --- Constants and Definitions ---
#define MAX_MOTOR_TORQUE 100
#define COLLISION_FORCE_NEWTONS 50
#define TORQUE_PROFILE_FAST 80
#define TORQUE_PROFILE_PRECISE 30

// --- Simulated Hardware Inputs ---
volatile int target_position;
volatile int current_position;
volatile float force_sensor_n;
volatile int speed_profile; // (CRV candidate)
volatile bool motion_enabled;

// --- Function Prototypes ---
void read_robot_sensors();
void log_robot_state(const char* reason, int torque);
bool is_collision_detected();
int calculate_nominal_torque();
int apply_torque_saturation(int torque);
int step(int last_torque_command);

/**
 * @brief Simulates reading data from robot sensors.
 */
void read_robot_sensors() {
    // Simulate random sensor values within reasonable ranges
    target_position = 900 + rand() % 201;        // 900 to 1100
    current_position = 400 + rand() % 201;
    speed_profile = rand() % 2; // Randomly select 0 (Precise) or 1 (Fast)
    motion_enabled = rand() % 2; // Randomly enable or disable motion
}

/**
 * @brief Logs the output for simulation purposes.
 */
void log_robot_state(const char* reason, int torque) {
    printf("Logic: %-25s | Motor Torque: %d\n", reason, torque);
}

/**
 * @brief Checks for a critical collision event.
 */
bool is_collision_detected() {
    return force_sensor_n > COLLISION_FORCE_NEWTONS;
}

/**
 * @brief Calculates motor torque under normal, non-collision conditions.
 */
int calculate_nominal_torque() {
    if (!motion_enabled || (target_position == current_position)) {
        log_robot_state("Motion Disabled/At Target", 0);
        return 0;
    }

    int target_torque = (speed_profile == 0) ? TORQUE_PROFILE_PRECISE : TORQUE_PROFILE_FAST;
    const char* reason = (speed_profile == 0) ? "Precise Profile" : "Fast Profile";
    log_robot_state(reason, target_torque);

    return (target_position > current_position) ? target_torque : -target_torque;
}

/**
 * @brief Clamps the torque command to its absolute physical limits.
 */
int apply_torque_saturation(int torque) {
    if (torque > MAX_MOTOR_TORQUE) return MAX_MOTOR_TORQUE;
    if (torque < 0) return 0; // Simplified for demo
    return torque;
}

/**
 * @brief Main robot control step.
 */
int step(int last_torque_command) {
    int new_torque;

    // Priority 1: Critical Safety Check
    if (is_collision_detected()) {
        new_torque = 0;
        log_robot_state("COLLISION DETECTED", new_torque);
    } 
    // Priority 2: Normal Operation
    else {
        new_torque = calculate_nominal_torque();
    }

    return apply_torque_saturation(new_torque);
}

/**
 * @brief Main execution loop.
 */
int main() {
    int motor_torque = 0;
    printf("--- Robotic Arm Control Simulation (Multi-Function) ---\n");

    for(int i = 0; i < 200; ++i) {
        read_robot_sensors();
        motor_torque = step(motor_torque);
        
        if (motor_torque < 0 || motor_torque > MAX_MOTOR_TORQUE) {
            printf("!!! SAFETY VIOLATION !!!\n");
            return -1;
        }

        if (i == 1) force_sensor_n = 10.0f;
    }

    return 0;
}

