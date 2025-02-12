#include <SoftwareSerial.h> 

const int buzzerPin = 8;           // Pino onde o buzzer está conectado
const int sensorPin = A0;          // Pino do sensor MQ-2
const int ledVerdePin = 11;        // Pino do LED verde (sem fumaça)
const int ledVermelhoPin = 10;     // Pino do LED vermelho (fumaça detectada)
const int threshold = 400;         // Limite do sensor MQ-2 para detectar fumaça

void setup() {
  pinMode(buzzerPin, OUTPUT);      // Configura o pino do buzzer como saída
  pinMode(sensorPin, INPUT);       // Configura o pino do sensor como entrada
  pinMode(ledVerdePin, OUTPUT);    // Configura o pino do LED verde como saída
  pinMode(ledVermelhoPin, OUTPUT); // Configura o pino do LED vermelho como saída

  Serial.begin(115200);              // Comunicação com o ESP
}

void loop() {
  int sensorValue = analogRead(sensorPin);  // Lê o valor do sensor MQ-2
  
  if (sensorValue > threshold) {
    digitalWrite(ledVerdePin, LOW);        // Desativa o LED verde
    digitalWrite(ledVermelhoPin, HIGH);    // Ativa o LED vermelho

    // Buzzer intermitente
    for (int i = 0; i < 5; i++) {          // Toca o buzzer 5 vezes
      tone(buzzerPin, 1000);               // Frequência de 1 kHz
      delay(500);                          // Buzzer ligado por 500 ms
      noTone(buzzerPin);                   // Desativa o buzzer
      delay(500);                          // Pausa de 500 ms
    }
    
    // Envia mensagem para o ESP
    Serial.println(sensorValue);
  } else {
    digitalWrite(ledVerdePin, HIGH);       // Ativa o LED verde
    digitalWrite(ledVermelhoPin, LOW);     // Desativa o LED vermelho
    noTone(buzzerPin);                     // Garante que o buzzer esteja desligado
    
    // Envia mensagem para o ESP
    
    Serial.println(sensorValue);
  }

  delay(5000);  // Aguarda 5 segundo antes de próxima leitura
} 
