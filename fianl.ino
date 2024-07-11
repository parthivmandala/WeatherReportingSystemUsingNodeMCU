#include <DHT.h>
#include <ESP8266HTTPClient.h> 
#include <ESP8266WiFi.h> 

// Pin Definitions
#define DHTPIN D5      // Pin connected to the DHT11 sensor
#define DHTTYPE DHT11  // DHT 11
#define RAINPIN D6     // Pin connected to the Rain sensor
#define LDRPIN A0      // Pin connected to the LDR

// Create an instance for the DHT11 sensor
DHT dht(DHTPIN, DHTTYPE);

// Your WiFi credentials
char ssid[] = "Parthiv";
char pass[] = "12345678";

// WiFi client object
WiFiClient client;

void setup() {
  // Start serial communication
  Serial.begin(115200);
  Serial.println("Sensor Test");

  // Initialize the DHT11 sensor
  dht.begin();
  delay(2000); // Delay to stabilize DHT11 sensor
  Serial.println("DHT11 sensor initialized.");

  // Set pin modes
  pinMode(RAINPIN, INPUT);
  pinMode(LDRPIN, INPUT);

  // Connect to WiFi
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
}

void loop() {
  Serial.println("Reading sensors...");

  // Read data from DHT11
  float humidity = dht.readHumidity();
  float temperatureDHT = dht.readTemperature();

  // Check if DHT11 readings are valid
  if (isnan(humidity) || isnan(temperatureDHT)) {
    Serial.println("Failed to read from DHT sensor!");
  } else {
    // Check DHT11 readings
    Serial.print("DHT11 Temperature: ");
    Serial.print(temperatureDHT);
    Serial.print(" *C\t");
    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.println(" %");
  }

  // Read data from Rain Sensor
  int rainValue = digitalRead(RAINPIN);
  String rainStatus = rainValue == HIGH ? "It's not Raining" : "It's Raining";

  // Check Rain Sensor reading
  Serial.print("Rain Sensor: ");
  Serial.println(rainStatus);

  // Read data from LDR Sensor
  int ldrValue = analogRead(LDRPIN);
  String ldrStatus = ldrValue < 700 ? "It's Dark" : "It's Light";

  // Check LDR reading
  Serial.print("LDR Value: ");
  Serial.println(ldrValue);
  Serial.print("LDR Status: ");
  Serial.println(ldrStatus);

  HTTPClient http;
  http.begin(client, "http://192.168.198.195:5000/values");
  http.addHeader("Content-Type", "application/json"); // Specify content-type header

  // Prepare the HTTP request data
  String httpRequestData = "" +
    String("\"Temperature\": ") + String(temperatureDHT) + ",\n" +
    String("\"Humidity\": ") + String(humidity) + ",\n" +
    String("\"LDR_Value\": ") + String(ldrValue) + ",\n" +
    String("\"LDR_Status\": \"") + ldrStatus + "\",\n" +
    String("\"Rain_Status\": ") + String(rainValue == HIGH ? 0 : 1) + ",\n" +
    String("\"Rain_Description\": \"") + rainStatus + "\"\n";

  // Add warnings for high temperature and humidity
  if (temperatureDHT > 35) {
    httpRequestData += ",\n\"Temperature_Warning\": \"It's too hot!\"";
  }
  if (humidity > 85) {
    httpRequestData += ",\n\"Humidity_Warning\": \"It's too humid!\"";
  }

  httpRequestData += "\n";

  int httpResponseCode = http.POST(httpRequestData);

  if (httpResponseCode > 0) {
    String response = http.getString();
    Serial.println(httpResponseCode);
    Serial.println(response);
  } else {
    Serial.print("Error on sending POST: ");
    Serial.println(httpResponseCode);
  }

  http.end();

  // Add delay between readings
  delay(4000); // Wait 4 seconds before next reading
}