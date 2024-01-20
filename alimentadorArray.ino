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
int AL[] = {5, 6, 7}; // 5 e 6

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

void doseracao() {
  unsigned long startMillis = millis();
  unsigned long elapsedMillis = 0;

  for (cont = 0; cont < 3; cont++) {
    digitalWrite(AL[cont], HIGH);
    while (elapsedMillis < tempo[cont]) {
      elapsedMillis = millis() - startMillis;
      // Aqui você pode adicionar outras ações que precisam ocorrer durante a espera
    }
    Serial.println("liga");
    digitalWrite(AL[cont], LOW);
    Serial.println("desliga");
    delay(2000);
  }
}

void executa() {
  DateTime now = rtc.now();
  // Trata todos
  if ((((now.hour() == horaliga1) || (now.hour() == horaliga2) || (now.hour() == horaliga3)) && (now.minute() == minlig) && (now.second() <= 2))) {
    doseracao();
  }
  // Trata aquario
  if ((((now.hour() == horaliga4)) && (now.minute() == minlig) && (now.second() <= 2))) {
    digitalWrite(AL[0], HIGH);
    delay(tempo[1]);
    digitalWrite(AL[0], LOW);
    delay(2000);
  }
}

void setup() {
  // Inicializa os pinos AL como output:
  for (cont = 0; cont < 3; cont++) {
    pinMode(AL[cont], OUTPUT);
  }

  Serial.begin(9600);
#ifdef AVR
  Wire.begin();
#else
  Wire1.begin(); // Shield I2C pins connect to alt I2C bus on Arduino Due
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
