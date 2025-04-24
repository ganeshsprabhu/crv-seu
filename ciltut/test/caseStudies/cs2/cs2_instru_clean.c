int control_temperature(int temperature , int fan_speed , int mode , int humidity_level )
{
  int humidity_effect ;
  {
  if (mode == 1) {
    temperature -= fan_speed;
  } else
  if (mode == 2) {
    temperature += fan_speed;
  }
  if (humidity_level >= 20) {
    if (humidity_level <= 80) {
      if (temperature < 200) {
        simulate_seu_main(& humidity_effect);
        humidity_effect = (humidity_level * 5) / 10;
        temperature += humidity_effect;
      }
    }
  }
  return (temperature);
}
}
extern int ( printf)() ;
int main(void)
{
  int temperature ;
  int fan_speed ;
  int mode ;
  int humidity_level ;
  int final_temperature ;
  int tmp ;
  {
  temperature = 250;
  fan_speed = 5;
  mode = 1;
  humidity_level = 60;
  tmp = control_temperature(temperature, fan_speed, mode, humidity_level);
  final_temperature = tmp;
  printf("Final temperature: %d.%d\302\260C\n", final_temperature / 10, final_temperature % 10);
  return (0);
}
}
