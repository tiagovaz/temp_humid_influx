// https://github.com/tiagovaz/temp_humid_influx

#include <Fishino.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <ArduinoJson.h>

// For json serializer 
#define DEVICE "YOUR_DEVICE_NAME"

// Network setup
#ifndef __MY_NETWORK_H
#define MY_SSID  "yourssid"
#define MY_PASS	"yourpass"
#endif

// Set DHT pin and type:
#define DHTPIN 2
#define DHTTYPE DHT11

// Initialize DHT sensor for normal 16mhz Arduino:
DHT dht = DHT(DHTPIN, DHTTYPE);

FishinoServer server(80);

void printWifiStatus()
{
	// print the SSID of the network you're attached to:
	// stampa lo SSDI della rete a cui si è connessi
	Serial.print("SSID: ");
#ifdef STANDALONE_MODE
	Serial.println(MY_SSID);
#else
	Serial.println(Fishino.SSID());
#endif

	// get phy mode and show it
	uint8_t mode = Fishino.getPhyMode();
	Serial.print("PHY MODE: (");
	Serial.print(mode);
	Serial.print(") ");
	switch(mode)
	{
		case PHY_MODE_11B:
			Serial.println("11B");
			break;

		case PHY_MODE_11G:
			Serial.println("11G");
			break;

		case PHY_MODE_11N:
			Serial.println("11N");
			break;
			
		default:
			Serial.println("UNKNOWN");
	}
	
#ifdef STANDALONE_MODE
	// get AP IP info
	IPAddress ip, gw, nm;
	if(Fishino.getApIPInfo(ip, gw, nm))
	{
		Serial << F("Fishino IP      :") << ip << "\r\n";
		Serial << F("Fishino GATEWAY :") << gw << "\r\n";
		Serial << F("Fishino NETMASK :") << nm << "\r\n";
	}
	else
		Serial << F("Couldn't get Fishino IP info\r\n");
#else
	// print your Fishino's IP address:
	IPAddress ip = Fishino.localIP();
	Serial.print("IP Address: ");
	Serial.println(ip);

	// print the received signal strength:
	Serial.print("signal strength (RSSI):");
	Serial.print(Fishino.RSSI());
	Serial.println(" dBm");
#endif

}

//Json setup

StaticJsonDocument<64> sjson;
JsonObject sjson_dht11 = sjson["dht11"].createNestedObject();
char json_string[256];

void setup()
{

  // Initialize sensor
  dht.begin();  
  
	//Initialize serial and wait for port to open:
	Serial.begin(115200);

	// reset and test wifi module
	Serial << F("Resetting Fishino...");
	while(!Fishino.reset())
	{
		Serial << ".";
		delay(500);
	}
	Serial << F("OK\r\n");

	Serial << F("Fishino WiFi web server\r\n");

	// set PHY mode to 11G
	Fishino.setPhyMode(PHY_MODE_11G);
	
	// for AP MODE, setup the AP parameters
  #ifdef STANDALONE_MODE
  	// setup SOFT AP mode
  	Serial << F("Setting mode SOFTAP_MODE\r\n");
  	Fishino.setMode(SOFTAP_MODE);
  
  	// stop AP DHCP server
  	Serial << F("Stopping DHCP server\r\n");
  	Fishino.softApStopDHCPServer();
  	
  	// setup access point parameters
  	Serial << F("Setting AP IP info\r\n");
  	Fishino.setApIPInfo(ip, ip, IPAddress(255, 255, 255, 0));
  
  	Serial << F("Setting AP WiFi parameters\r\n");
  	Fishino.softApConfig(MY_SSID, MY_PASS, 1, false);
  	
  	// restart DHCP server
  	Serial << F("Starting DHCP server\r\n");
  	Fishino.softApStartDHCPServer();
  	
  #else
  	// setup STATION mode
  	Serial << F("Setting mode STATION_MODE\r\n");
  	Fishino.setMode(STATION_MODE);
  
  	Serial << F("Connecting to AP...");
  	while(!Fishino.begin(MY_SSID, MY_PASS))
  	{
  		Serial << ".";
  		delay(500);
  	}
  	Serial << F("OK\r\n");
  
  	// setup IP or start DHCP server
    #ifdef IPADDR
  	  Fishino.config(ip, gw, nm);
    #else
  	  Fishino.staStartDHCP();
    #endif

  	// wait for connection completion
  	Serial << F("Waiting for IP...");
  	while(Fishino.status() != STATION_GOT_IP)
  	{
  		Serial << ".";
  		delay(500);
  	}
  	Serial << F("OK\r\n");  
  #endif

	// show connection status
	printWifiStatus();

	// start listening for clients
	server.begin();
}


void loop()
{
	// wait for a new client:
	FishinoClient client = server.available();

	if (client)
	{
		Serial.println("new client");
		
		// an http request ends with a blank line
		boolean currentLineIsBlank = true;
		while (client.connected())
		{
			if (client.available())
			{
				char c = client.read();
				Serial.write(c);
				
				// if you've gotten to the end of the line (received a newline
				// character) and the line is blank, the http request has ended,
				// so you can send a reply
				if (c == '\n' && currentLineIsBlank)
				{
					// send a standard http response header
					client.println("HTTP/1.1 200 OK");
					client.println("Content-Type: application/json");
					client.println("Connection: close");  // the connection will be closed after completion of the response
					//client.println("Refresh: 5");  // refresh the page automatically every 5 sec
					client.println();
//					client.println("<!DOCTYPE HTML>");
//					client.println("<html>");

          // Read the humidity in %:
          float h = dht.readHumidity();
          // Read the temperature as Celsius:
          float t = dht.readTemperature();
          // Compute heat index as Celsius:
          float hic = dht.computeHeatIndex(t, h, false);

          // Serializer
          sjson_dht11["temp"] = t;
          sjson_dht11["humid"] = h;
          sjson["device"] = DEVICE;

          serializeJson(sjson, json_string);
          client.println(json_string);
					break;
				}
				if (c == '\n')
				{
					// you're starting a new line
					currentLineIsBlank = true;
				}
				else if (c != '\r')
				{
					// you've gotten a character on the current line
					currentLineIsBlank = false;
				}
			}
		}
		// give the web browser time to receive the data
		delay(1);

		// close the connection:
		client.stop();
		Serial.println("client disonnected");
//		Serial << F("Free RAM: ") << FreeRam() << "\n";
	}
}
