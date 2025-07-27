// Daniela López
// 22/07/2025

//Aquí inlcuyo la librería principal para usar pinMode, digitalWrite.
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
void mostrarLED(uint8_t valor); //Esta función enciende el Led correspondiende

// Variables
uint8_t contador = 0;
unsigned long ultimoSube = 0;
unsigned long ultimoBaja = 0;
const unsigned long retardo = 200;

bool subeAnt = HIGH;
bool bajaAnt = HIGH;

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
  bool subeAct = digitalRead(botonSube);
  bool bajaAct = digitalRead(botonBaja);
  unsigned long ahora = millis();

  //Botón para INCREMENTAR (sube)
  if (subeAnt == HIGH && subeAct == LOW && ahora - ultimoSube > retardo) {
    contador++;
    if (contador > 3) contador = 0; //Si pasa de 3 regresa a 0
    mostrarLED(contador); //Actualiza el Led
    ultimoSube = ahora;
  }

  //Botón para DECREMENTAR (baja)
  if (bajaAnt == HIGH && bajaAct == LOW && ahora - ultimoBaja > retardo) {
    if (contador == 0)
      contador = 3;
    else
      contador--;
    mostrarLED(contador);
    ultimoBaja = ahora;
  }

  // Guardar estados para detectar cambios en la siguiente vuelta
  subeAnt = subeAct;
  bajaAnt = bajaAct;
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
  apagarTodosLEDs(); //Apago todos para que no haya problema
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
