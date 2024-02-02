#include <CoreX.h>
#include "Connection.h"
#include <Servo.h>

const char* AUTH_TOKEN = "eCSlztnMe5akxs9";
const char* DEVICE_ID = "9K9ktqpAQ";

const char ssid[] = "HUAWEI-uX8s";
const char pass[] = "vAk3hwC4";

Servo servo1;
Servo servo2;
CoreXTimer timer;

#define trigPin 21
#define echoPin 19
#define ledPin1 15
#define ledPin2 2
const int servoPin1 = 18;
const int servoPin2 = 5;

long durasi;
int jarak;

bool statusLed1 = false;
bool statusLed2 = false;

void send() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  durasi = pulseIn(echoPin, HIGH);
  jarak = durasi * 0.0343 / 2;

  corex.send("level_air", jarak);
}

void receive(String& bucket, String& data) {
  Serial.println(String() + "bucket: " + bucket + " data: " + data);
  if (bucket == "servo1") {  // kontrol servo1
    servo1.write(data.toInt());
    if (data.toInt() >= 90) {
      statusLed1 = true;
    } else {
      statusLed1 = false;
    }
  }

  if (bucket == "servo2") {  // kontrol servo2
    servo2.write(data.toInt());
    if (data.toInt() >= 90) {
      statusLed2 = true;
    } else {
      statusLed2 = false;
    }
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(trigPin, OUTPUT);  // suara dikeluarkan
  pinMode(echoPin, INPUT);   // suara diterima

  digitalWrite(ledPin1, LOW);
  digitalWrite(ledPin2, LOW);
  // servo1.attach(servoPin, 500, 2400);
  servo1.attach(servoPin1);
  servo2.attach(servoPin2);
  WiFi.begin(ssid, pass);
  // corex.begin(server, net);
  corex.begin(net);

  corex.onMessage(receive);       // terima data dari coreX
  timer.setInterval(1000, send);  // kiirm data ke corex
  setupCoreX();
}

void loop() {
  corex.loop();
  timer.run();
  delay(10);

  if (!corex.connected()) {
    setupCoreX();
  }

  //==LETAKAN KODE PROGRAM DISINI UNTUK DILAKUKAN PROSES==//
  if (statusLed1 == true) {
    digitalWrite(ledPin1, HIGH);
  } else {
    digitalWrite(ledPin1, LOW);
  }
  if (statusLed2 == true) {
    digitalWrite(ledPin2, HIGH);
  } else {
    digitalWrite(ledPin2, LOW);
  }
}