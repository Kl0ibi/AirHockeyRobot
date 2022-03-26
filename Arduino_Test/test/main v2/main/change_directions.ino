void changeDirection0()  {
  //noInterrupts();
  //Serial.println("changing direction of motor 0");
  //Serial.println(micros());
  //changing_dir[0] = true;
  //TCNT3 = 0;
  PORT_DIR_L ^= (1 << PIN_DIR_L);
  delayMicroseconds(5); 
  //Serial.println(micros());
  //changing_dir[0] = false;  
  //interrupts();
}

void changeDirection1() {
  //noInterrupts();
  //Serial.println("changing direction of motor 1");
  //changing_dir[1] = true;
  //TCNT1 = 0;
  PORT_DIR_R ^= (1 << PIN_DIR_R);
  delayMicroseconds(5);  
  //changing_dir[1] = false;  
  //interrupts();
}
