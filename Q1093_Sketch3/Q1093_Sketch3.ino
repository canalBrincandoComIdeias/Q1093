#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <string>

// UUIDs de 128 bits (PARA USO REAL):
#define SERVICE_UUID "0000FFE0-0000-1000-8000-00805F9B34FB"
#define CHARACTERISTIC_UUID "0000FFE1-0000-1000-8000-00805F9B34FB"

#define pinCanalA 14
#define pinCanalB 27
#define pinCanalC 26
#define pinCanalD 25

// DECLARAÇÃO DA VARIÁVEL deviceConnected *ANTES* das classes
bool deviceConnected = false;

// DECLARAÇÃO DAS CLASSES DE CALLBACK ANTES DE USÁ-LAS
class MyServerCallbacks : public BLEServerCallbacks {
public:
  void onConnect(BLEServer *pServer) {
    deviceConnected = true;
    Serial.println("Conectado");
  }
  void onDisconnect(BLEServer *pServer) {
    deviceConnected = false;
    Serial.println("Desconectado");
    pServer->getAdvertising()->start();
  }
};

class MyCallbacks : public BLECharacteristicCallbacks {
public:
  void onWrite(BLECharacteristic *characteristic) {
    if (characteristic->getValue().length() > 0) {
      std::string rxValue(characteristic->getValue().c_str());
      char byteRecebido = rxValue[0];
      Serial.print("Dados recebidos: ");
      Serial.write(byteRecebido);
      Serial.println();

      if (byteRecebido == '*') {
        delay(1000);
        char byteEnviar;
        if (digitalRead(pinCanalA)) {
          byteEnviar = 'A';
        } else {
          byteEnviar = 'a';
        }
        Serial.print("Enviando estado atual CnA: ");
        Serial.write(byteEnviar);
        Serial.println();
        characteristic->setValue((uint8_t*)&byteEnviar, 1);
        characteristic->notify();  // Usando 'characteristic'

        if (digitalRead(pinCanalB)) {
          byteEnviar = 'B';
        } else {
          byteEnviar = 'b';
        }
        Serial.print("Enviando estado atual CnB: ");
        Serial.write(byteEnviar);
        Serial.println();
        characteristic->setValue((uint8_t*)&byteEnviar, 1);
        characteristic->notify();

        if (digitalRead(pinCanalC)) {
          byteEnviar = 'C';
        } else {
          byteEnviar = 'c';
        }
        Serial.print("Enviando estado atual CnC: ");
        Serial.write(byteEnviar);
        Serial.println();
        characteristic->setValue((uint8_t*)&byteEnviar, 1);
        characteristic->notify();

        if (digitalRead(pinCanalD)) {
          byteEnviar = 'D';
        } else {
          byteEnviar = 'd';
        }
        Serial.print("Enviando estado atual CnD: ");
        Serial.write(byteEnviar);
        Serial.println();
        characteristic->setValue((uint8_t*)&byteEnviar, 1);
        characteristic->notify();

      } else if (byteRecebido == 'a') {
        digitalWrite(pinCanalA, LOW);
        Serial.println("Desligando porta CnA.");
      }

      else if (byteRecebido == 'A') {
        digitalWrite(pinCanalA, HIGH);
        Serial.println("Ligando porta CnA.");

      } else if (byteRecebido == 'b') {
        digitalWrite(pinCanalB, LOW);
        Serial.println("Desligando porta CnB.");

      } else if (byteRecebido == 'B') {
        digitalWrite(pinCanalB, HIGH);
        Serial.println("Ligando porta CnB.");

      } else if (byteRecebido == 'c') {
        digitalWrite(pinCanalC, LOW);
        Serial.println("Desligando porta CnC.");

      } else if (byteRecebido == 'C') {
        digitalWrite(pinCanalC, HIGH);
        Serial.println("Ligando porta CnC.");

      } else if (byteRecebido == 'd') {
        digitalWrite(pinCanalD, LOW);
        Serial.println("Desligando porta CnD.");

      } else if (byteRecebido == 'D') {
        digitalWrite(pinCanalD, HIGH);
        Serial.println("Ligando porta CnD.");
      }
    }
  }
};

// DECLARAÇÃO DAS VARIÁVEIS GLOBAIS FORA DE QUALQUER FUNÇÃO
BLEServer *pServer = NULL;
BLECharacteristic *pCharacteristic = NULL;  // Inicializado com NULL

void setup() {
  // ... (resto do setup() igual à versão anterior)
  Serial.begin(115200);
  Serial.println("Iniciando ESP32 BLE...");

  pinMode(pinCanalA, OUTPUT);
  pinMode(pinCanalB, OUTPUT);
  pinMode(pinCanalC, OUTPUT);
  pinMode(pinCanalD, OUTPUT);

  BLEDevice::init("ESP32-BLE");
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());
  BLEService *pService = pServer->createService(SERVICE_UUID);

  uint32_t properties = BLECharacteristic::PROPERTY_NOTIFY | BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_WRITE_NR;

  pCharacteristic = pService->createCharacteristic(CHARACTERISTIC_UUID, properties);
  pCharacteristic->addDescriptor(new BLE2902());
  pCharacteristic->setCallbacks(new MyCallbacks());

  pService->start();
  pServer->getAdvertising()->start();
  Serial.println("Aguardando conexões BLE...");
}

void loop() {
  if (deviceConnected) {
    if (Serial.available() > 0) {
      size_t len = Serial.available();
      uint8_t sbuf[len];
      Serial.readBytes(sbuf, len);
      pCharacteristic->setValue(sbuf, len);
      pCharacteristic->notify();
      Serial.print("Dados enviados para o celular: ");
      Serial.write(sbuf, len);
      Serial.println();
    }
  }
  delay(10);
}