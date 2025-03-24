#include <stdio.h>

// Define the PID structure
struct PID
{
    float Kp;              // Proportional gain constant
    float Ki;              // Integral gain constant
    float Kd;              // Derivative gain constant
    float Kaw;             // Anti-windup gain constant
    float T_C;             // Time constant for derivative filtering
    float T;               // Time step
    float max;             // Max command
    float min;             // Min command
    float max_rate;        // Max rate of change of the command
    float integral;        // Integral term
    float err_prev;        // Previous error
    float deriv_prev;      // Previous derivative
    float command_sat_prev;// Previous saturated command
    float command_prev;    // Previous command
};

// PID step function
float PID_Step(struct PID *pid, float measurement, float setpoint)
{
    // Calculate error
    float error = setpoint - measurement;

    // Proportional term
    float P = pid->Kp * error;

    // Integral term with anti-windup
    pid->integral += pid->Ki * pid->T * (error + pid->Kaw * (pid->command_sat_prev - pid->command_prev));

    // Derivative term with filtering
    float deriv = (error - pid->err_prev) / pid->T;
    float D = pid->Kd * (pid->T_C * deriv + (1 - pid->T_C) * pid->deriv_prev);

    // PID output before saturation
    float command = P + pid->integral + D;

    // Saturate command
    float command_sat = command;
    if (command > pid->max)
        command_sat = pid->max;
    else if (command < pid->min)
        command_sat = pid->min;

    // Rate limiting
    float rate = (command_sat - pid->command_sat_prev) / pid->T;
    if (rate > pid->max_rate)
        command_sat = pid->command_sat_prev + pid->max_rate * pid->T;
    else if (rate < -pid->max_rate)
        command_sat = pid->command_sat_prev - pid->max_rate * pid->T;

    // Update previous values
    pid->err_prev = error;
    pid->deriv_prev = deriv;
    pid->command_prev = command;
    pid->command_sat_prev = command_sat;

    return command_sat;
}

// Define the Object structure (the system to be controlled)
struct Object
{
    float mass;            // Mass of the object
    float damping;         // Damping coefficient
    float position;        // Position of the object
    float velocity;        // Velocity of the object
    float force;           // Applied force
    float max_force;       // Maximum force that can be applied
    float min_force;       // Minimum force that can be applied
    float T;               // Time step
};

// Object step function
void Object_Step(struct Object *obj)
{
    // Saturate the applied force
    if (obj->force > obj->max_force)
        obj->force = obj->max_force;
    else if (obj->force < obj->min_force)
        obj->force = obj->min_force;

    // Calculate acceleration
    float acceleration = (obj->force - obj->damping * obj->velocity) / obj->mass;

    // Update velocity and position
    obj->velocity += acceleration * obj->T;
    obj->position += obj->velocity * obj->T;
}

int main()
{
    // Initialize PID controller
    struct PID pid = {
        .Kp = 1.0,
        .Ki = 0.1,
        .Kd = 0.01,
        .Kaw = 0.5,
        .T_C = 0.01,
        .T = 0.1,
        .max = 100.0,
        .min = -100.0,
        .max_rate = 10.0,
        .integral = 0.0,
        .err_prev = 0.0,
        .deriv_prev = 0.0,
        .command_sat_prev = 0.0,
        .command_prev = 0.0
    };

    // Initialize Object
    struct Object obj = {
        .mass = 10.0,
        .damping = 0.5,
        .position = 0.0,
        .velocity = 0.0,
        .force = 0.0,
        .max_force = 100.0,
        .min_force = -100.0,
        .T = 0.1
    };

    // Simulation parameters
    float setpoint = 10.0; // Desired position
    int steps = 100;       // Number of simulation steps

    // Simulation loop
    for (int i = 0; i < steps; i++)
    {
        // Compute control force using PID
        obj.force = PID_Step(&pid, obj.position, setpoint);

        // Update object state
        Object_Step(&obj);

        // Print current state
        printf("Step %d: Position = %.2f, Velocity = %.2f, Force = %.2f\n",
               i, obj.position, obj.velocity, obj.force);
    }

    return 0;
}

