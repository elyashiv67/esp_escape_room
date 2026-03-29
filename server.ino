#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>

int numberToDisplay = 0;
IPAddress apIP(55,55,55,55);
const char* ssid = "eli server";
const char* password = "12345678";
ESP8266WebServer server(80);

void wifi_setup() {
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP(ssid, password);

  server.on("/", handleRoot);
  server.on("/data",HTTP_POST ,handleGetData);
  server.on("/unlock", HTTP_POST, handleUnlock);
  server.onNotFound(handleNotFound);
  server.begin();

  Serial.begin(9600);
  Serial.println();
  Serial.print("Access Point Name: ");
  Serial.println(ssid);
  Serial.print("AP IP address: ");
  Serial.println(WiFi.softAPIP());  
}

void handleNotFound() {
  String message = "File Not Found \n \n";
  message += "URI: " + server.uri() + "\n";
  message += "Method: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: " + String(server.args()) + "\n";

  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }

  server.send(404, "text/plain", message);
}

void updateDisplay() {
  byte charToDisplay;
  
  if (currentDigit < revealedDigits) {
    charToDisplay = digitMap[secretCode[currentDigit]];
  } else {
    charToDisplay = digitMap[0]; 
  }

 
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, 0x00); 
  shiftOut(dataPin, clockPin, MSBFIRST, 0x00); 
  digitalWrite(latchPin, HIGH);
  

  digitalWrite(latchPin, LOW);

  shiftOut(dataPin, clockPin, MSBFIRST, ~charToDisplay); 
  shiftOut(dataPin, clockPin, MSBFIRST, digitSelect[currentDigit]);

  digitalWrite(latchPin, HIGH);

  delay(2);

  currentDigit++;
  if (currentDigit > 3) {
    currentDigit = 0;
  }
}


void handleRoot() {
  String html = R"rawliteral(
    <!DOCTYPE html>
    <html dir="rtl">
    <head>
      <meta charset="UTF-8">
      <meta name="viewport" content="width=device-width, initial-scale=1.0">
      <title>חדר בריכה - מערכת נעילה</title>
      <style>
        body { font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif; background-color: #121212; color: #ffffff; display: flex; justify-content: center; align-items: center; height: 100vh; margin: 0; text-align: center; }
        .container { background-color: #1e1e1e; padding: 40px; border-radius: 15px; box-shadow: 0 8px 16px rgba(0, 0, 0, 0.5); width: 90%; max-width: 400px; }
        h1 { color: #00bcd4; margin-bottom: 20px; }
        p { font-size: 1.1em; margin-bottom: 30px; }
        input[type="text"] { width: 80%; padding: 15px; font-size: 24px; text-align: center; letter-spacing: 10px; border: 2px solid #00bcd4; border-radius: 8px; background-color: #2c2c2c; color: #ffffff; outline: none; margin-bottom: 20px; direction: ltr; }
        input[type="submit"] { background-color: #00bcd4; color: #121212; border: none; padding: 15px 30px; font-size: 18px; font-weight: bold; border-radius: 8px; cursor: pointer; transition: background-color 0.3s ease; }
        input[type="submit"]:hover { background-color: #0097a7; }
      </style>
    </head>
    <body>
      <div class="container">
        <h1>מערכת נעילה חכמה</h1>
        <p>הזן את הקוד בן 4 הספרות שגילית במהלך החידות:</p>
        <form action="/unlock" method="POST">
          <input type="text" name="code" maxlength="4" pattern="\d{4}" autocomplete="off" required>
          <br>
          <input type="submit" value="פתח דלת">
        </form>
      </div>
    </body>
    </html>
  )rawliteral";
  
  server.send(200, "text/html", html);
}

void handleGetData() {
  if (server.hasArg("plain")) {
    String d = server.arg("plain");
    JsonDocument val;
    DeserializationError error = deserializeJson(val, d);

    if (!error) {
      int s = val["riddle"];
      Serial.print("Received number: ");
      Serial.println(s);
      
      if (s >= 0 && s <= 4) {
        revealedDigits = s; 
      }
      
      server.send(200, "text/plain", "ok");
    } else {
      server.send(400, "text/plain", "JSON Error");
    }
  }
}
  
void handleUnlock() {
  if (server.hasArg("code")) {
    String enteredCode = server.arg("code");
    
    
    String correctCode = String(secretCode[0]) + String(secretCode[1]) + 
                         String(secretCode[2]) + String(secretCode[3]);

    if (enteredCode == correctCode) {
     
      isDoorOpen = true;
      unlockStartTime = millis(); 
      digitalWrite(relayPin, HIGH); 

     
      String successHtml = "<html dir='rtl'><head><meta charset='UTF-8'><meta name='viewport' content='width=device-width, initial-scale=1.0'></head><body style='background:#121212; color:#00FF00; font-family:sans-serif; text-align:center; padding-top:20%;'><h1>הדלת נפתחה בהצלחה! 🎉</h1><p style='color:#fff;'>תוכלו לצאת מהחדר.</p></body></html>";
      server.send(200, "text/html", successHtml);
      
    } else {
      
      String errorHtml = "<html dir='rtl'><head><meta charset='UTF-8'><meta name='viewport' content='width=device-width, initial-scale=1.0'></head><body style='background:#121212; color:#FF0000; font-family:sans-serif; text-align:center; padding-top:20%;'><h1>קוד שגוי!</h1><br><a href='/' style='color:#00bcd4; font-size:24px; text-decoration:none;'>נסה שוב</a></body></html>";
      server.send(401, "text/html", errorHtml);
    }
  }
}

void wifi_loop() {
  server.handleClient();
}
