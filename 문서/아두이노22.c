#define CLK 5
#define DATA 4
#define LATCH 6

#define ROW_0 12

char dot_data = 0x23;
unsigned char num[8] = 
  {0x00, 0x38, 0x44, 0x04, 0x08, 0x10, 0x20, 0x7c};
void setup() {
  for(int i = 0; i < 3; i++) pinMode(4 + i, OUTPUT);
  for(int i = 0; i < 8; i++) pinMode(ROW_0+i, OUTPUT); // ROW INITIALZATION
  for(int i = 0; i < 8; i++) {
    digitalWrite(ROW_0 + i, LOW); // ALL ROW OFF
  }
  digitalWrite(ROW_0, HIGH);
}

int idx = 0;

void loop() {
  for(int i = 0; i < 8; i++) {
    digitalWrite(ROW_0 + i, LOW); // ALL ROW OFF
  }
  
  set_column(num[idx]);  
    digitalWrite(ROW_0 + idx, HIGH); // ALL ROW OFF
  idx++;
  if(idx >= 8) idx = 0;
  delay(1);

}

void set_column(char dot_data){
  for(int i = 0; i < 8; i++){
    if(dot_data & 0x80 >> i){
      digitalWrite(DATA, LOW);
    }else{
      digitalWrite(DATA, HIGH);
    }
    digitalWrite(CLK, HIGH);
    digitalWrite(CLK, LOW);
  }
  digitalWrite(LATCH, HIGH);
  digitalWrite(LATCH, LOW);
}