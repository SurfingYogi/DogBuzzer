
#include <SPI.h>
#include <WiFiNINA.h>

// Note: You need to create secrets.h to define your ssid and password, by copying secrets.example.h".
#include "secrets.h"
#include "webpage.h"

WiFiServer server(80);

constexpr uint8_t BUZZER_PIN = 7;
constexpr unsigned int BUZZER_FREQUENCY_HZ = 25000;

// For debugging:
//constexpr unsigned int BUZZER_FREQUENCY_HZ = 10000;

// Built in LED pin 
const int outputPin = LED_BUILTIN;

// Webpage triggered activation
bool webBuzzerActive = false;           //Webpage's timed activation is running
unsigned long webBuzzerStopTime = 0;    //When the webpage activation expires

// Momentary button used to turn on the buzzer
const int buttonPin = A2;
bool buzzerRunning = false;             //Whether the 25 kHz output is actually running

void setup() {

  // Configure LED pin
  pinMode(outputPin, OUTPUT);

  // Configure buzzer pin
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);
  stopBuzzer();

  // Initialize serial port to 115.2Kb
  Serial.begin(115200);

  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Wi-Fi module not found");
    while (true) {}
  }

  Serial.print("Connecting to Wi-Fi");
  
  while (WiFi.begin(WIFI_SSID, WIFI_PASSWORD) != WL_CONNECTED) {
    Serial.print(".");
    delay(3000);
  }

  server.begin();

  Serial.println();
  Serial.print("Open http://");
  Serial.println(WiFi.localIP());
}

void updateBuzzer() {
  bool buttonPressed = (digitalRead(buttonPin) == LOW);

  // "webBuzzerActive" and "webBuzzerStopTime " are used by your web command.
  if (webBuzzerActive && (long)(millis() - webBuzzerStopTime ) >= 0) {
    webBuzzerActive = false;
  }

  bool shouldRun = buttonPressed || webBuzzerActive;

  if (shouldRun != buzzerRunning) {
    buzzerRunning = shouldRun;

    if (shouldRun) {

      // Start your existing 25 kHz waveform here.
      startBuzzer();
    } else {

      // Stop your existing 25 kHz waveform here.
      // Leave the output LOW when stopped.
      stopBuzzer();
    }
  }
}

void startBuzzer()
{

  // Turn on the LED
  digitalWrite(outputPin, HIGH);

  // Turn on the buzzer
  tone(BUZZER_PIN, BUZZER_FREQUENCY_HZ);
}

void stopBuzzer()
{

  // Turn off the LED
  digitalWrite(outputPin, LOW);

  // Turn off the buzzer
  noTone(BUZZER_PIN);
  digitalWrite(BUZZER_PIN, LOW);
}

void loop() {

  // Update the buzzer based on the physical button and web timer
  updateBuzzer();

  WiFiClient client = server.available();
  if (!client) return;

  client.setTimeout(200);
  String request = client.readStringUntil('\n');
  request.trim();

  // Drain HTTP headers, with a short timeout.
  unsigned long started = millis();
  while (client.connected() && millis() - started < 300) {

    // Keep button responsive while reading headers.
    updateBuzzer();

    if (!client.available()) continue;

    String header = client.readStringUntil('\n');
    header.trim();

    if (header.length() == 0) break;
  }

  if (request.startsWith("POST /activate?seconds=")) {
    int start = request.indexOf("seconds=") + 8;
    int seconds = request.substring(start).toInt();

    if (!webBuzzerActive && (seconds >= 1) && (seconds <= 5)) {

      // Activate the web timer
      webBuzzerActive = true;
      webBuzzerStopTime = millis() + seconds * 1000UL;

      // Start waveform immediately
      updateBuzzer();

      Serial.println("Fuck that Dawg!");
      client.println("HTTP/1.1 200 OK");
    } else {
      client.println("HTTP/1.1 409 Conflict");
    }
    client.println("Content-Length: 0");
    client.println("Connection: close");
    client.println();
  } else {

    // Serve the webpage
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html; charset=utf-8");
    client.println("Connection: close");
    client.println();

    // Serve the webpage directly from flash
    client.print(reinterpret_cast<const __FlashStringHelper*>(webpage));
  }

  client.flush();
  client.stop();
}
