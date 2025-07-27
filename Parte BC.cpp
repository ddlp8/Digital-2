#include <Arduino.h>

// Pines de LEDs
#define ledAzul     18
#define ledVerde    21
#define ledRojo     22
#define ledAmarillo 23

// Pines de botones
#define botonSube   15
#define botonBaja   4
#define botonModo   5  // BOTÓN PULL-DOWN 

// Variables globales
uint8_t contador = 0;
uint8_t modo = 0; // 0 = decimal, 1 = binario

// Antirrebote
unsigned long ultimoSube = 0;
unsigned long ultimoBaja = 0;
unsigned long ultimoModo = 0;
const unsigned long retardo = 200; //Antirrebote

bool subeAnt = HIGH;
bool bajaAnt = HIGH;
bool modoAnt = LOW;  // Por ser pull-down, inicia en LOW

// Prototipos
void apagarTodosLEDs(void);
void mostrarLED(uint8_t valor);
void mostrarBinario(uint8_t valor);

void setup() {
  pinMode(ledAzul, OUTPUT);
  pinMode(ledVerde, OUTPUT);
  pinMode(ledRojo, OUTPUT);
  pinMode(ledAmarillo, OUTPUT);

  pinMode(botonSube, INPUT_PULLUP);   // pull-up interno
  pinMode(botonBaja, INPUT_PULLUP);   // pull-up interno
  pinMode(botonModo, INPUT);          // PULL-DOWN físico

  apagarTodosLEDs();
  mostrarLED(contador);
}

void loop() {
  unsigned long ahora = millis();

  // Cambio de HIGH a LOW
  bool subeAct = digitalRead(botonSube);
  if (subeAnt == HIGH && subeAct == LOW && ahora - ultimoSube > retardo) {
    contador++;
    if (modo == 0 && contador > 3) contador = 0;
    if (modo == 1 && contador > 15) contador = 0; //Si supera el límite se reinicia
    mostrarLED(contador);
    //Se actualiza el estado anterior para la proxima vuelta
    ultimoSube = ahora;
  }
  subeAnt = subeAct;

  //Para decrementar
  bool bajaAct = digitalRead(botonBaja);
  if (bajaAnt == HIGH && bajaAct == LOW && ahora - ultimoBaja > retardo) {
    if (contador == 0) {
      contador = (modo == 0) ? 3 : 15;
    } else {
      contador--;
    }
    mostrarLED(contador);
    ultimoBaja = ahora;
  }
  bajaAnt = bajaAct;

  // BOTÓN MODO (PULL-DOWN: detectamos flanco ASCENDENTE)
  bool modoAct = digitalRead(botonModo);
  if (modoAnt == LOW && modoAct == HIGH && ahora - ultimoModo > retardo) {
    modo = !modo; //Cmabia de modo decimal a binario
    contador = 0;
    mostrarLED(contador);
    ultimoModo = ahora;
  }
  modoAnt = modoAct;
}

// Apaga todos los LEDs
void apagarTodosLEDs(void) {
  digitalWrite(ledAzul, LOW);
  digitalWrite(ledVerde, LOW);
  digitalWrite(ledRojo, LOW);
  digitalWrite(ledAmarillo, LOW);
}

// Muestra LEDs según el modo
void mostrarLED(uint8_t valor) {
  apagarTodosLEDs();
  if (modo == 0) {
    switch (valor) {
    case 0:
      digitalWrite(ledAzul, HIGH);
      break;
    case 1:
      digitalWrite(ledVerde, HIGH);
      break;
    case 2:
      digitalWrite(ledRojo, HIGH);
      break;
    case 3:
      digitalWrite(ledAmarillo, HIGH);
      break;
    }
  } else {
    mostrarBinario(valor);
  }
}

// Modo binario con desplazamientos simples
//Si el bit está en 1, enciende el LED que corresponde
void mostrarBinario(uint8_t valor) {
  // Bit 0 --> LED Azul
  if ((valor >> 0) & 1) digitalWrite(ledAzul, HIGH);
  // Bit 1 --> LED Verde
  if ((valor >> 1) & 1) digitalWrite(ledVerde, HIGH);
  // Bit 2 --> LED Rojo
  if ((valor >> 2) & 1) digitalWrite(ledRojo, HIGH);
  // Bit 3 --> LED Amarillo
  if ((valor >> 3) & 1) digitalWrite(ledAmarillo, HIGH);
}
