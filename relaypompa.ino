#define relay 12
 int dep=7200000; //on pompa selama 2 jam
 int dej=20000; //delay relay on 20 detik

void setup() {
 // put your setup code here, to run once:
 pinMode(relay, OUTPUT);
 

}

void loop() {
  // put your main code here, to run repeatedly:
digitalWrite(relay, HIGH);
  delay(dej);
}
