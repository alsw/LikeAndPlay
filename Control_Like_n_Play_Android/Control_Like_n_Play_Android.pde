import ketai.ui.*;
import ketai.net.bluetooth.*;
import ketai.sensors.*;

KetaiBluetooth Kbt;
KetaiSensor Ksen;
ArrayList listaDisp;
String nombreDisp;
//Valores de control, de -255 a +255
int controlX, controlY, controlZ;
int tAnt;                 //Tiempo de la ultima actualizacion en ms

Boton botYp;
Boton botYn;
Boton botXp;
Boton botXn;
Boton botZp;
Boton botZn;
Boton botCon;

void setup() {
  //Se asegura de que no se pueda girar la pantalla y la abarca a
  //totalidad
  orientation(LANDSCAPE);
  size(displayWidth, displayHeight);

  //Predispone la altura del texto dentro de los botones
  //asi como su alineacion
  textSize(height*0.05);
  textAlign(CENTER, CENTER);

  //Inicializa las clases de bluetooth y sensores
  Kbt = new KetaiBluetooth(this);
  Ksen = new KetaiSensor(this);
  Ksen.start();

  //Obtiene la lista de dispositivos bluetooth emparejados
  listaDisp = Kbt.getPairedDeviceNames();

  //Inicializa los botones de la interfase de usuario
  botXn = new Boton("Izquierda",
                    int(width*0.3), int(height*0.375),
                    int(width*0.2), int(height*0.25));
  botXp = new Boton("Derecha",
                    int(width*0.5), botXn.y,
                    botXn.w, botXn.h);

  botYp = new Boton("Adelante",
                    int(width*0.1), int(height*0.25),
                    int(width*0.2), int(height*0.25));
  botYn = new Boton("Atras",
                    botYp.x, int(height*0.5),
                    botYp.w, botYp.h);

  botZp = new Boton("Subir",
                    int(width*0.7), botYp.y,
                    botXp.w, botXp.h);
  botZn = new Boton("Bajar",
                    botZp.x, botYn.y,
                    botXp.w, botXp.h);

  botCon = new Boton("Conexion",
                     int(width*0.4), int(height*0.8),
                     int(width*0.2), int(height*0.2));
}

void draw() {
  String Buffer;

  //Actualiza el tiempo actual
  int tAct = millis();

  dibujarPantalla();

  //Si existe un dispositivo seleccionado, realiza la transmision
  //de las coordenadas X y Y del cursor en intervalos periodicos
  //(20 veces por segundo)
  if (tAct - tAnt > 50 && nombreDisp != null) {
    if (botXp.presionado())      controlX = 255;
    else if (botXn.presionado()) controlX = -255;
    else                         controlX = 0;

    if (botYp.presionado())      controlY = 255;
    else if (botYn.presionado()) controlY = -255;
    else                         controlY = 0;

    if (botZp.presionado())      controlZ = 255;
    else if (botZn.presionado()) controlZ = -255;
    else                         controlZ = 0;

    Buffer = String.format("X%d\n", controlX);
    Kbt.writeToDeviceName(nombreDisp, Buffer.getBytes());
    Buffer = String.format("Y%d\n", controlY);
    Kbt.writeToDeviceName(nombreDisp, Buffer.getBytes());
    Buffer = String.format("Z%d\n", controlZ);
    Kbt.writeToDeviceName(nombreDisp, Buffer.getBytes());
    tAnt = tAct;
  }
}

void mousePressed() {
  if (botCon.mouseDentro()) {
    //Crea una nueva lista de seleccion en pantalla para que el
    //usuario elija un dispositivo bluetooth
    KetaiList klist = new KetaiList(this , listaDisp);
  }
}

void onKetaiListSelection(KetaiList klist) {
  //Obtiene la cadena recien seleccionada
  nombreDisp = klist.getSelection();

  //Efectua la conexion bluetooth al dispositivo seleccionado
  Kbt.connectToDeviceByName(nombreDisp);

  //Desaloja la lista, ya no es necesaria
  klist = null;
}

void dibujarPantalla() {
  //Limpia el fondo con color negro
  background(0);

  //Dibuja todos los botones
  botXp.dibujar();
  botXn.dibujar();
  botYp.dibujar();
  botYn.dibujar();
  botZp.dibujar();
  botZn.dibujar();
  botCon.dibujar();
}
