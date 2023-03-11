void receiver() {
  Serial3.readBytes(buffer,22); //26 tane veri alınır ilk 25 index'e yazılır
  Serial3.flush(); // porta gelen diğer veriler temizlenir 
}
