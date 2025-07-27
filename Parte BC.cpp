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
const unsigned long retardo = 200;

bool estadoSubeAnterior = HIGH;
bool estadoBajaAnterior = HIGH;
bool estadoModoAnterior = LOW;  // Por ser pull-down, inicia en LOW

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

  // --- BOTÓN SUBIR ---
  bool estadoSube = digitalRead(botonSube);
  if (estadoSubeAnterior == HIGH && estadoSube == LOW && ahora - ultimoSube > retardo) {
    contador++;
    if (modo == 0 && contador > 3) contador = 0;
    if (modo == 1 && contador > 15) contador = 0;
    mostrarLED(contador);
    ultimoSube = ahora;
  }
  estadoSubeAnterior = estadoSube;

  // --- BOTÓN BAJAR ---
  bool estadoBaja = digitalRead(botonBaja);
  if (estadoBajaAnterior == HIGH && estadoBaja == LOW && ahora - ultimoBaja > retardo) {
    if (contador == 0) {
      contador = (modo == 0) ? 3 : 15;
    } else {
      contador--;
    }
    mostrarLED(contador);
    ultimoBaja = ahora;
  }
  estadoBajaAnterior = estadoBaja;

  // --- BOTÓN MODO (PULL-DOWN: detectamos flanco ASCENDENTE)
  bool estadoModo = digitalRead(botonModo);
  if (estadoModoAnterior == LOW && estadoModo == HIGH && ahora - ultimoModo > retardo) {
    modo = !modo;
    contador = 0;
    mostrarLED(contador);
    ultimoModo = ahora;
  }
  estadoModoAnterior = estadoModo;
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
