import spacebrew.*;

//Instancias de los botones graficos
Boton botYp;
Boton botYn;
Boton botXp;
Boton botXn;
Boton botZp;
Boton botZn;

//Variable que almacena el ultimo eje que fue modificado
char ultimoEje;

//Objeto de comunicacion con el servicio spacebrew
Spacebrew sb;

void setup() {
  //Inicializa la ventana del control
  size(400, 200);

  //Predispone la altura del texto dentro de los botones
  //asi como su alineacion
  textSize(height*0.05);
  textAlign(CENTER, CENTER);

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

  //Crea el objeto de spacebrew, luego agrega los publicistas y se conecta
  sb = new Spacebrew(this);
  sb.addPublish("Eje X", "range", 0);
  sb.addPublish("Eje Y", "range", 0);
  sb.addPublish("Eje Z", "range", 0);
  sb.connect("sandbox.spacebrew.cc", "Control Like & Play",
             "Control de la maquina de atrapar dulces en processing");
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
}

void mousePressed() {
  //Al detectarse un click dentro del area de cualquier boton, se envia
  //el mensaje correspondiente al mismo mediante spacebrew. Luego se
  //guarda cual fue el ultimo eje modificado para recordarlo cuando se
  //libere el boton
  if (botXp.mouseDentro()) { sb.send("Eje X",  255); ultimoEje = 'x'; }
  if (botXn.mouseDentro()) { sb.send("Eje X", -255); ultimoEje = 'x'; }
  if (botYp.mouseDentro()) { sb.send("Eje Y",  255); ultimoEje = 'y'; }
  if (botYn.mouseDentro()) { sb.send("Eje Y", -255); ultimoEje = 'y'; }
  if (botZp.mouseDentro()) { sb.send("Eje Z",  255); ultimoEje = 'z'; }
  if (botZn.mouseDentro()) { sb.send("Eje Z", -255); ultimoEje = 'z'; }
}

void mouseReleased() {
  //Al liberar el mouse, se envia el mensaje de spacebrew dependiendo de
  //cual fue el utimo eje modificado. El valor enviado es cero para
  //indicar que se apague el eje
  switch (ultimoEje) {
    case 'x': sb.send("Eje X", 0); break;
    case 'y': sb.send("Eje Y", 0); break;
    case 'z': sb.send("Eje Z", 0); break;
  }
}
class Boton {
  String etiqueta;
  int x, y, w, h;

  Boton(String n_etiqueta, int n_x, int n_y, int n_ancho, int n_alto) {
    //Copia los datos del constructor a la clase
    etiqueta = n_etiqueta;
    x = n_x;
    y = n_y;
    w = n_ancho;
    h = n_alto;
  }

  void dibujar() {
    //Dibuja el rectangulo que representa el boton
    stroke(128);
    //Cambia el color de relleno en caso que se presione el boton
    if (presionado()) fill(255);
    else fill(192);
    //Dibuja el rectangulo
    rect(x, y, w, h);

    //Dibuja la etiqueta del boton en color negro
    fill(0);
    text(etiqueta, x, y, w, h);
  }

  boolean presionado() {
    //Devuelve verdadero en caso que se haga un click dentro del
    //area del boton
    return mousePressed && mouseX >= x && mouseX < x+w
                        && mouseY >= y && mouseY < y+h;
  }

  boolean mouseDentro() {
    //Devuelve verdadero en caso que el cursor del mouse este
    //dentro del area del boton
    return mouseX >= x && mouseX < x+w &&
           mouseY >= y && mouseY < y+h;
  }
};


