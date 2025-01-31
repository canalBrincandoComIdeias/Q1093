#include <SoftwareSerial.h>

#define pinCanalA 8
#define pinCanalB 9
#define pinCanalC 10
#define pinCanalD 11

SoftwareSerial moduloBT(3, 4);  // RX, TX

bool conexaoCancelada = false;
bool oKRecebido = false;

void setup() {
  pinMode(pinCanalA, OUTPUT);
  pinMode(pinCanalB, OUTPUT);
  pinMode(pinCanalC, OUTPUT);
  pinMode(pinCanalD, OUTPUT);

  Serial.begin(9600);
  while (!Serial) {
    ;
  }
  Serial.println("Serial iniciado.");

  moduloBT.begin(9600);
}

void loop() {

  //Dados recebidos do bluetooth
  if (moduloBT.available()) {
    char byteRecebido = moduloBT.read();

    //Retorno ao monitor serial
    Serial.write(byteRecebido);
    Serial.println();

    //Pedido de estado
    if (byteRecebido == '*') {
      char byteEnviar;

      //Canal A
      if (digitalRead(pinCanalA)) {
        byteEnviar = 'A';
      } else {
        byteEnviar = 'a';
      }

      Serial.print("Enviando estado atual CnA: ");
      Serial.write(byteEnviar);
      Serial.println();

      moduloBT.write(byteEnviar);

      //Canal B
      if (digitalRead(pinCanalB)) {
        byteEnviar = 'B';
      } else {
        byteEnviar = 'b';
      }

      Serial.print("Enviando estado atual CnB: ");
      Serial.write(byteEnviar);
      Serial.println();

      moduloBT.write(byteEnviar);

      //Canal C
      if (digitalRead(pinCanalC)) {
        byteEnviar = 'C';
      } else {
        byteEnviar = 'c';
      }

      Serial.print("Enviando estado atual CnC: ");
      Serial.write(byteEnviar);
      Serial.println();

      moduloBT.write(byteEnviar);

      //Canal D
      if (digitalRead(pinCanalD)) {
        byteEnviar = 'D';
      } else {
        byteEnviar = 'd';
      }

      Serial.print("Enviando estado atual CnD: ");
      Serial.write(byteEnviar);
      Serial.println();

      moduloBT.write(byteEnviar);
    }

    //Comando de desconexao. Ignora texto automatico do modulo.
    if (byteRecebido == '+') {
      conexaoCancelada = true;
    }

    if (conexaoCancelada) {
      if (byteRecebido == 'O') {
        oKRecebido = true;
      }
      if (oKRecebido) {
        if (byteRecebido == 'K') {
          conexaoCancelada = false;
          oKRecebido = false;
        }
      }
    }

    if (!conexaoCancelada) {

      if (byteRecebido == 'a') {
        Serial.println("Desligando porta CnA.");
        digitalWrite(pinCanalA, LOW);
      }

      if (byteRecebido == 'A') {
        Serial.println("Ligando porta CnA.");
        digitalWrite(pinCanalA, HIGH);
      }

      if (byteRecebido == 'b') {
        Serial.println("Desligando porta CnB.");
        digitalWrite(pinCanalB, LOW);
      }

      if (byteRecebido == 'B') {
        Serial.println("Ligando porta CnB.");
        digitalWrite(pinCanalB, HIGH);
      }

      if (byteRecebido == 'c') {
        Serial.println("Desligando porta CnC.");
        digitalWrite(pinCanalC, LOW);
      }

      if (byteRecebido == 'C') {
        Serial.println("Ligando porta CnC.");
        digitalWrite(pinCanalC, HIGH);
      }

      if (byteRecebido == 'd') {
        Serial.println("Desligando porta CnD.");
        digitalWrite(pinCanalD, LOW);
      }

      if (byteRecebido == 'D') {
        Serial.println("Ligando porta CnD.");
        digitalWrite(pinCanalD, HIGH);
      }
    }
  }

  //Dados digitados no monitor serial, repassados para o bluetooth
  if (Serial.available()) {
    moduloBT.write(Serial.read());
  }
}
