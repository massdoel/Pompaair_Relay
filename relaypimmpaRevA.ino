
#define relay 12
#define LRun 13
 int pumpOn=7200; //on pompa selama 2 jam -> 7200detik
 int pumpOff=3600;
 //10800;
 int dej=20; //delay relay on 20 detik
 int penghitung;


 bool doneStartup;

 long int eksekusiDetik;

void setup() {
 // put your setup code here, to run once:
 pinMode(relay, OUTPUT);
 pinMode(LRun, OUTPUT);
 pinMode(9, OUTPUT);
 pinMode(8, INPUT);
 eksekusiDetik = millis();
 penghitung =0;
 doneStartup=LOW;
}

void loop() {
  // put your main code here, to run repeatedly:
  
  //----eksekusi detik
  if (millis() > eksekusiDetik)
  {
  eksekusiDetik = millis()+1000;
  penghitung ++;
  if(penghitung>=10800) penghitung=0;

  if(digitalRead(relay)==HIGH)
  {
  digitalWrite(LRun,!digitalRead(LRun));
  digitalWrite(9,LOW);
  }
  else
  {
  digitalWrite(LRun,LOW);
  digitalWrite(9,HIGH);
  }
  
  if(digitalRead(8)==HIGH) penghitung=penghitung+200;
  
  }

  if (penghitung>=20 && doneStartup==LOW)
  {
    digitalWrite(relay, HIGH);
    doneStartup=HIGH;
  }

  //continue pump on off
  // pump on
   if (penghitung>=pumpOn && doneStartup==HIGH) 
  {
    digitalWrite(relay, LOW);    
  }
  else
  {
    digitalWrite(relay, HIGH); 
  }


}
