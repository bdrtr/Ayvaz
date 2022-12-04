
void copyA(int* src, int* dst, int len) {
    memcpy(dst, src, sizeof(src[0])*len);
}

void receiver() {
  int MSG2[27];

  int i=0,value;
    for (;i<27;i++) {
    receive_data = Serial2.read();
    MSG[i]=receive_data;
    Serial.print(i);
    Serial.print("- ");
    Serial.println(MSG[i]);
    }    
    /*
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
  */
  
   //copyA(MSG,MSG2,sizeof(MSG));
}
