#include "AsyncUDP.h"
#include <WiFiClient.h>
AsyncUDP udp;
WiFiServer server(80);


//constants
const int resetTime = 1000;//time(ms) used to reset the packet in case the same door sends the same message


#include <SoftwareSerial.h>//for sending data to particle
const int particleRX = 33;
const int particleTX = 32;
SoftwareSerial particle(particleRX, particleTX);//rx tx
//this compiled after installing EspSoftwareSerial library


//functions
void initAP();

//globals
unsigned long bluetoothTimeoutStart;

unsigned long startTimeForReset;
char oldPacketReceived[200];
char newPacket[200];

int receivedPackets = 0;
boolean firstPacket = true;
