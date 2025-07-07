// automated_welding_robot_controller.c
// Controls the movement of a welding robot arm. The robot normally follows a pre-programmed path defined by a series of coordinates (a CRV). A proximity sensor that detects a human entering the safety cage provides a critical override, immediately halting all motion, irrespective of the robot's position in its programmed path.
// motor_command >= 0 && motor_command <= 1
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#define MOTOR_CMD_STOP 0
#define MOTOR_CMD_MOVE 1

// Simulated hardware inputs
volatile bool human_in_safety_zone;
volatile int program_path_step; // (CRV candidate)
volatile int total_path_steps = 100;

void read_robot_sensors() {
    human_in_safety_zone = (rand() % 20 == 0);
}

void log_robot_command(const char* reason, int command) {
    printf("Reason: %-25s | Motor Command: %s\n", reason, command == MOTOR_CMD_MOVE ? "MOVE" : "STOP");
}

bool is_human_detected() {
    return human_in_safety_zone;
}

int calculate_normal_command() {
    if (program_path_step < total_path_steps) {
        log_robot_command("Executing Weld Path", MOTOR_CMD_MOVE);
        program_path_step++;
        return MOTOR_CMD_MOVE;
    } else {
        log_robot_command("Path Complete", MOTOR_CMD_STOP);
        return MOTOR_CMD_STOP;
    }
}

int step_control_logic() {
    if (is_human_detected()) {
        log_robot_command("HUMAN IN ZONE - E-STOP", MOTOR_CMD_STOP);
        return MOTOR_CMD_STOP;
    } else {
        return calculate_normal_command();
    }
}

int main() {
    srand(time(NULL));
    int motor_command = MOTOR_CMD_STOP;
    program_path_step = 0;
    printf("--- Welding Robot Control Simulation (Multi Function) ---\n");

    while (program_path_step < total_path_steps) {
        read_robot_sensors();
        motor_command = step_control_logic();
        
        if (motor_command < 0 || motor_command > 1) {
            printf("!!! SAFETY VIOLATION: Invalid motor command !!!\n");
            return -1;
        }

        if (motor_command == MOTOR_CMD_STOP && human_in_safety_zone) {
            printf("E-Stop triggered. Halting simulation.\n");
            break;
        }
    }

    return 0;
}

