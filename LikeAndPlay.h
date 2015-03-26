#include <Arduino.h>

//Pines de las entradas analogicas de los joystick
const int pinJoyX = A1;
const int pinJoyY = A0;
const int pinJoyZ = A2;

//Pines de PWM hacia los motores
const int pinMXA = 5;
const int pinMXB = 3;
const int pinMYA = 6;
const int pinMYB = 10;
const int pinMZA = 11;
const int pinMZB = 9;

//Pines de I/O a los sensores de fin de carrera
const int pinFCXL = 12;
const int pinFCXR = 13;
const int pinFCYD = 7;
const int pinFCYA = 8;

//Pin de control de modo del bluetooth
const int pinBtKey = A5;
