// Daniela López
// 22/07/2025

#include <Arduino.h>
#include <stdint.h>

// Definiciones de pines
#define ledAzul     18
#define ledVerde    21
#define ledRojo     22
#define ledAmarillo 23

#define botonSube 15   // Botón para incrementar (pull-up)
#define botonBaja 4    // Botón para decrementar (pull-up)

// Prototipos
void apagarTodosLEDs(void);
void mostrarLED(uint8_t valor);

// Variables
uint8_t contador = 0;
unsigned long ultimoSube = 0;
unsigned long ultimoBaja = 0;
const unsigned long retardo = 200;

bool estadoSubeAnterior = HIGH;
bool estadoBajaAnterior = HIGH;

void setup() {
  // Configurar LEDs como salida
  pinMode(ledAzul, OUTPUT); 
  pinMode(ledVerde, OUTPUT); 
  pinMode(ledRojo, OUTPUT);
  pinMode(ledAmarillo, OUTPUT); 

  // Configurar botones ambos son pull-up
  pinMode(botonSube, INPUT_PULLUP); 
  pinMode(botonBaja, INPUT_PULLUP); 

  apagarTodosLEDs();
  mostrarLED(contador);
}

void loop() {
  // Leer estado de los botones
  bool estadoSubeActual = digitalRead(botonSube);
  bool estadoBajaActual = digitalRead(botonBaja);
  unsigned long ahora = millis();

  //Botón para INCREMENTAR (sube)
  if (estadoSubeAnterior == HIGH && estadoSubeActual == LOW && ahora - ultimoSube > retardo) {
    contador++;
    if (contador > 3) contador = 0;
    mostrarLED(contador);
    ultimoSube = ahora;
  }

  //Botón para DECREMENTAR (baja)
  if (estadoBajaAnterior == HIGH && estadoBajaActual == LOW && ahora - ultimoBaja > retardo) {
    if (contador == 0)
      contador = 3;
    else
      contador--;
    mostrarLED(contador);
    ultimoBaja = ahora;
  }

  // Guardar estados para detectar cambios en la siguiente vuelta
  estadoSubeAnterior = estadoSubeActual;
  estadoBajaAnterior = estadoBajaActual;
}

// Apagar todos los LEDs
void apagarTodosLEDs(void) {
  digitalWrite(ledAzul, LOW);
  digitalWrite(ledVerde, LOW);
  digitalWrite(ledRojo, LOW);
  digitalWrite(ledAmarillo, LOW);
}

//Enciendo leds correspondiente al contador
void mostrarLED(uint8_t valor) {
  apagarTodosLEDs();
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
}