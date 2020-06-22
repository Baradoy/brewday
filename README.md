# BrewDay
Temperature measurement and controll for brewing.

Brewdays are great fun especially when the brew turns out well. I wanted to have insight and controll around my mash and fermentation temperatures. I have built plenty of dashboards in the past, why not build one to help me brew?

# Equipment

Sensors are built with [Adafruit Feather HUZZAH ESP8266](https://www.adafruit.com/product/2821)s with [High Temp Waterproof DS18B20 Digital Temperature Sensor](https://www.adafruit.com/product/3846)s.
The docker setup is running off of A Raspberry Pi 4b with Raspbian installed on an external drive. 

# Running the Project

## Grafana and InfluxDB

Set the following environment variables: 
 - DB_NAME
 - DB_USER
 - DB_PASSWORD

Run `docker-compose up -d` to run InfluxDB and Grafana. Grafana can be accessed at `http:<pi4b-ip>:3000` with `admin:admin`.  

## Temperature Shipper

Connect your temperature sensors' power, ground and signal to the ESP's power, ground, and pin 2. Connect a 4.7 KΩ resistor between the signal and pin 2. You can find a diagram of the setup [here](https://learn.adafruit.com/using-ds18b20-temperature-sensor-with-circuitpython/hardware).

Use the template in `temperature_shipper.ino` to create a `config.h` file in the `temperature_shipper` directory. The Arduino IDE can be used to deploy the code in the `temperature_shipper` directory. 

# Acknowledgements
Thanks to [Starke](https://github.com/jrstarke/) and [Hez](https://github.com/hez/) for help with much of the initial templates and pointing me in the right direction. 
