#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define KP 3
#define KI 1
#define KD 2

int p(int iterations, int initial_seed, bool *door_open, int *alarm_flag) {
    int set_point = 80;  // Now variable
    int error = 0, prev_error = 0, integral = 0, derivative = 0;
    int output = 0;
    int temp = 0;
    bool alarm = false;
    int seed = initial_seed;

    for (int count = 0; count < iterations; count++) {
	temp=(seed * 23 + 47) % 300;
        seed = temp;

        // Simulate door opening 3 steps before end
        if (count == iterations - 3) {
            *door_open = true;
            set_point = 50;  // Raise the target temp when door opens
            printf("Door opened! Raising set point to %d\n", set_point);
        }

        error = set_point - temp;
        integral += error;
        derivative = error - prev_error;

        output = KP * error + KI * integral + KD * derivative;
        prev_error = error;

        // Early return if output exceeds limits
        if ((output > 255 && !*door_open)) {
            return output;
        }

        // Lower bound enforcement
        if (output < 20) {
            output = 20;
            alarm = true;
        }
    }

    *alarm_flag = alarm;
    return output;
}

int main() {
    int output;
    int alarm_triggered = 0;
    bool *door_open = malloc(sizeof(bool));
    *door_open = false;

    output = p(15, 88, door_open, &alarm_triggered);

    bool p_output_valid = (output >= 20) &&
                         ((*door_open && output <= 150) ||
                          (!*door_open && output <= 255)); // safety condition

    printf("Final Output: %d\n", output);
    printf("Door Open: %s\n", *door_open ? "TRUE" : "FALSE");
    printf("Alarm Triggered: %s\n", alarm_triggered ? "TRUE" : "FALSE");
    printf("Output Valid: %s\n", p_output_valid ? "TRUE" : "FALSE");

    free(door_open);
    return 0;
}

