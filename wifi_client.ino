#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <WiFiUdp.h>
#include <ArduinoJson.h>


#include <ESP8266HTTPClient.h>

const char* ssid = "eli server";
const char* pswd = "12345678";
const char* serverUrl = "http://55.55.55.55/data";

WiFiClient client;
int server_port = 80;  //http

void wifi_Setup() {
  Serial.println("wifiSetup");
  WiFi.begin(ssid, pswd);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.println("trying ...");
    delay(100);
  }
  Serial.println("Connected to network");
}


void SendData(String name, int val) {
  HTTPClient http;
  http.begin(client, serverUrl);
  http.addHeader("Content-Type", "application/json");

  JsonDocument data;
  data[name] = val;

  String postBody;
  serializeJson(data, postBody);

  int httpCode = http.POST(postBody);
  Serial.println(httpCode);
  Serial.println(postBody);
  http.end();
}


// int GetData() {
//   int ret = -1;
//   HTTPClient http;
//   String dataURL = "";
//   dataURL += "ACT=GET&DEV=" + String(DEV) + "&CH=" + String(CH);
//   // dataURL += "ACT=GET&DEV=1166&CH=1";
//   http.begin(client, "http://api.kits4.me/GEN/api.php?" + dataURL);
//   int httpCode = http.GET();
//   Serial.println(httpCode);
//   if (httpCode == HTTP_CODE_OK) {
//     Serial.print("HTTP response code ");
//     Serial.println(httpCode);
//     String Res = http.getString();
//     Serial.println(Res);
//     ret = Res.toInt();
//   }
//   http.end();

//   return ret;
// }



// }