void initMonitorUDP() {

  if (udp.listen(config.udpPort)) {
    Serial.print("UDP Listening on IP: ");
    Serial.println(WiFi.localIP());
    udp.onPacket([](AsyncUDPPacket packet) {// all data comes in here
      for (int i = 0; i < packet.length(); i++) {
        newPacket[i] = char(packet.data()[i]);
        newPacket[i + 1] = NULL;
      }
      receivedPackets++;
      firstPacket = true;
     // firstPacketTime = millis();
    });
  }
}
