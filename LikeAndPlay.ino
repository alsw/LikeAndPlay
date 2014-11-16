#include  <Bridge.h>

int blue[] = {2, 4}; //Pines del Blue

int MX[] = {3, 5}; //Pines del motor en eje X
int MY[] = {6, 10}; //Pines del motor en eje Y
int MZ[] = {9, 11}; //Pines del motor en eje Z

int FCX[] = {7, 8}; //Pines de final de carrera izquierda, derecha
int FCY[] = {13, 12}; //Pines de final de carrera adelante, atras

int DX, DY, DZ; //Direcion del moviento en X,Y y Z
int DXp, DYp, DZp; //Ultima direcion de moviento
// 0 Detenido
// 1 Adelante
// 2 Atras


void setup() {
  for (int i = 0; i < 2; i++) {//Configurar pines
    pinMode(MX[i], OUTPUT);
    pinMode(MY[i], OUTPUT);
    pinMode(MZ[i], OUTPUT);
    pinMode(FCX[i], INPUT);
    pinMode(FCY[i], INPUT);
    digitalWrite(FCX[i], 1);
    digitalWrite(FCY[i], 1);
  }
  DX = 0;
  DY = 0;
  DZ = 0;
  DXp = 0;
  DYp = 0;
  DZp = 0;

  Serial.begin(9600);// activar puerto serial
  Serial.println("Iniciando Like&PLay");
}

void loop() {
  Leer();
  Mover();
  Mostar();
  delay(200);
}

void Mostar() {
  Serial.print("valor X ");
  Serial.print(FinalCarreraX());
  Serial.print(" Y ");
  Serial.println(FinalCarreraY());
  
}

void Leer() {
  while (Serial.available()) {
    char Eje = Serial.read();
    int Direcion = Serial.read() - 48;
    Serial.print("Recivido ");
    Serial.print(Eje);
    Serial.print(" ");
    Serial.println(Direcion);
    
    
  }

  if (FinalCarreraX() > 0) {
    DX = 0;
  }
  if (FinalCarreraY() > 0) {
    DY = 0;
  }
}

void Mover() {//Comprueva si hay cambios de direciones y los aplica
  if (DX != DXp) {
    DXp = DX;
    DMover(DX, MX);
  }
  if (DY != DYp) {
    DYp = DY;
    DMover(DY, MY);
  }
  if (DZ != DZp) {
    DZp = DZ;
    DMover(DZ, MZ);
  }
}

void DMover(int D, int Motor[]) { //mueve el motor segun estado
  switch (D) {
    case 0:
      Parar(Motor);
      break;
    case 1:
      Adelante(Motor);
      break;
    case 2:
      Reversa(Motor);
      break;
  }
}

void Parar(int Motor[]) { // detiene el motor que se envie
  digitalWrite(Motor[0], 0);
  digitalWrite(Motor[1], 0);
}

void Adelante(int Motor[]) { //Ponen en marcha el motor que se envie
  digitalWrite(Motor[0], 1);
  digitalWrite(Motor[1], 0);
}

void Reversa(int Motor[]) { // Pone en reversa el motor que se envie
  digitalWrite(Motor[0], 0);
  digitalWrite(Motor[1], 1);
}

int FinalCarreraX() {  // 0 para inactivo, 1 para activo a la izquierda, 2 activo a la derecha
  if (digitalRead(FCX[0]) == 1 )
    return 1;
  if (digitalRead(FCX[1]) == 1)
    return 2;
  return 0;
}

int FinalCarreraY() { // 0 para inactivo, 1 para activo a la adelante, 2 activo a la atras
  if (digitalRead(FCY[0]) == 1 )
    return 1;
  if (digitalRead(FCY[1]) == 1)
    return 2;
  return 0;
}
