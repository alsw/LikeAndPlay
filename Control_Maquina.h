//Estructura que guarda los ejes de control
struct EJES_CONTROL {
  int x;
  int y;
  int z;
};

//Estructura con la agrupacion de todos los controles para todos los origenes
struct CONTROL_MAQUINA {
  EJES_CONTROL js;  //Joystick local
  EJES_CONTROL bt;  //Modulo bluetooth (aplicacion Android u otros)
  EJES_CONTROL sb;  //Spacebrew (via internet)
};

//Instancia de la agrupacion de controles
extern CONTROL_MAQUINA control;

//Funcion de control de la maquina
void controlarMaquina();
