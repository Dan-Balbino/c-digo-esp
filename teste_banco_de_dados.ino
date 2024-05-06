#include <WiFi.h>
#include <Wire.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

const char* ssid = "Danilo";
const char* password =  "20420104";

String cria_arquivo_JSON(float temperatura, int umidade) {
  // Criar um objeto JSON
  StaticJsonDocument<200> doc;
  doc["sensor_id"] = "1";
  doc["temperatura"] = temperatura; // Cria a propriedade temperatura
  doc["umidade"] = umidade; // Cria a propriedade umidade

  // Adiciona as propriedades no arquivo JSON, em seguida retorna ele
  String jsonString;
  serializeJson(doc, jsonString);
  return jsonString;
}

void setup() {
  Serial.begin(115200);
  delay(4000);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando ao WiFi...");
  }

  Serial.println("Conectado ao WiFi");
  Serial.println(WiFi.localIP());}

void loop() {
  delay(2000);

  float u = 20;
  float t = 32.6;

  if (isnan(u) || isnan(t)) {
    Serial.println("Falha ao ler do sensor DHT!");
  } else {
    Serial.print("Umidade: ");
    Serial.print(u);
    Serial.print(" %\t");
    Serial.print("Temperatura: ");
    Serial.print(t);
    Serial.println(" *C ");

    HTTPClient http;

    String serverPath = "http://localhost:3000/dados-sensores";
    Serial.println(serverPath);

    int timeout = 15000;

    http.setTimeout(timeout);

    http.begin(serverPath.c_str());

    http.addHeader("Content-Type", "application/json");

    String arq_Json = cria_arquivo_JSON(t, u);
    Serial.println("JSON: " + arq_Json);

    int httpResponseCode = http.POST(arq_Json);

    if(httpResponseCode>0){
      String response = http.getString();                       
      Serial.println(httpResponseCode);   
      Serial.println(response);           
    }else{
      Serial.print("Erro na requisição: ");
      Serial.println(httpResponseCode);
    }
    Serial.println("---------------------------------------------------------------------------------------------------------------");
    http.end();
  }}
