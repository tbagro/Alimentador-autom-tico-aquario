
//código relógio rtc alimentador
#include <Wire.h>
#include "RTClib.h"
#include <avr/wdt.h>

RTC_DS1307 rtc;
char hora[30];

/// Hora da festa
int horaliga1 = 6;
int horaliga2 = 18;
int horaliga3 = 11;
int horaliga4 = 9;
int minlig = 40;

/// liga dosadores
int AL[] = {5, 6, 7};//5 e 6

/// define quantidade de tempo de acionamento dosadores
int tempo[] = {0, 690, 300};//PEQUENO, MEDIO, GRANDE
int cont;

//tempo millis
const long interval = 1000;
unsigned long previousMillis = 0;


void relogio () {

  unsigned long currentMillis = millis();
  DateTime now = rtc.now();
  sprintf( hora, "%02d:%02d:%02d", now.hour(), now.minute(), now.second() );
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    Serial.println(hora);
  }
}

void doseracao() {
  for (cont = 0; cont < 3; cont++) {
    digitalWrite(AL[cont], HIGH);
    delay(tempo[cont]);
    Serial.println("liga");
    digitalWrite(AL[cont], LOW);
    Serial.println("deslig");
    delay(2000);
  }
}

void executa () {
  DateTime now = rtc.now();
 //trata todos
  if ((((now.hour() == horaliga1) || (now.hour() == horaliga2) || (now.hour() == horaliga3)) && (now.minute() == minlig) && (now.second() <= 2))) {
    doseracao();
  }
  //trata aquario 
  if ((((now.hour() == horaliga4)) && (now.minute() == minlig) && (now.second() <= 2))) {
    digitalWrite(AL[0], HIGH);
    delay(tempo[1]);
    digitalWrite(AL[0], LOW);
    delay(2000);
  }
}

void setup() {
  // inicializa os pinos AL como output:
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

  if ( rtc.isrunning()) {
   // Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(__DATE__, __TIME__));
  }
  wdt_enable(WDTO_8S); //Função que ativa e altera o Watchdog
}
void loop() {
  executa ();
  relogio ();
  wdt_reset(); //diz que esta tudo ok
}
