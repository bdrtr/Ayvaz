
void receiver() {
  Serial3.readBytes(buffer,30);
  Serial3.flush();
}
