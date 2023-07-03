#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <DHT.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define DHTPIN D3      // Pin connected to the DHT11 sensor
#define DHTTYPE DHT11 // DHT sensor type

#define api_key "@3fkOby&ydn3)pdj+(2jn)<d6gLZerEk;*58ev"

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin
#define SCREEN_ADDRESS 0x3C

const char* ssid = "EFISCT";
const char* password = "AFEA/*77320494";
const char* endpoint = "http://192.168.1.103:8000/dht/"; // Replace with your actual endpoint URL

DHT dht(DHTPIN, DHTTYPE);

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


void setup() {
  Serial.begin(115200);
  dht.begin();
  
  // initialize the OLED object
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  // Clear the buffer.  
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(WHITE);
  

  
  connectToWiFi();
}

void loop() {
  
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  
  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor!");
    display.clearDisplay();
    display.setCursor(0,10);
    display.println("Failed to read from DHT sensor!");
    display.display();


    return;
  }

  String temp = String(temperature, 2);
  String humi = String(humidity, 2);

  display.clearDisplay();
  display.setCursor(2,10);
  display.println("TEMPERATURE: "+temp+" C");
  display.println("");
  display.setCursor(2,30);
  display.println("HUMIDITY: "+humi+" %");
  display.display();


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

  if (httpPostResponseCode == 201){
    display.setCursor(0,50);
    display.print("POST:DONE ");
    display.display();
  }
  else{
    display.setCursor(0,50);
    display.print("POST:FAILD ");
    display.display();
  }

  Serial.println("");
 
  int httpGetResponseCode = http.GET(); 
  Serial.print("HTTP GET Response code: ");
  Serial.println(httpGetResponseCode);

  if (httpGetResponseCode == 200){
      display.setCursor(64,50);
      display.println("GET:DONE");
      display.display();
    }
  else{
      display.setCursor(64,50);
      display.println("GET:FAILD");
      display.display();
    }
  
  String payload = http.getString();
  Serial.println(payload);
     
  http.end();

  delay(5000); // Adjust delay as needed

}

void connectToWiFi() {
  
  display.clearDisplay();
  display.setCursor(3,10);
  
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi...");
  
  display.println("Connecting to WiFi..");
  display.display();
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  
  display.clearDisplay();
  display.setCursor(3,10);
  
  Serial.println("\nConnected to WiFi!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
  display.println("\nConnected to "+ WiFi.SSID());
  display.setCursor(3,35);
  display.print("IP : ");
  display.println(WiFi.localIP());
  display.display();
  delay(4000);
}