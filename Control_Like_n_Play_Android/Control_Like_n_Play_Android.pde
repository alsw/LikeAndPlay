import ketai.ui.*;
import ketai.net.bluetooth.*;

//Instancias de los botones graficos
Boton botYp;
Boton botYn;
Boton botXp;
Boton botXn;
Boton botZp;
Boton botZn;
Boton botCon;

//Variable que almacena el ultimo eje que fue modificado
char ultimoEje;

//Objeto de comunicacion por bluetooth y variables asociadas
KetaiBluetooth Kbt;
ArrayList listaDisp;
String nombreDisp;

void setup() {
  //Se asegura de que no se pueda girar la pantalla y la abarca a
  //totalidad
  orientation(LANDSCAPE);
  size(displayWidth, displayHeight);

  //Predispone la altura del texto dentro de los botones
  //asi como su alineacion
  textSize(height*0.05);
  textAlign(CENTER, CENTER);

  //Inicializa la clase de bluetooth
  Kbt = new KetaiBluetooth(this);

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

void mousePressed() {
  String Buffer;

  if (botCon.mouseDentro()) {
    //Crea una nueva lista de seleccion en pantalla para que el
    //usuario elija un dispositivo bluetooth
    KetaiList klist = new KetaiList(this , listaDisp);
  }

  //Al detectarse un click dentro del area de cualquier boton, se envia
  //el mensaje correspondiente al mismo mediante bluetooth. Luego se
  //guarda cual fue el ultimo eje modificado para recordarlo cuando se
  //libere el boton
  if (botXp.mouseDentro()) {
    Buffer = "X255\n";
    Kbt.writeToDeviceName(nombreDisp, Buffer.getBytes());
    ultimoEje = 'x';
  }
  if (botXn.mouseDentro()) {
    Buffer = "X-255\n";
    Kbt.writeToDeviceName(nombreDisp, Buffer.getBytes());
    ultimoEje = 'x';
  }

  if (botYp.mouseDentro()) {
    Buffer = "Y255\n";
    Kbt.writeToDeviceName(nombreDisp, Buffer.getBytes());
    ultimoEje = 'y';
  }
  if (botYn.mouseDentro()) {
    Buffer = "Y-255\n";
    Kbt.writeToDeviceName(nombreDisp, Buffer.getBytes());
    ultimoEje = 'y';
  }

  if (botZp.mouseDentro()) {
    Buffer = "Z255\n";
    Kbt.writeToDeviceName(nombreDisp, Buffer.getBytes());
    ultimoEje = 'z';
  }
  if (botZn.mouseDentro()) {
    Buffer = "Z-255\n";
    Kbt.writeToDeviceName(nombreDisp, Buffer.getBytes());
    ultimoEje = 'z';
  }
}

void mouseReleased() {
  String Buffer;

  //Al liberar el mouse, se envia el mensaje al bluetooth dependiendo de
  //cual fue el utimo eje modificado. El valor enviado es cero para
  //indicar que se apague el eje
  switch (ultimoEje) {
    case 'x': Buffer = "X0\n"; break;
    case 'y': Buffer = "Y0\n"; break;
    default: Buffer = "Z0\n"; break;
  }

  Kbt.writeToDeviceName(nombreDisp, Buffer.getBytes());
}

void onKetaiListSelection(KetaiList klist) {
  //Obtiene la cadena recien seleccionada
  nombreDisp = klist.getSelection();

  //Efectua la conexion bluetooth al dispositivo seleccionado
  Kbt.connectToDeviceByName(nombreDisp);

  //Desaloja la lista, ya no es necesaria
  klist = null;
}
