#include "config.h"

/* config.h should look something like the following:
 * #define INFLUXDB_HOST <host>
 * #define INFLUXDB_DB "climate"
 * #define INFLUXDB_USER "db_user"
 * #define INFLUXDB_PASSWORD "db_password"
 * #define INFLUXDB_PORT 8086
 * #define WIFI_SSID <ssid>
 * #define WIFI_PASSWORD <wifi password>
 */

/* REQUIRES the following Arduino libraries:
 * - DallasTemperature Sensors: https://github.com/milesburton/Arduino-Temperature-Control-Library
 * - OneWire Protocol: https://github.com/PaulStoffregen/OneWire
 * - ESP8266 Influxdb: https://github.com/tobiasschuerg/InfluxDB-Client-for-Arduino
 */

#include <OneWire.h>
#include <DallasTemperature.h>
#include <ESP8266WiFi.h>
#include <InfluxDb.h>

#define READ_DELAY 5
#define ONE_WIRE_BUS 2 // PIN Connected the OneWire bus
#define INDICATOR_LIGHT 0

// Init Libraries 
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
Influxdb influx(INFLUXDB_HOST);
int sensorCount = 1;

// Set Wifi details
const char* ssid     = WIFI_SSID;
const char* password = WIFI_PASSWORD;
char* hostname = "esp_sensor";

void setupSensors() {
  sensors.begin();
  sensorCount = sensors.getDeviceCount();
}

void setupInfluxDB() {
  influx.setDbAuth(INFLUXDB_DB, INFLUXDB_USER, INFLUXDB_PASSWORD);
  influx.setVersion(1);
  influx.setPort(INFLUXDB_PORT);
}

void setupWifi() {  
  itoa(ESP.getChipId(), hostname, 16);
  WiFi.hostname(hostname);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(200);
    Serial.print(".");
  }
 
  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println(ESP.getChipId());
}

void sendSensorData() {
  sensors.requestTemperatures();

  for (int i = 0; i < sensorCount; i++) { 
    float temperatureMeasurement = sensors.getTempCByIndex(i);

    Serial.print("Transmitting ");
    Serial.print(temperatureMeasurement);
    Serial.println(" to Influx");

    InfluxData row("temperature");
    row.addTag("host", hostname);
    row.addTag("index", String(i));
    row.addValue("value", temperatureMeasurement);
    if (influx.write(row)) {
      Serial.println("Transmittion Succeeded");
    } else {
      Serial.println("Transmittion Failed");
      Serial.println(influx.getLastErrorMessage());
    }
  }
}

void setup() {
  Serial.begin(9600);
  
  // Setup the indicator light
  pinMode(INDICATOR_LIGHT, OUTPUT);    
  

  setupWifi();
  setupInfluxDB();
  setupSensors();
}

void loop() {

  digitalWrite(INDICATOR_LIGHT, LOW);
  sendSensorData();
  digitalWrite(INDICATOR_LIGHT, HIGH);

  delay(READ_DELAY * 1000);
}
