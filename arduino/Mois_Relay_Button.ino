// #include <FS.h>                   //this needs to be first, or it all crashes and burns...
#include <ESP8266WiFi.h>
// #include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiClient.h>
#include <ESP8266WiFiMulti.h>
// #include <ESP8266HTTPClient.h>
#include <PubSubClient.h>
#include "DHT.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

WiFiClient espClientMqtt;
// WiFiClient espClientHttp;
ESP8266WiFiMulti wifiMulti;

LiquidCrystal_I2C lcd(0x27, 16, 2);

const char* ssid = "rus";
const char* password = "77777777";
const char* mqtt_server = "broker.hivemq.com";

PubSubClient mqttClient(espClientMqtt);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE	(50)
char msg[MSG_BUFFER_SIZE];
int value = 0;

#include "DHT.h"
#define DHTPIN D3
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

int analogPinMoisture = A0;
int mois_val = 0;

int relay1Pin = D0;
int relay2Pin = D4;

int btn1Pin = D6;
int btn2Pin = D7;
int btn3Pin = D8;

int btn1State = 1;
int btn2State = 1;
int btn3State = 1;

int autoState = 1;

int autoState_lcd = 1;
int fanState_lcd = 0;
int pumpState_lcd = 0;

int new_mois_value;

void setup() 
{
  
  Serial.begin(115200);

  dht.begin();
  
  
  pinMode(relay1Pin, OUTPUT);
  pinMode(relay2Pin, OUTPUT);

  pinMode(btn1Pin, INPUT);
  pinMode(btn2Pin, INPUT);

  pinMode(D5, OUTPUT);

  digitalWrite(relay1Pin, 1);
  digitalWrite(relay2Pin, 1);

  setup_wifi();
  mqttClient.setServer(mqtt_server, 1883);
  mqttClient.setCallback(callback);

  lcd.begin();
  lcd.backlight();
}
 
void loop() 
{
  if (!mqttClient.connected()) 
  {
    reconnect();
  }
  mqttClient.loop();

  unsigned long now = millis();

  if (now - lastMsg > 2000) 
  {
    lcd.setCursor(0, 0); 
    lcd.print("                "); 
    lcd.setCursor(0, 1); 
    lcd.print("                "); 
    
    lastMsg = now;
    
    // DHT11 ************************************************************************************************
    float h = dht.readHumidity();
    float t = dht.readTemperature();

    if (isnan(t) || isnan(h)) 
    {
      Serial.println("Failed to read from DHT");
    } 
    else 
    {
      Serial.print("Humidity: "); 
      Serial.print(h);
      Serial.print(" %\t");
      Serial.print("Temperature: "); 
      Serial.print(t);
      Serial.println(" *C");

      static char t_ch[7];
      dtostrf(t, 6, 2, t_ch);

      static char h_ch[7];
      dtostrf(h, 6, 2, h_ch);

      mqttClient.publish("sf/temp", t_ch);
      mqttClient.publish("sf/humid", h_ch);

      lcd.setCursor(0, 0); 
      lcd.print("Temp:  "); 
      lcd.print(t_ch);
      lcd.setCursor(0, 1); 
      lcd.print("Humid: ");
      lcd.print(h_ch);
      delay(2000);      
    }

    // Moisture ***************************************************************************************************
    mois_val = analogRead(analogPinMoisture)/10.24;

    new_mois_value = (((mois_val - 0) * (0 - 100)) / (100 - 0)) + 100;

    Serial.print("mois_val = ");
    Serial.println(new_mois_value);

    static char mois_val_ch[7];
    dtostrf(new_mois_value, 6, 2, mois_val_ch);

    mqttClient.publish("sf/mois", mois_val_ch);

    lcd.setCursor(0, 0); 
    lcd.print("Mois: ");
    lcd.print(mois_val_ch);
    lcd.setCursor(0, 1);
    lcd.print("                ");
    delay(2000);      

    // // database temp humid mois ********************************************************************************************
  
    // HTTPClient httpClient;

    // httpClient.begin(espClientHttp, "192.168.43.183:8080/esp8266/minipj/add.php"); // conect to pc ip
    // httpClient.addHeader("Content-Type", "application/x-www-form-urlencoded");

    // int httpCode = httpClient.POST("temp=" + String(t) + "&humid=" + String(h) + "&mois=" + String(new_mois_value));

    // if (httpCode > 0)
    // {
    //   Serial.printf("[HTTP] GET... code: %d\n", httpCode);
    //   if (httpCode == HTTP_CODE_OK) 
    //   {
    //     String payload = httpClient.getString();
    //     Serial.println(payload);
    //   }
    // }
    // else
    // {
    //   Serial.printf("[HTTP] GET... failed, error: %s\n", httpClient.errorToString(httpCode).c_str());
    // }

    // httpClient.end();

    // Button *****************************************************************************************************
    if (digitalRead(btn1Pin))
    {
      btn1State = !btn1State;
      autoState = !autoState;

      if (autoState)
      {
        mqttClient.publish("sf/autoState", "ON");
        digitalWrite(D5, 0);    
        autoState_lcd = 1;    
      }
      else
      {
        mqttClient.publish("sf/autoState", "OFF");
        mqttClient.publish("sf/fanState", "OFF");
        mqttClient.publish("sf/pumpState", "OFF");
        digitalWrite(relay1Pin, 1);
        digitalWrite(relay2Pin, 1);
        digitalWrite(D5, 1);
        autoState_lcd = 0;  
        fanState_lcd = 0;
        pumpState_lcd = 0;
      }      
      
      while (digitalRead(btn1Pin)) { delay(100); }  
    }
    if (digitalRead(btn2Pin))
    {
      btn2State = !btn2State;
      digitalWrite(relay1Pin, btn2State);

      if (btn2State)
      {
        mqttClient.publish("sf/fanState", "OFF");
        fanState_lcd = 0;
      }
      else
      {
        mqttClient.publish("sf/fanState", "ON");
        fanState_lcd = 1;
      }  

      while (digitalRead(btn2Pin)) { delay(100); }  
    }
    if (digitalRead(btn3Pin))
    {
      btn3State = !btn3State;
      digitalWrite(relay2Pin, btn3State);

      if (btn3State)
      {
        mqttClient.publish("sf/pumpState", "OFF");
        pumpState_lcd = 0;
      }
      else
      {
        mqttClient.publish("sf/pumpState", "ON");
        pumpState_lcd = 1;
      }
      
      while (digitalRead(btn3Pin)) { delay(100); }  
    }

    // auto state *****************************************************************************************************
    if (autoState)
    {
      if (t > 35)
      {
        digitalWrite(relay1Pin, 0);
        mqttClient.publish("sf/fanState", "ON");
        fanState_lcd = 0;
      }
      else
      {
        digitalWrite(relay1Pin, 1);
        mqttClient.publish("sf/fanState", "OFF");
        fanState_lcd = 0;
      }

      if (new_mois_value < 20)
      {
        digitalWrite(relay2Pin, 0);
        mqttClient.publish("sf/pumpState", "ON"); 
        pumpState_lcd = 1;
      }
      else
      {
        digitalWrite(relay2Pin, 1);
        mqttClient.publish("sf/pumpState", "OFF"); 
        pumpState_lcd = 0;
      }
    }

    delay(2000);

    // lcd state *****************************************************************************************************
    lcd.setCursor(0, 0); 
    lcd.print("                ");
    lcd.setCursor(0, 1); 
    lcd.print("                ");

    if (autoState_lcd)
    {
      lcd.setCursor(0, 0); 
      lcd.print("Auto: ON");
    }
    else
    {
      lcd.setCursor(0, 0); 
      lcd.print("Auto: OFF");
    }    
    if (fanState_lcd)
    {
      lcd.setCursor(0, 1);
      lcd.print("Fan:ON  ");
    }
    else
    {
      lcd.setCursor(0, 1);
      lcd.print("Fan:OFF ");
    } 
    if (pumpState_lcd)
    {
      lcd.setCursor(8, 1);
      lcd.print("Pump:ON ");
    }
    else
    {
      lcd.setCursor(8, 1);
      lcd.print("Pump:OFF");
    } 
  }

  delay(2000);
}

