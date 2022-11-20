/*
void receiver() {
  int value;
  receive_data = Serial2.read();
  if (receive_data == 99) {
    MSG[0] = receive_data;
    for (int i = 1; i < 27; i++) {
      value = Serial2.read();

      MSG[i] = value;
      Serial.print(i);
      Serial.print("- ");
      Serial.println(MSG[i]);
    }
  }
}
*/
void copyA(int* src, int* dst, int len) {
    memcpy(dst, src, sizeof(src[0])*len);
}

void receiver() {
  int MSG2[27];

  int i=0,value;
    for (;i<27;i++) {
    receive_data = Serial2.read();
    if (receive_data==99 && i!=0) {
      for (;i<27;i++) {
        MSG[i] = MSG2[i];
        Serial.print(i);
        Serial.print("- ");
        Serial.println(MSG[i]);
      } 
      i=0;
    }

    MSG[i] = receive_data;
    Serial.print(i);
    Serial.print("- ");
    Serial.println(MSG[i]);
  }
   copyA(MSG,MSG2,sizeof(MSG));
}
