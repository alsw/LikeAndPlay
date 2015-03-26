#include <Bridge.h>
#include <SpacebrewYun.h>

#include "LikeAndPlay.h"
#include "Control_Maquina.h"

//Variables de estado de la comunicacion serie bluetooth
bool esperandoTrama = true;  //Bandera que indica si se espera trama nueva
char ejeTrama = 0;    //Indica con 'x', 'y' o 'z' el eje que se esta recibiendo
char bufferSerie[5];  //Buffer de recepcion de datos (3 digitos, signo y cero terminador)
char posBuffer = 0;   //La posicion de escritura del buffer para los datos recibidos

//Instancia del objeto de comunicacion spacebrew
SpacebrewYun sb = SpacebrewYun("Like & Play", "Nuestra maquina de atrapar dulces!");

void setup() {
  //Hace escritura analogica de cero en todos los pines para
  //apagar los motores inicialmente
  analogWrite(pinMXA, 0);
  analogWrite(pinMXB, 0);
  analogWrite(pinMYA, 0);
  analogWrite(pinMYB, 0);
  analogWrite(pinMZA, 0);
  analogWrite(pinMZB, 0);

  //Coloca todos los pines de los sensores como entradas con
  //pull-up
  pinMode(pinFCXL, INPUT_PULLUP);
  pinMode(pinFCXR, INPUT_PULLUP);
  pinMode(pinFCYD, INPUT_PULLUP);
  pinMode(pinFCYA, INPUT_PULLUP);

  //Coloca el pin de control de bluetooth en modo de
  //comunicacion
  digitalWrite(pinBtKey, LOW);
  pinMode(pinBtKey, OUTPUT);

  //Inicializa el puerto serie de hardware conectado al
  //bluetooth
  Serial1.begin(9600);

  //Descomentar temporalmente para configurar modulo bluetooth
  //en modo AT (Nota: Se bloquea el sketch)
/*
  digitalWrite(pinBtKey, HIGH);
  Serial.begin(9600);
  while (!Serial);
  Serial.println("Modo AT activo");
  for (;;) {
    if (Serial1.available())
      Serial.write(Serial1.read());
    if (Serial.available())
      Serial1.write(Serial.read());
  }
*/

  //Inicializa la libreria bridge
  Bridge.begin();

  //Inicializa el objeto spacebrew agregando 3 suscriptores, luego agrega el callback para los
  //mensajes de tipo range y finalmente se conecta
  sb.addSubscribe("Eje X", "range");
  sb.addSubscribe("Eje Y", "range");
  sb.addSubscribe("Eje Z", "range");
  sb.onRangeMessage(handleRange);
  sb.connect("sandbox.spacebrew.cc"); 
}

void loop() {
  static unsigned long tAnt = 0;
  unsigned long tAct;

  //Primeramente se leen los joysticks cada 10ms
  tAct = millis();
  if (tAct - tAnt >= 10) {
    leerJoysticks();
    tAnt = tAct;
  }

  //A continuacion se procesan los datos serie que vienen del modulo bluetooth
  procesarTramaSerie();

  //Luego se monitorea la conexion con spacebrew
  sb.monitor();

  //En caso que la conexion con spacebrew se pierda, se desactivan los controles asociados
  if (!sb.connected() ) {
    control.sb.x = 0;
    control.sb.y = 0;
    control.sb.z = 0;
  }

  //Finalmente se controla la maquina en si, con la informacion recolectada de todos los
  //controles
  controlarMaquina();
}

void leerJoysticks() {
  //Se leen los controles de joystick y se umbralizan
  control.js.x = umbralizarJoystick(analogRead(pinJoyX));
  control.js.y = umbralizarJoystick(analogRead(pinJoyY));
  control.js.z = umbralizarJoystick(analogRead(pinJoyZ));
}

int umbralizarJoystick(int lectura) {
  //El proceso de umbralizacion toma una lectura de ADC entre 0 y 1023 y determina si el joystick
  //se activo hacia atras (valor de ADC bajo) devolviendo -255, o bien si esta centrado (valor
  //intermedio) devolviendo cero, o bien si se activo hacia adelante (valor de ADC alto)
  //retornando 255
  if (lectura < 341)
    return -255;
  else if (lectura > 682)
    return 255;
  else
    return 0;
}

