
void copyA(uint8_t* src, uint8_t* dst, uint8_t len) {
    memcpy(dst, src, sizeof(src[0])*len);
}

void receiver() {
  Serial.print("gel--");
  Serial3.readBytes(buffer,30);
  Serial3.flush();
  delay(200);
  
  for (int i=0;i<30;i++) {
    Serial.println(buffer[i]);
  }
  
  //Serial.println(buffer[0]);
  Serial.print("git--");
  

    //Serial.flush();
  
}
