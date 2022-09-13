#include <DHT.h>  // Librería para el DHT11
 
#include <ESP8266WiFi.h>
 
String apiKey = "*****";     //  Introduce la Write API key desde ThingSpeak
 
const char *ssid =  "*****";     // Reemplazar por el SSID del WiFi y su WPA2 key
const char *pass =  "*****";
const char* server = "api.thingspeak.com";
 
#define DHTPIN 0          //pin donde está conectado el DHT11
 
DHT dht(DHTPIN, DHT11);
 
WiFiClient client;
 
void setup() 
{
       Serial.begin(115200);
       delay(10);
       dht.begin();
 
       Serial.println("Conectando a ");
       Serial.println(ssid);
 
 
       WiFi.begin(ssid, pass);
 
      while (WiFi.status() != WL_CONNECTED) 
     {
            delay(500);
            Serial.print(".");
     }
      Serial.println("");
      Serial.println("Conectado a WiFi");
 
}
 
void loop() 
{
  
      float h = dht.readHumidity();
      float t = dht.readTemperature();
      
              if (isnan(h) || isnan(t)) 
                 {
                     Serial.println("¡Fallo en la lectura del sensor DHT!");
                      return;
                 }
 
                         if (client.connect(server,80))   //   "184.106.153.149" o api.thingspeak.com
                      {  
                            
                             String postStr = apiKey;
                             postStr +="&field1=";
                             postStr += String(t);
                             postStr +="&field2=";
                             postStr += String(h);
                             postStr += "\r\n\r\n";
 
                             client.print("POST /update HTTP/1.1\n");
                             client.print("Host: api.thingspeak.com\n");
                             client.print("Connection: close\n");
                             client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
                             client.print("Content-Type: application/x-www-form-urlencoded\n");
                             client.print("Content-Length: ");
                             client.print(postStr.length());
                             client.print("\n\n");
                             client.print(postStr);
 
                             Serial.print("Temperatura: ");
                             Serial.print(t);
                             Serial.print(" grados Celcius, Humedad: ");
                             Serial.print(h);
                             Serial.println("%. Enviado a ThingSpeak.");
                        }
          client.stop();
 
          Serial.println("Esperando...");
  
  // ThingSpeak necesita un delay mínimo de 15 segundos entre actualizaciones de datos
  delay(30000);
}
