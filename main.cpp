#include <WiFi.h>
#include <ESPAsyncWebServer.h>

#define RADAR_PIN 34

const char* ssid = "ESP_MARAUDER";
const char* password = "12345678";

AsyncWebServer server(80);

void setup() {
  Serial.begin(115200);
  pinMode(RADAR_PIN, INPUT);

  WiFi.softAP(ssid, password);
  Serial.println("WiFi AP Started. IP: " + WiFi.softAPIP().toString());

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/index.html", "text/html");
  });

  server.on("/camera", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/camera.html", "text/html");
  });

  server.on("/radar", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/radar.html", "text/html");
  });

  server.on("/radar-status", HTTP_GET, [](AsyncWebServerRequest *request) {
    bool detected = digitalRead(RADAR_PIN);
    request->send(200, "text/plain", detected ? "Human Detected!" : "No Motion");
  });

  server.on("/capture", HTTP_GET, [](AsyncWebServerRequest *request) {
    // This placeholder would trigger camera capture.
    // OV7670 capture must be handled in a real implementation.
    request->send(200, "image/jpeg", "Placeholder for captured image");
  });

  if (!SPIFFS.begin(true)) {
    Serial.println("SPIFFS Mount Failed");
    return;
  }

  server.begin();
}

void loop() {}
