// Arduino IDE uart receive thread https://forum.arduino.cc/index.php?topic=288234.0
// Keywords for uart commands
// Commnand descreption on: https://github.com/Kl0ibi/AirHockeyRobot/blob/main/Uart%20Commands
// To use Uart correctly, check to config Both NL&CR and 115200 baud
char home_keyword[]  = "home";
char demo_keyword[]  = "demo";
char cyclic_test_keyword[] = "cyclic_test";
char position_keyword[]  = "p";
char velocity_keyword[]  = "v";
char set_maxspeed_keyword[]  = "vmax";
char default_keyword[]  = "default";
char preventwallhit_keyword[]  = "preventwallhit";
char led_keyword[] = "leds";
char blink_keyword[] = "blink";
char fan[] = "fans";
char solenoid[] = "solenoid";
char recievedChar;
char * strtokIndx;
char buf[20];


void checkSerialInput() {
  strcpy(buf, "");
  while (Serial.available())  {
    readline(Serial.read(), buf, 80);
  }
  if ((buf[0] != NULL)) {

    strtokIndx  = strtok(buf, ",");
    //Homing
    if (strcmp(strtokIndx, home_keyword) == 0) {

      doHoming();

    }
    //Postion Control
    else if (strcmp(strtokIndx, position_keyword) == 0) { // set new desired position

      strtokIndx = strtok(NULL, ","); //parse same strtokIndx
      float value0 = atof(strtokIndx);  //convert string to integer
      strtokIndx = strtok(NULL, ","); //parse same strtokIndx
      float value1 = atof(strtokIndx);
      waitUntilPosReached(value0, value1);
    }
    //demo
    else if (strcmp(strtokIndx, demo_keyword) == 0) {
      //fans, solenoid, pushermove, leds
      demo();
    }//set your own speed
    else if (strcmp(strtokIndx, velocity_keyword) == 0) {
      //setSpeed
      strtokIndx = strtok(NULL, ","); //parse same strtokIndx
      float value0 = atof(strtokIndx);  //convert string to integer
      setDesiredSpeed(value0);
    }
    //set max speed
    else if (strcmp(strtokIndx, set_maxspeed_keyword) == 0) {
      setMaximalSpeed();
    }
    //cycle test
    else if (strcmp(strtokIndx, cyclic_test_keyword) == 0) {
      //move to every corner
      cycleTest();
    }
    //set default params
    else if (strcmp(strtokIndx, default_keyword) == 0) {
      setDefaultParams1();
    }
    //tells if there will be a wallhit
    else if (strcmp(strtokIndx, preventwallhit_keyword) == 0) {

    }
    //push solenoid
    else if (strcmp(strtokIndx, solenoid) == 0) {
      Serial.println("Solenoid pushed");
      pushSolenoid();
    }
    //activate fans
    else if (strcmp(strtokIndx, fan) == 0) {
      Serial.println("Toggled fans");
      fans();
    }
    //turn leds on
    else if (strcmp(strtokIndx, led_keyword) == 0) {
      Serial.println("Toggled Leds");
    }
    //blink dedicated leds
    else if (strcmp(strtokIndx, blink_keyword) == 0) {
      Serial.println("blink");
    }
    else {
      Serial.println("Unknown Command: " + String(strtokIndx));
    }

  }
}



int readline(int readch, char *buffer, int len) {
  static int pos = 0;
  int rpos;
  if (readch > 0) {
    switch (readch) {
      default:
        if (pos < len - 1) {
          buffer[pos++] = readch;
          buffer[pos] = 0;
        }
      case '\r': // Ignore CR
        break;
      case '\n': // Return on new-line
        rpos = pos;
        pos = 0;  // Reset position index ready for next time
        return rpos;

    }
  }
  return 0;
}
