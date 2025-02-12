#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <WiFiManager.h> // Biblioteca para configuração do Wi-Fi

WiFiClient wifiClient;

const int lampPort = 55443;
String lampIP1, lampIP2, lampIP3, lampIP4, lampIP5, userEmail; // Variável para IPs e e-mail do usuário
const char* apiBaseURL = "http://apicheiro-dev.eba-bctbmw7j.us-east-1.elasticbeanstalk.com/api/sensordata/post-sensor-data/";

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

void setup() {
  Serial.begin(115200);

  WiFiManager wifiManager;
  wifiManager.resetSettings();  // Limpa configurações antigas

  // Parâmetros para IPs das lâmpadas e e-mail
  WiFiManagerParameter customLampIP1("lampIP1", "IP da Lâmpada 1", "", 16);
  WiFiManagerParameter customLampIP2("lampIP2", "IP da Lâmpada 2", "", 16);
  WiFiManagerParameter customLampIP3("lampIP3", "IP da Lâmpada 3", "", 16);
  WiFiManagerParameter customLampIP4("lampIP4", "IP da Lâmpada 4", "", 16);
  WiFiManagerParameter customLampIP5("lampIP5", "IP da Lâmpada 5", "", 16);
  WiFiManagerParameter customUserEmail("userEmail", "E-mail do Usuário", "", 40);

  wifiManager.addParameter(&customLampIP1);
  wifiManager.addParameter(&customLampIP2);
  wifiManager.addParameter(&customLampIP3);
  wifiManager.addParameter(&customLampIP4);
  wifiManager.addParameter(&customLampIP5);
  wifiManager.addParameter(&customUserEmail);

  wifiManager.setConnectTimeout(60); // Timeout de 60 segundos para conexão

  // Sempre iniciar portal de configuração
  if (!wifiManager.startConfigPortal("ESP8266_Config")) {
    Serial.println("Falha ao iniciar o portal de configuração");
    ESP.restart();
  }

  lampIP1 = customLampIP1.getValue();
  lampIP2 = customLampIP2.getValue();
  lampIP3 = customLampIP3.getValue();
  lampIP4 = customLampIP4.getValue();
  lampIP5 = customLampIP5.getValue();
  userEmail = customUserEmail.getValue();

  Serial.println("Wi-Fi configurado com sucesso!");
  Serial.println("E-mail do usuário: " + userEmail);
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    if (Serial.available()) {
      String sensorData = Serial.readStringUntil('\n');
      String jsonData = "{\"SensorValue\": " + sensorData + "}";

      int value = sensorData.toInt();
      if (value > 400) {
        alternarCoresTodasLampadas();
      }

      // Monta a URL com o e-mail do usuário
      String url = String(apiBaseURL) + userEmail;
      

      Serial.println("Enviando dados para a API: " + jsonData);
      Serial.println(url);
      http.begin(wifiClient, url); 
      http.addHeader("Content-Type", "application/json");

      int httpResponseCode = http.POST(jsonData);
      if (httpResponseCode > 0) {
        String response = http.getString();
        Serial.println("Resposta da API: " + response);
      } else {
        Serial.print("Erro ao enviar a requisição: ");
        Serial.println(httpResponseCode);
      }
      http.end();
    }
  } else {
    Serial.println("Wi-Fi desconectado!");
  }

  delay(500);
}
