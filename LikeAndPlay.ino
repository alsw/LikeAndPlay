#include  <Bridge.h>

int MX[] = {4, 6}; //Pines del motor en eje X
int MY[] = {7, 10}; //Pines del motor en eje Y
int MZ[] = {9, 11}; //Pines del motor en eje Z

int FCX[] = {7, 8}; //Pines de final de carrera izquierda, derecha
int FCY[] = {13, 12}; //Pines de final de carrera adelante, atras

void setup() {

  for (int i = 0; i < 2; i++) {
    pinMode(MX[i], OUTPUT);
    pinMode(MY[i], OUTPUT);
    pinMode(MZ[i], OUTPUT);
    pinMode(FCX[i], INPUT);
    pinMode(FCY[i], INPUT);
  }
  Serial.begin(9600);
  Serial.println("Iniciando Like&PLay");
}

void loop() {
  Serial.print("valor ");
  Serial.println(FinalCarreraX());
  delay(400);
}

int FinalCarreraX() {
  if (digitalRead(FCX[0]) == 1 )
    return 1;
  if (digitalRead(FCX[1]) == 1)
    return 2;
  return 0;
}

int FinalCarreraY() {
  if (digitalRead(FCY[0]) == 1 )
    return 1;
  if (digitalRead(FCY[1]) == 1)
    return 2;
  return 0;
}
