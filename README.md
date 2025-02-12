

```markdown
# Arduíno TCC

Este repositório contém os códigos utilizados no projeto de TCC para a detecção de gás e fumaça com a utilização de um Arduino Uno Wi-Fi, ESP8266 e o sensor MQ-2. O código é responsável por monitorar a presença de fumaça ou gás, acionar alertas sonoros e visuais, além de enviar os dados do sensor para a API.

## Funcionalidades

- **Leitura de Sensor MQ-2**: O código monitora a leitura do sensor MQ-2, que detecta fumaça e gás.
- **Alertas Visuais e Sonoros**: Quando a fumaça ou gás é detectado, o código ativa o buzzer e LEDs (vermelho/verde) para alertar o usuário.
- **Integração com ESP8266**: O ESP8266 é utilizado para enviar os dados do sensor para a API, bem como controlar as lâmpadas Yeelight conectadas via Wi-Fi.

## Como Executar

### Requisitos

- **Placa Arduino Uno Wi-Fi (Atmega328)**.
- **ESP8266**.
- **Sensor MQ-2**.
- **Lâmpadas Yeelight E27 (Wi-Fi)**.
- **IDE Arduino** (para upload do código na placa).

### Passos para o Upload do Código

1. Faça o upload do código da **Placa Uno Wi-Fi**(_CódigoUno) e do **ESP8266** (_CódigoESP_V4) para as respectivas placas utilizando a IDE do Arduino.
   
2. **Configuração das Chaves**:
   - Para **carregar o código na placa Uno Wi-Fi**, altere as **Chaves 3 e 4** para **ON**.
   - Para **carregar o código no ESP8266**, altere as **Chaves 5, 6 e 7** para **ON**.
   - Para garantir a comunicação entre as placas, mantenha as **Chaves 1, 2 e 5** no modo **ON**.

3. Conecte o sensor MQ-2 à placa Arduino e conecte as lâmpadas Yeelight à rede Wi-Fi.

### Código da Placa Uno Wi-Fi

O código da placa Uno Wi-Fi utiliza o sensor MQ-2 para detectar fumaça ou gás, acionar o buzzer e LEDs, e enviar dados para a API. O código do ESP8266 permite configurar as lâmpadas Yeelight e enviar as informações para a API.

### Exemplo de Código do ESP8266

```cpp
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <WiFiManager.h> // Biblioteca para configuração do Wi-Fi

WiFiClient wifiClient;

const int lampPort = 55443;
String lampIP1, lampIP2, lampIP3, lampIP4, lampIP5, userEmail; // Variáveis para IPs e e-mail do usuário
const char* apiBaseURL = "http://apicheiro-dev.eba-bctbmw7j.us-east-1.elasticbeanstalk.com/api/sensordata/post-sensor-data/";

// Função para alternar as cores das lâmpadas
void alternarCoresTodasLampadas() {
  String vermelho = "{\"id\":1,\"method\":\"set_rgb\",\"params\":[16711680, \"smooth\", 500]}\r\n";  // Vermelho
  String branco = "{\"id\":1,\"method\":\"set_rgb\",\"params\":[16777215, \"smooth\", 500]}\r\n";    // Branco

  String lampIPs[] = {lampIP1, lampIP2, lampIP3, lampIP4, lampIP5};

  for (int i = 0; i < 5; i++) {
    if (!lampIPs[i].isEmpty() && wifiClient.connect(lampIPs[i].c_str(), lampPort)) {
      wifiClient.print(vermelho);
      wifiClient.stop();
    }
  }
  delay(1000);

  for (int i = 0; i < 5; i++) {
    if (!lampIPs[i].isEmpty() && wifiClient.connect(lampIPs[i].c_str(), lampPort)) {
      wifiClient.print(branco);
      wifiClient.stop();
    }
  }
  delay(1000);
  Serial.println("Lâmpadas alternaram cores simultaneamente!");
}
