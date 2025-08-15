#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Servo.h>

// Ganti dengan WiFi kamu 1921681.9
const char *ssid = "Mancing-Mania";
const char *password = "mantap123";

ESP8266WebServer server(80);
Servo myServo;

const int servoPin = D4; // GPIO2, bisa juga D1/D5 tergantung wiring

void setup()
{
  Serial.begin(115200);

  // Sambungkan ke WiFi
  WiFi.begin(ssid, password);
  Serial.print("Menghubungkan ke WiFi");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  // Tampilkan IP NodeMCU
  Serial.println();
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Setup Servo
  myServo.attach(servoPin);
  myServo.write(90); // posisi tengah saat awal

  // Endpoint: /servo?pos=l / c / r
  server.on("/servo", []()
            {
    if (server.hasArg("pos")) {
      String pos = server.arg("pos");

      if (pos == "l") {
                myServo.write(0);
        server.send(200, "text/plain", "Servo ke KIRI (0°)");
      } else if (pos == "c") {
        myServo.write(90);
        server.send(200, "text/plain", "Servo ke TENGAH (90°)");
      } else if (pos == "r") {
        myServo.write(180);
        server.send(200, "text/plain", "Servo ke TENGAH (90°)");
      } else if (pos == "gift") {
        myServo.write(180);
        delay(1000);
        myServo.write(0);
        delay(1000);
        myServo.write(90);
        server.send(200, "text/plain", "Servo gerak karena gift TikTok");
      } else {
        server.send(400, "text/plain", "Posisi tidak dikenali");
      }

    } else {
      server.send(400, "text/plain", "Parameter 'pos' tidak ditemukan");
    } });

  server.begin();
  Serial.println("Web server aktif");
}

void loop()
{
  server.handleClient();
}