void setup_wifi() 
{
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length)
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) 
  {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '0') 
  {
    autoState = 0;
    autoState_lcd = 0;  
    fanState_lcd = 0;
    pumpState_lcd = 0;
    mqttClient.publish("sf/autoState", "OFF");
    mqttClient.publish("sf/fanState", "OFF");
    mqttClient.publish("sf/pumpState", "OFF");
    digitalWrite(relay1Pin, 1);
    digitalWrite(relay2Pin, 1);
    digitalWrite(D5, 1);
  } 
  else if ((char)payload[0] == '1')  
  {
    autoState = 1;
    autoState_lcd = 1;
    mqttClient.publish("sf/autoState", "ON");
    digitalWrite(D5, 0);
  } 
  else if ((char)payload[0] == '2')  
  {
    btn2State = 0;
    fanState_lcd = 1;
    digitalWrite(relay1Pin, 0);
    mqttClient.publish("sf/fanState", "ON"); 
  } 
  else if ((char)payload[0] == '3')  
  {
    btn2State = 1;
    fanState_lcd = 0;
    digitalWrite(relay1Pin, 1); 
    mqttClient.publish("sf/fanState", "OFF"); 
  }
  else if ((char)payload[0] == '4')  
  {
    btn3State = 0;
    pumpState_lcd = 1;
    digitalWrite(relay2Pin, 0); 
    mqttClient.publish("sf/pumpState", "ON"); 
  } 
  else if ((char)payload[0] == '5')  
  {
    btn3State = 1;
    pumpState_lcd = 0;
    digitalWrite(relay2Pin, 1); 
    mqttClient.publish("sf/pumpState", "OFF"); 
  }

}

void reconnect()
{
  // Loop until we're reconnected
  while (!mqttClient.connected()) 
  {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (mqttClient.connect(clientId.c_str())) 
    {
      Serial.println("connected");
      // Once connected, publish an announcement...
      mqttClient.publish("sf/control", "hello world");
      // ... and resubscribe
      mqttClient.subscribe("sf/control");
    } 
    else 
    {
      Serial.print("failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}