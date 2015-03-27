#include "Control_Maquina.h"
#include "Pines.h"

CONTROL_MAQUINA control = {
  { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 },
};

void controlarMaquina() {
  int ejeX, ejeY, ejeZ;

  //Unifica los controles de los 3 origenes (joystick local, bluetooth y spacebrew) por medio
  //de superponerlos vectorialmente. Luego recorta los valores para confinarlos dentro
  //de los limites de -255 a 255
  ejeX = control.js.x + control.bt.x + control.sb.x;
  ejeX = constrain(ejeX, -255, 255);

  ejeY = control.js.y + control.bt.y + control.sb.y;
  ejeY = constrain(ejeY, -255, 255);

  ejeZ = control.js.z + control.bt.z + control.sb.z;
  ejeZ = constrain(ejeZ, -255, 255);

  //Controla el motor del eje X
  if (ejeX < 0) {
    //Si el eje tiene un control negativo, escribe cero sobre el pin A del motor y manda el
    //valor de PWM con el signo ajustado al pin B, pero solo si el fin de carrera no esta activado
    analogWrite(pinMXA, 0);
    if (!digitalRead(pinFCXL)) analogWrite(pinMXB, -ejeX);
    else                       analogWrite(pinMXB, 0);
  }
  else {
    //Si el eje tiene un control positivo, intercambia los roles de los pines A y B, siempre
    //cuidando que no se active el fin de carrera
    if (!digitalRead(pinFCXR)) analogWrite(pinMXA, ejeX);
    else                       analogWrite(pinMXA, 0);
    analogWrite(pinMXB, 0);
  }

  //Se actua de forma similar para el motor del eje Y
  if (ejeY < 0) {
    analogWrite(pinMYA, 0);
    if (!digitalRead(pinFCYD)) analogWrite(pinMYB, -ejeY);
    else                       analogWrite(pinMYB, 0);
  }
  else {
    if (!digitalRead(pinFCYA)) analogWrite(pinMYA, ejeY);
    else                       analogWrite(pinMYA, 0);
    analogWrite(pinMYB, 0);
  }

  //El motor del eje Z no tiene fin de carrera (tiene proteccion mecanica), asi que solo se
  //activan los pines A y B dependiendo del signo de la variable de control del eje
  if (ejeZ < 0) {
    analogWrite(pinMZA, 0);
    analogWrite(pinMZB, -ejeZ);
  }
  else {
    analogWrite(pinMZA, ejeZ);
    analogWrite(pinMZB, 0);
  }
}
