#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "TOKO MBAK KUM";
const char* password = "sekawan007";
const char* serverName = "http://192.168.1.3:5000/sensor/data";

#define TRIGGER_PIN 13
#define ECHO_PIN 12

long duration;
int distance;

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
}

void loop() {
  // Trigger ultrasonik
  digitalWrite(TRIGGER_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);

  // Hitung durasi gelombang ultrasonik untuk kembali
  duration = pulseIn(ECHO_PIN, HIGH);

  // Hitung jarak berdasarkan durasi
  distance = duration * 0.034 / 2;

  // Kirim data ke server
  postData(distance);
  
  delay(1000); // Delay 1 detik sebelum pengukuran berikutnya
}

void postData(int distance) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(serverName);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    // Buat data form
    char httpRequestData[100];
    sprintf(httpRequestData, "distance=%d", distance);

    int httpResponseCode = http.POST(httpRequestData);

    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
      Serial.println(response);
    } else {
      Serial.print("Error on sending POST: ");
      Serial.println(httpResponseCode);
    }

    http.end();
  } else {
    Serial.println("Error in WiFi connection");
  }
}
