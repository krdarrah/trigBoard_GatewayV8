

void initAP() {
  Serial.println(WiFi.softAP(config.ssid, config.pw, 1, 1, 8)); //ssid,pw,ch,hid,conn
  //pw at least 8 char
  Serial.print("AP will be ");
  Serial.println(config.ssid);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  server.begin();



  if (udp.listen(config.udpPort)) {
    //    Serial.print("UDP Listening on IP: ");
    //    Serial.println(WiFi.localIP());
    udp.onPacket([](AsyncUDPPacket packet) {// all data comes in here
      digitalWrite(0, HIGH);

      for (int i = 0; i < packet.length(); i++) {
        newPacket[i] = char(packet.data()[i]);
        newPacket[i + 1] = '#';
        newPacket[i + 2] = NULL;
      }


      if (strcmp(newPacket, oldPacketReceived) != 0) {//don't match, so must be new
        strcpy(oldPacketReceived, newPacket);
        receivedPackets = 0;
        firstPacket = true;

        //        //Good debug info
        //        Serial.print("UDP Packet Type: ");
        //        Serial.print(packet.isBroadcast() ? "Broadcast" : packet.isMulticast() ? "Multicast" : "Unicast");
        //        Serial.print(", From: ");
        //        Serial.print(packet.remoteIP());
        //        Serial.print(":");
        //        Serial.print(packet.remotePort());
        //        Serial.print(", To: ");
        //        Serial.print(packet.localIP());
        //        Serial.print(":");
        //        Serial.print(packet.localPort());
        //        Serial.print(", Data: ");

        Serial.println(newPacket);


        //done with cellular, now sniff out for -#-
        const char delimiter[] = "-";

        char *pointerToFoundDash = strstr(newPacket, delimiter);//go find dash
        if (pointerToFoundDash != NULL) {//found a dash
          char parsedStrings[3][50];
          char *token =  strtok(newPacket, delimiter);

          strncpy(parsedStrings[0], token, sizeof(parsedStrings[0]));//first one

          token =  strtok(NULL, delimiter);
          strncpy(parsedStrings[1], token, sizeof(parsedStrings[1]));//second half - if exists
          token =  strtok(NULL, delimiter);
          strncpy(parsedStrings[2], token, sizeof(parsedStrings[1]));//last part

          //now going to recombine two strings
          strcat(parsedStrings[0], parsedStrings[2]);
          particle.print(parsedStrings[0]);
          //Serial.println(parsedStrings[1]);//found something
          int trackNumberToPlay = atoi(parsedStrings[1]);

          WiFiUDP udpTX;
          if (trackNumberToPlay > 0 && trackNumberToPlay < 256) {
            IPAddress broadcastIP(255, 255, 255, 255);
            for (int i = 0; i < 10; i++) {
              if (udpTX.beginPacket(broadcastIP , 1234) == 1) {
                udpTX.printf("%i", trackNumberToPlay);
                udpTX.endPacket();
              }
              delay(10);
            }
          }
        } else {
          particle.print(newPacket);
        }

      } else {
        receivedPackets++;
        Serial.println(receivedPackets);
      }
      digitalWrite(0, LOW);
    });
  }



}
