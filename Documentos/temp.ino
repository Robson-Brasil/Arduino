void irSend(const String &data) {
  // Send IR command
  // Ex.: nec&20DF40BF&32&1
  //      (protocol&data&bits&times)
  //      raw&3202+9816+560+1518+536+490+540+490+544+490+540+1532+540+490+540+490+544+490+566+1518+536+1562+536+504+536+504+536+490+540+494+540+508+540+490+540+490+540+490+540+490+544+486+544+490+566+1496+540+492+544+1532+540+490+540+490+540+490+544+1522+540&38
  //      (raw&d1+d2+d3...+dn&kHz)
  int p = data.indexOf("&");
  int pp = data.indexOf("&", p + 1);
  String type = data.substring(0, p);
  type.toLowerCase();
  DEBUG("type= [" + type + "]");
  String s = data.substring(p + 1, pp) + "+";
  DEBUG("data= [" + s + "]");
  if (type == "raw") {
    // Send RAW
    uint16_t code[255];
    int f = 0;
    int t = s.indexOf("+");
    int i = 0;
    while (t != -1) {
      code[i] = s.substring(f, t).toInt();
      DEBUG("code " + String(i) + "\t"  + String(code[i]));
      i++;
      f = t + 1;
      t = s.indexOf("+", f);
    }
    DEBUG("kHz: " + String(data.substring(pp + 1).toInt()));
    log("Sending raw code, " + String(i) + " timings");
    irSnd.sendRaw(code, i, data.substring(pp + 1).toInt());
  } else {
    // Other types
    p = data.substring(pp + 1).toInt();
    DEBUG("bits " + String(data.substring(pp + 1).toInt()));

    pp = data.indexOf("&", pp + 1) + 1;
    DEBUG("substr: [" + data.substring(pp) + "]");
    DEBUG("times " + String(data.substring(pp).toInt()));
    pp = data.substring(pp).toInt();
    if (pp == 0) {
      pp = 1;
    }
    DEBUG("times corrected " + String(pp));
    log("Sending " + type + " code, [" + s + "] X " + String(pp));
    for (int b = 0; b < pp; b++) {
      // Send command
      int f = 0;
      int t = s.indexOf("+");
      do {
        String codeStr = s.substring(f, t);
        DEBUG("codeStr= " + codeStr);
        unsigned long code = strtoul(codeStr.c_str(), NULL, 16);
        if (type == "nec") {
          irSnd.sendNEC(code, p);
        } else if (type == "sony") {
          irSnd.sendSony(code, p);
        } else if (type == "rc5") {
          irSnd.sendRC5(code, p);
        } else if (type == "rc6") {
          irSnd.sendRC6(code, p);
        } else if (type == "dish") {
          irSnd.sendDISH(code, p);
        } else if (type == "sharp") {
          irSnd.sendSharp(code, p);
        } else if (type == "jvc") {
          irSnd.sendJVC(code, p, false);
        } else if (type == "samsung") {
          irSnd.sendSAMSUNG(code, p);
        } else if (type == "lg") {
          irSnd.sendLG(code, p);
        } else if (type == "whynter") {
          irSnd.sendWhynter(code, p);
        } else if (type == "aiwa_rc_t501") {
          irSnd.sendAiwaRCT501(code);
        } else if (type == "panasonic") {
          irSnd.sendPanasonic(code, p);
        } else if (type == "denon") {
          irSnd.sendDenon(code, p);
        } else {
          // Invalid IR type
          log("Invalid IR type");
        }

        // Process Broker connection
        if (client.connected()) {
          client.loop();
        } else {
          reconnect();
        }

        delay(400);
        f = t + 1;
        t = s.indexOf("+", f);
      } while (t != -1);
      DEBUG("Send " + String(b));
    }
  }
}
