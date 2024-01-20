#include <Wire.h>
#include "RTClib.h"
#include <avr/wdt.h>

RTC_DS1307 rtc;
char hora[30];

// Hora da festa
int horaliga1 = 6;
int horaliga2 = 18;
int horaliga3 = 11;
int horaliga4 = 9;
int minlig = 40;

// Liga dosadores
int AL[] = {5, 6, 7}; // Pinos dos dosadores

// Define quantidade de tempo de acionamento dosadores
int tempo[] = {0, 690, 300}; // PEQUENO, MEDIO, GRANDE
int cont;

// Tempo millis
const long interval = 1000;
unsigned long previousMillis = 0;

void relogio() {
  unsigned long currentMillis = millis();
  DateTime now = rtc.now();
  sprintf(hora, "%02d:%02d:%02d", now.hour(), now.minute(), now.second());
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    Serial.println(hora);
  }
}

void doseracao(int dosadorPin, int dosadorTempo) {
  digitalWrite(dosadorPin, HIGH);
  unsigned long startMillis = millis();
  while (millis() - startMillis < dosadorTempo) {
    // Aguarde o tempo do dosador
  }
  digitalWrite(dosadorPin, LOW);
}

void executa() {
  DateTime now = rtc.now();
  // Trata todos
  if ((((now.hour() == horaliga1) || (now.hour() == horaliga2) || (now.hour() == horaliga3)) && (now.minute() == minlig) && (now.second() <= 2))) {
    doseracao(AL[1], tempo[1]); // Executa dosador médio
  }
  // Trata aquario
  if ((((now.hour() == horaliga4)) && (now.minute() == minlig) && (now.second() <= 2))) {
    doseracao(AL[0], tempo[1]); // Executa dosador pequeno
  }
}

void setup() {
  // Inicializa os pinos dos dosadores como output:
  for (cont = 0; cont < 3; cont++) {
    pinMode(AL[cont], OUTPUT);
    digitalWrite(AL[cont], LOW); // Garante que os dosadores estão desligados inicialmente
  }

  Serial.begin(9600);
#ifdef AVR
  Wire.begin();
#else
  Wire1.begin(); // Conecta os pinos I2C à I2C alternativa no Arduino Due
#endif
  rtc.begin();

  if (!rtc.isrunning()) {
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
  wdt_enable(WDTO_8S);
}

void loop() {
  executa();
  relogio();
  wdt_reset();
}
