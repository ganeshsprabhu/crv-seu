#include <stdio.h>
#include <stdbool.h>

// Function to simulate a room temperature control system
int control_temperature(float temperature, int fan_speed, int mode, int humidity_level)
{
    bool alarm = false;

    // Temperature adjustment based on mode
    if (mode == 1)
    { // Cooling mode
        temperature -= fan_speed;
    }
    else if (mode == 2)
    { // Heating mode
        temperature += fan_speed;
    }
    else
    { // Invalid mode
        alarm = true;
        printf("Invalid mode! Setting alarm...\n");
    }

    // Limit the effect of humidity_level: only add a small, bounded value
    if (humidity_level >= 20 && humidity_level <= 80)
    {
        temperature += humidity_level * 0.05; // Small adjustment based on humidity
    }

    // Safety property: Temperature must not exceed 30 degrees
    if (temperature > 30)
    {
        alarm = true;
    }

    printf("Alarm: %s\n", alarm ? "ON" : "OFF");
    return temperature;
}

int main()
{
    float temperature = 25;  // Initial room temperature
    int fan_speed = 5;       // Fan speed setting
    int mode = 1;            // Mode: 1 for cooling, 2 for heating
    int humidity_level = 60; // Humidity level (affects comfort but not safety directly)

    int temperature = control_temperature(temperature, fan_speed, mode, humidity_level);
    printf("Final temperature: %d\n", temperature);
    return 0;
}