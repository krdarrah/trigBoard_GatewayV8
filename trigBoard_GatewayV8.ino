//ESP32 Gateway System from UDP

#include "includes.h"
#include "gatewayIncludes.h"//special stuff needed just for this setup, mp3 player and pins

const char fwVersion[] = "8/03/20 GATEWAY";

void setup() {
  pinMode(LEDpin, OUTPUT);
  digitalWrite(LEDpin, HIGH);
  Serial.begin(115200);
  particle.begin(9600);
  delay(500);
  loadConfiguration(filename, config);
  pinMode(0, OUTPUT);

  pinMode(silentButtonPin, INPUT_PULLUP);

  initAP();
  initBluetooth();
  bluetoothTimeoutStart = millis();
}

void loop() {

  if ((millis() - bluetoothTimeoutStart) < (60 * 5 * 1000)) {//kill the bluetooth after 5minutes from boot
    //if (!OTAsetup)
    serviceBluetooth();
    //    else
    //      checkOTA();
  } else {
    btStop();
    digitalWrite(LEDpin, LOW);
    //    if (WiFi.status() != WL_CONNECTED) {
    //      Serial.println("WiFi Disconnected on its own");
    //      delay(100);
    //      ESP.restart();
    //    }
  }

  if (strcmp(newPacket, oldPacketReceived) == 0) {//we received new packet, so will start a timer before resetting
    if (firstPacket) {
      startTimeForReset = millis();
      firstPacket = false;
    }
    if (millis() - startTimeForReset > resetTime) {
      firstPacket = true;
      startTimeForReset = millis();
      strcpy(oldPacketReceived, "");
    }

  }




  //  if (firstPacket) {//this fires everytime a new packet is received - which is noramlly a blast of 10-20 just to make sure something gets here
  //    Serial.print(millis() - firstPacketTime);
  //    Serial.println("ms");
  //    firstPacket = false;
  //    firstPacketTime = millis();
  //    trackWillBePlayed = true;
  //  }
  //  else if (trackWillBePlayed && ((millis()-firstPacketTime) > 500)) {//now after all packets are received, play the track. This keeps things from double playing
  //    Serial.print(millis() - firstPacketTime);
  //    Serial.println("ms");
  //    trackWillBePlayed = false;
  //    Serial.print(receivedPackets);
  //    Serial.println(" packets");
  //    receivedPackets = 0;
  //    playTheTrack();
  //  }

}
