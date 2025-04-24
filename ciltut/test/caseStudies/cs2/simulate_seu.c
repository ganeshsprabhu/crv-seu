#include <stdio.h>
#include <stdbool.h>
#include "simulate_seu.h"

// Function without SEU
int control_temperature(int temperature, int fan_speed, int mode, int humidity_level) {
    if (mode == 1) temperature -= fan_speed;
    else if (mode == 2) temperature += fan_speed;

    // Modify logic to reduce humidity impact
    if (humidity_level >= 20 && humidity_level <= 80 && temperature < 200) {
        int humidity_effect = (humidity_level * 5) / 10;
        temperature += humidity_effect;
    }

    return temperature;
}

// Function with SEU
int control_temperature_prime(int temperature, int fan_speed, int mode, int humidity_level) {
    if (mode == 1) temperature -= fan_speed;
    else if (mode == 2) temperature += fan_speed;

    // Introduce SEU before using humidity_level
     // simulate_seu_main(&humidity_level);  // Bit flip here
    if (humidity_level >= 20 && humidity_level <= 80 && temperature < 200) {
       // simulate_seu_main(&humidity_level);  // Bit flip here
        int humidity_effect = (humidity_level * 5) / 10;
        simulate_seu_main(&humidity_effect);  // Bit flip here
        temperature += humidity_effect;
    }

    return temperature;
}

int main() {
    int temperature;       // Initial temperature (scaled, e.g., 250 = 25.0°C)
    int fan_speed;         // Fan speed in same scale
    int mode;              // 1 = cool, 2 = heat
    int humidity_level;    // Bit flipped input

    int final_temperature = control_temperature(temperature, fan_speed, mode, humidity_level);
    printf("Final temperature: %d.%d°C\n", final_temperature / 10, final_temperature % 10);

    int final_temperature_prime = control_temperature_prime(temperature, fan_speed, mode, humidity_level);
    printf("Final temperature: %d.%d°C\n", final_temperature_prime / 10, final_temperature_prime % 10);

    // Safety check
    int phi = final_temperature <= 300;
    int phi_prime = final_temperature_prime <= 300;

    // If SEU flips do not impact result, it's not a CRV
    __CPROVER_assert(!(phi ^ phi_prime), "CRV Result for humidity_level => if SUCCESS, then it's not a CRV");

    return 0;
}
