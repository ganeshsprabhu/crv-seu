#include <stdio.h>
#include <stdlib.h>

#define TIME_STEP 0.1
#define LENGTH 1000  // Simulation length
//FILE *file;

// Define the PID structure
typedef struct {
    float Kp, Ki, Kd, Kaw, T_C, T;
    float max, min, max_rate;
    float integral, err_prev, deriv_prev;
    float command_prev, command_sat_prev;
} PID;

// Define the Object structure
typedef struct {
    float m, k, F_max, F_min, T;
    float v, z;
} Object;

/* Combined Function for PID Control and Object Step */
float PID_Object_Step(PID *pid, Object *obj, float measurement, float setpoint)
{
    /* PID Control Calculation */
    float err = setpoint - measurement;
    pid->integral += pid->Ki * err * pid->T + pid->Kaw * (pid->command_sat_prev - pid->command_prev) * pid->T;

    float deriv_filt = (err - pid->err_prev + pid->T_C * pid->deriv_prev) / (pid->T + pid->T_C);
    pid->err_prev = err;
    pid->deriv_prev = deriv_filt;

    float command = pid->Kp * err + pid->integral + pid->Kd * deriv_filt;
    pid->command_prev = command;

    /* Saturate command */
    float command_sat;
    if (command > pid->max) command_sat = pid->max;
    else if (command < pid->min) command_sat = pid->min;
    else command_sat = command;

    /* Apply rate limiter */
    if (command_sat > pid->command_sat_prev + pid->max_rate * pid->T)
        command_sat = pid->command_sat_prev + pid->max_rate * pid->T;
    else if (command_sat < pid->command_sat_prev - pid->max_rate * pid->T)
        command_sat = pid->command_sat_prev - pid->max_rate * pid->T;

    pid->command_sat_prev = command_sat;

    /* Object Update (F = ma) */
    float F_sat;
    if (command_sat > obj->F_max) F_sat = obj->F_max;
    else if (command_sat < obj->F_min) F_sat = obj->F_min;
    else F_sat = command_sat;

    float dv_dt = (F_sat - obj->k * obj->v) / obj->m;
    obj->v += dv_dt * obj->T;
    obj->z += obj->v * obj->T;

    return obj->z;
}

int main()
{
    // Current simulation time
    float t = 0;
    int i = 0;

    // Setpoints and output
    float command1 = 0, command2 = 0;
    float stp1 = 100, stp2 = 50;
    float z1 = 0, z2 = 0;

    // PID Controller Parameters
    PID pid1 = {1, 0.1, 5, 0.1, 1, TIME_STEP, 100, -100, 40, 0, 0, 0, 0, 0};
    PID pid2 = {1.8, 0.3, 7, 0.3, 1, TIME_STEP, 100, -100, 40, 0, 0, 0, 0, 0};

    // Object Parameters
    Object obj1 = {10, 0.5, 100, -100, TIME_STEP, 0, 0};
    Object obj2 = {10, 0.5, 100, -100, TIME_STEP, 0, 0};

    // Open file for logging
    //file = fopen("data_PID_C.txt", "w");

    while (i < LENGTH)
    {
        /* Change setpoint at t = 60 seconds */
        if (t >= 60)
        {
            stp1 = 200;
            stp2 = 150;
        }

        // Execute PID and Object update using single function
        z1 = PID_Object_Step(&pid1, &obj1, z1, stp1);
        z2 = PID_Object_Step(&pid2, &obj2, z2, stp2);

        // Log data
        //fprintf(file, "%f %f %f %f %f %f %f\n", t, command1, z1, stp1, command2, z2, stp2);

        // Increment time and iteration counter
        t += TIME_STEP;
        i++;
    }

    //fclose(file);
    return 0;
}