void procesarTramaSerie() {
  char caracter;
  byte i;
  static bool limpiarBuffer = true;

  //Si no hay caracteres pendientes de procesar, retorna inmediatamente
  if (!Serial1.available()) return;

  //Si los hay, lee el siguiente
  caracter = Serial1.read();
  Serial.print(caracter);

  //Si la bandera de limpieza esta activa, procede a limpiar el buffer de entrada
  if (limpiarBuffer) {
    //Recorre todo el buffer escribiendo cada caracter con un cero terminador
    for (i=0; i<sizeof(bufferSerie); i++)
      bufferSerie[i] = 0;    

    posBuffer = 0;  //La posicion del buffer ahora apunta al principio

    limpiarBuffer = false;   //Limpia la bandera para no repetir el proceso
  }

  //El caracter se procesa dependiendo de si se espera una trama o no
  if (esperandoTrama) {
    //Si se espera trama nueva, se verifica que ingrese una letra X, Y o Z
    //Al recibir cualquiera de ellas exitosamente, se guarda el eje asociado a la trama y
    //se desmarca la bandera de espera
    switch (caracter) {
      case 'X':
      case 'x':
        ejeTrama='x';
        esperandoTrama = false;
        break;
      case 'Y':
      case 'y':
        ejeTrama='y';
        esperandoTrama = false;
        break;
      case 'Z':
      case 'z':
        ejeTrama='z';
        esperandoTrama = false;
        break;
    }
    //Nota: Si el caracter no es ninguno de los esperados entonces se descarta
  }
  else {
    //Si hay una trama en curso, se actua dependiendo del caracter recibido
    if (caracter == '\n') {
      //Si se recibe un fin de linea, se procesa la trama dependiendo del eje asociado
      switch(ejeTrama) {
        case 'x':
          //Se convierte el buffer a una variable tipo entero y se guarda en el eje de control
          //correspondiente, luego se recorta para evitar desbordamientos
          control.bt.x = atoi(bufferSerie);
          control.bt.x = constrain(control.bt.x, -255, 255);
          break;
        case 'y':
          control.bt.y = atoi(bufferSerie);
          control.bt.y = constrain(control.bt.y, -255, 255);
          break;
        case 'z':
          control.bt.z = atoi(bufferSerie);
          control.bt.z = constrain(control.bt.z, -255, 255);
          break;
      }

      //Una vez procesado el buffer, se limpiara el mismo a la proxima iteracion y se esperara
      //una nueva trama
      limpiarBuffer = true;
      esperandoTrama = true;
    }
    else if (((caracter >= '0' && caracter <= '9') || caracter == '-') &&
             posBuffer < sizeof(bufferSerie) - 1) {
      //Si se recibe un caraccter entre 0 y 9 o bien el signo negativo, y ademas hay espacio
      //en buffer, entonces se guarda ese caracter
      bufferSerie[posBuffer++] = caracter;
    }
    else {
      //Si el caracter no es ninguno de los esperados (fin de linea, digito de 0 a 9, o signo
      //negativo) o bien se agoto el espacio en buffer, se descarta la trama por ser invalida
      //y se limpia el buffer para esperar una nueva
      limpiarBuffer = true;
      esperandoTrama = true;
    }
  }
/*
  //Depuracion solamente, insertar Serial.begin(9600) en setup() si se usa
  Serial.print("X = ");
  Serial.print(control.bt.x);
  Serial.print(" Y = ");
  Serial.print(control.bt.y);
  Serial.print(" Z = ");
  Serial.println(control.bt.z);
*/
}

void handleRange (String route, int value) {
  if (route == "Eje X") control.sb.x = constrain(value, -255, 255);
  if (route == "Eje Y") control.sb.y = constrain(value, -255, 255);
  if (route == "Eje Z") control.sb.z = constrain(value, -255, 255);
}
