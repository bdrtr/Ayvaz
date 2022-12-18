
void copyA(int* src, int* dst, int len) {
    memcpy(dst, src, sizeof(src[0])*len);
}

void receiver() {
  int MSG2[27];

  int i=0;
    
    receive_data = Serial3.read();
    if (i==0) {
      for (;i<27;i++) {
        receive_data = Serial3.read();
        MSG[i] = receive_data;
      } 
    }

    /*
    MSG[i] = receive_data;
    Serial.print(i);
    Serial.print("- ");
    Serial.println(MSG[i]);
  
   copyA(MSG,MSG2,sizeof(MSG));
   */
}
