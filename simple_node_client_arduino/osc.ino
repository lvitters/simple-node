void onPacketOSC(AsyncUDPPacket packet) {

  OSCMessage msgIn;
  if ((packet.length() > 0)) {
    msgIn.fill(packet.data(), packet.length());
    packet.flush();
    if (!msgIn.hasError()) {
      msgIn.route("/command1", OSCcommand1);
      msgIn.route("/command2", OSCcommand2);
      msgIn.route("/updatefirmware", OSCupdateFirmware);
      msgIn.route("/ufversionurl", OSCupdateFirmwareSetVersionURL);
      msgIn.route("/ufbinaryurl", OSCupdateFirmwareSetBinaryURL);

      packet.flush();
    }
  }
  packet.flush();
}

void OSCcommand1(OSCMessage &msg, int addrOffset) {
  Serial.print("received parapmeters: ");
  Serial.println( msg.size());
  if (msg.size() == 2) {
    Serial.print(msg.getInt(0));
    Serial.print(" ");
    Serial.print(msg.getInt(1));
  }
}

void OSCcommand2(OSCMessage &msg, int addrOffset) {
  Serial.print("received parapmeters: ");
  Serial.println( msg.size());
  if (msg.size() == 5) {
    Serial.print(msg.getDouble(0));
    Serial.print(" ");
    Serial.print(msg.getDouble(1));
    Serial.print(" ");
    Serial.println(msg.getDouble(2));
  }
}

 
void OSCupdateFirmware(OSCMessage &msg, int addrOffset) {
  UPDATE_FIRMWARE = true; // set the hook for the main loooooop
}

void OSCupdateFirmwareSetVersionURL(OSCMessage &msg, int addrOffset) {
  Serial.print(">> /ufversionurl ");
  char tmpstr[512];
  int retlength = msg.getString(0, tmpstr, 512);
  Serial.print(tmpstr);
  Serial.print(" ( ");
  Serial.print(retlength);
  Serial.println(" )");
  if (retlength > 20) {
    strncpy(URL_FW_VERSION, tmpstr, retlength);
    Serial.print("new URL_FW_VERSION = ");
    Serial.println(URL_FW_VERSION);
  }
}

void OSCupdateFirmwareSetBinaryURL(OSCMessage &msg, int addrOffset) {
  Serial.print(">> /ufbinaryurl ");
  char tmpstr[512];
  int retlength = msg.getString(0, tmpstr, 512); // xxx.xxx.xxx.xxx = 15 + string ende char = 16
  Serial.print(tmpstr);
  Serial.print(" ( ");
  Serial.print(retlength);
  Serial.println(" )");
  if (retlength > 20) {
    strncpy(URL_FW_BINARY, tmpstr, retlength);
    Serial.print("new URL_FW_BINARY = ");
    Serial.println(URL_FW_BINARY);
  }
}


void sendPingOSC() {
  AsyncUDPMessage udpMsg;
  OSCMessage oscMsg("/ping");
  oscMsg.add(int(millis()));
  oscMsg.add(MY_NODE_ID);
  oscMsg.add(WiFi.localIP().toString().c_str());
  oscMsg.add(WiFi.macAddress().c_str());
  oscMsg.add(FW_VERSION);
  oscMsg.add(123); // send some other data
  oscMsg.send(udpMsg);
  oscMsg.empty();
  udpOut.broadcastTo(udpMsg, networkOutPort);

}
