#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <DHT.h>


#define DHTPIN D3      // Pin connected to the DHT11 sensor
#define DHTTYPE DHT11 // DHT sensor type
#define api_key "@3fkOby&ydn3)pdj+(2jn)<d6gLZerEk;*58ev"
const char* ssid = "EFISCT";
const char* password = "AFEA/*77320494";
const char* endpoint = "http://192.168.1.103:8000/dht/"; // Replace with your actual endpoint URL

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  dht.begin();
  connectToWiFi();
}

void loop() {
  
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  
  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  String temp = String(temperature, 2);
  String humi = String(humidity, 2);
  
  WiFiClient client;
  HTTPClient http;
  
  http.begin(client, endpoint);
  http.addHeader("Content-Type", "application/json");
  
  DynamicJsonDocument doc(300);
  doc["temp"] = temp;
  doc["humi"] = humi;
  doc["api_key"] = api_key;
  
  String data;

  serializeJson(doc, data);
  
  int httpPostResponseCode = http.POST(data);
  Serial.print("HTTP POST Response code: ");
  Serial.println(httpPostResponseCode);

  Serial.println("");
 
  int httpGetResponseCode = http.GET(); 
  Serial.print("HTTP GET Response code: ");
  Serial.println(httpGetResponseCode);
  String payload = http.getString();
  Serial.println(payload);
     
      
  
  http.end();

  delay(5000); // Adjust delay as needed
}

void connectToWiFi() {
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi...");
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  
  Serial.println("\nConnected to WiFi!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}
