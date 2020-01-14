# SpeedSensor
A simple speed sensor made with arduino and laser toy. With this sesor, you can reed the speed of a moving object that passes the laser barrier. The arduino will calculate the speed of the object based on how much time needed for the moving object pass the laser (when it pass the laser, it will block the LDR from the laser) and the lenght of the pbject. For the lenght of the object, you can save the value inside the Arduino's EEPROM.

Components Needed :
- Arduino Uno or Mega
- LDR 
- Toy Laser
- 1 kilo ohm resistor
- some wires

For the schematic you only need to attach a D1Robot LCD Keypad (i have a repository discussing about how to use this module https://github.com/mich1342/D1RobotLCDKeypadShieldArduinoUno just visit and learn how to use it first if you want to make this sensor). For the laser, just make it always on and pointing to the center of the ldr. For the LDR, make a voltage divider circuit using the ldr and resistor.


