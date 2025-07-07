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
volatile int speed_profile; // 0: Precise, 1: Fast (CRV candidate)
volatile bool motion_enabled;

// --- Function Prototypes ---
void read_robot_sensors();
void log_robot_state(const char* reason, int torque);
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
 * @brief Main robot control logic step.
 * @param last_torque_command The previous torque command.
 * @return The new calculated motor torque command.
 */
int step(int last_torque_command) {
    int new_torque = 0;

    // 1. CRITICAL SAFETY OVERRIDE: Collision Detection
    // This path makes 'speed_profile' and positions irrelevant to the output.
    if (force_sensor_n > COLLISION_FORCE_NEWTONS) {
        new_torque = 0; // Immediately cut torque
        log_robot_state("COLLISION DETECTED", new_torque);
    } 
    // 2. STANDARD OPERATIONAL LOGIC
    else if (motion_enabled && (target_position != current_position)) {
        int target_torque;
        // Logic depends on the CRV 'speed_profile'
        if (speed_profile == 0) {
            target_torque = TORQUE_PROFILE_PRECISE;
            log_robot_state("Precise Profile", target_torque);
        } else {
            target_torque = TORQUE_PROFILE_FAST;
            log_robot_state("Fast Profile", target_torque);
        }

        // Move towards target position
        if (target_position > current_position) {
            new_torque = target_torque;
        } else {
            new_torque = -target_torque; // Note: simplified, assume torque can be negative
        }
    } 
    // 3. MOTION DISABLED OR AT TARGET
    else {
        new_torque = 0;
        log_robot_state("Motion Disabled/At Target", new_torque);
    }

    // 4. FINAL SAFETY SATURATION
    if (new_torque > MAX_MOTOR_TORQUE) {
        new_torque = MAX_MOTOR_TORQUE;
    }
    if (new_torque < 0) { // simplified for demo, real systems might have negative torque
        new_torque = 0;
    }

    return new_torque;
}

/**
 * @brief Main execution loop.
 */
int main() {
    int motor_torque = 0;
    printf("--- Robotic Arm Control Simulation ---\n");

    // Main control loop
    for(int i = 0; i < 200; ++i) {
        target_position = 900 + rand() % 201;        // 900 to 1100
        current_position = 400 + rand() % 201;
        speed_profile = rand() % 2; // Randomly select 0 (Precise) or 1 (Fast)
        motion_enabled = rand() % 2; // Randomly enable or disable motion
        motor_torque = step(motor_torque);
        
        // Safety property check
        if (motor_torque < 0 || motor_torque > MAX_MOTOR_TORQUE) {
            printf("!!! SAFETY VIOLATION !!!\n");
            return -1;
        }
        
        // Simulate a change for the next iteration
        if (i == 1) force_sensor_n = 10.0f;
    }

    return 0;
}

