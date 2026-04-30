//ref SmartHomeRakata20190313_1605
//20200420
//edit eeprom/ di case , kembali ke awal, write di subroutine


//20200421 module 4di 4do

//PompaPosMusafir_MIM20200425_modifon1015
//20200427 pompa transfer tokyo R2


#include <EEPROM.h>
#include <Wire.h> //I2C PROTOCOL
#include <DS1307.h> //RTC DS1307 . DS3231
#include <LiquidCrystal_I2C.h> //LCD
LiquidCrystal_I2C lcd(0x3F, 16, 2);//x027 ,0x3F(kuning)


//variable data RTC , LCD
int tahun;  
byte bLightLCD,regset,jame, menite, detike, bulan,tgl,tahunx,hari;

//variable array hari
String dino[8]={" ",
                "Ahad",
                "Senin",
                "Selasa",
                "Rabu",
                "Kamis",
                "Jum'at",
                "Sabtu",};

//----------------------------------------------00------------
// variable Relay
boolean regset_jamON_R1,regset_menitON_R1;
boolean regset_jamOFF_R1,regset_menitOFF_R1;
byte jamON_R1,menitON_R1;
byte jamOFF_R1,menitOFF_R1;
int start_R1;
int stop_R1;

boolean jamON_R1_0407;
boolean jamON_R1_0811;
boolean jamON_R1_1215;
boolean jamON_R1_1619;
boolean jamON_R1_2023;
boolean jamON_R1_0003;

boolean jamON_R1_1015; //20200425

boolean regset_jamON_R2,regset_menitON_R2;
boolean regset_jamOFF_R2,regset_menitOFF_R2;
byte jamON_R2,menitON_R2;
byte jamOFF_R2,menitOFF_R2;
int start_R2;
int stop_R2;

boolean regset_jamON_R3,regset_menitON_R3;
boolean regset_jamOFF_R3,regset_menitOFF_R3;
byte jamON_R3,menitON_R3;
byte jamOFF_R3,menitOFF_R3;
int start_R3;
int stop_R3;

boolean regset_jamON_R4,regset_menitON_R4;
boolean regset_jamOFF_R4,regset_menitOFF_R4;
byte jamON_R4,menitON_R4;
byte jamOFF_R4,menitOFF_R4;
int start_R4;
int stop_R4;
//---------------------------------------------------------
//-----------------------------------------------------------

// variable ambil data jam menit saat ini
int jamSaiki;

// variable tombol setting up down
boolean doneset, doneudset, back0x;
unsigned long udset,incset, back0;
byte done,doneSetAcc,DetikAdjust ;

// variable jeda ambil data dari rtc
unsigned long millisrtc;
byte seteldetik;

byte LS_Count; //20200419

# define LS 5  // --20200419

# define R1 12 //
# define R2 11 //
# define R3 10 //
# define R4 9 //

# define pbset 8 //5
# define pbup 7 //4
# define pbdown 6 //3

# define BZ 4 //12
# define BLINK 13

//------------------------- setup -----------------------
void setup() 
{
  pinMode(LS,INPUT); // level sensor d5 i4
  pinMode(R1,OUTPUT); //r1 d12
  pinMode(R2,OUTPUT); //r2 d11
  pinMode(R3,OUTPUT); //r3 d10
  pinMode(R4,OUTPUT); //r4 d9

  pinMode(pbset,INPUT); //PBset d8 i1
  pinMode(pbup,INPUT); //PBup d7 i2
  pinMode(pbdown,INPUT); //PBdown d6 i3
  
  pinMode(BZ,OUTPUT); //BUZZER d12->d4
  pinMode(BLINK,OUTPUT); //blink d13
  
  //digitalWrite(LS,HIGH);
      
  digitalWrite(R1,LOW);
  digitalWrite(R2,LOW);
  digitalWrite(R3,LOW);
  digitalWrite(R4,LOW);

  /*
  digitalWrite(pbset,HIGH);
  digitalWrite(pbup,HIGH);
  digitalWrite(pbdown,HIGH);
  */
   
  digitalWrite(BZ,LOW);
  digitalWrite(BLINK,HIGH);
  delay(100);
  digitalWrite(BZ,HIGH);    
  digitalWrite(BLINK,LOW);
 
  Wire.begin(); // memulai koneksi i2c dengan RTC  
  lcd.begin(); // memulai koneksi i2c dengan LCD


//--
  LS_Count=0;

  //------------------
  regset_jamON_R1=LOW;
  regset_menitON_R1=LOW;
  regset_jamOFF_R1=LOW;
  regset_menitOFF_R1=LOW;

  regset_jamON_R2=LOW;
  regset_menitON_R2=LOW;
  regset_jamOFF_R2=LOW;
  regset_menitOFF_R2=LOW;

  regset_jamON_R3=LOW;
  regset_menitON_R3=LOW;
  regset_jamOFF_R3=LOW;
  regset_menitOFF_R3=LOW;

  regset_jamON_R4=LOW;
  regset_menitON_R4=LOW;
  regset_jamOFF_R4=LOW;
  regset_menitOFF_R4=LOW;
  //------------------

  // LCD I2C 
  lcd.clear();  
  lcd.backlight();
  lcd.setCursor(0,0);

  // INIT VARIABLE
  regset = 0;
  back0x = LOW;
  back0 = millis(); 
  bLightLCD = 10; 

  millisrtc=millis();

  done = 0;
  doneSetAcc = 0;
  DetikAdjust = 1 ; //konstanta detik adjust 
 
  delay(1000);
  
    //buzzer start up..............
    digitalWrite(BZ,LOW);
    
    //---start up  display---
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print ("Starting");
    delay(750);
    digitalWrite(BZ,HIGH);     
    for(int x=9;x<16;x++)
    {lcd.setCursor(x,0); 
    digitalWrite(BZ,LOW);
    digitalWrite(BLINK,HIGH);
    delay(100);
    digitalWrite(BZ,HIGH);    
    digitalWrite(BLINK,LOW);   
    lcd.print(".");
    delay(500);}

    lcd.setCursor(0,1); 
    lcd.print("Please wait");    
    delay(1000);
    delay(1000);
    

//------------eeprom 
//------------------------- R1 --------------------------
  jamON_R1 = EEPROM.read(1);
  delay(500);
  menitON_R1 = EEPROM.read(2);
  delay(500);
  jamOFF_R1 = EEPROM.read(3);
  delay(500);
  menitOFF_R1 = EEPROM.read(4);
  delay(500);
    
  if (jamON_R1>23)
  {EEPROM.write(1,23);
  delay(500);}  
  if (menitON_R1>59)
  {EEPROM.write(2,59);
  delay(500);}  
  if (jamOFF_R1>23)
  {EEPROM.write(3,23);
  delay(500);}  
  if (menitOFF_R1>59)
  {EEPROM.write(4,59);
  delay(500);}  
//------------------------- R1 --------------------------
//------------------------- R2 --------------------------
  jamON_R2 = EEPROM.read(5);
  delay(500);
  menitON_R2 = EEPROM.read(6);
  delay(500);
  jamOFF_R2 = EEPROM.read(7);
  delay(500);
  menitOFF_R2 = EEPROM.read(8);
  delay(500);
    
  if (jamON_R2>23)
  {EEPROM.write(5,23);
  delay(500);}  
  if (menitON_R2>59)
  {EEPROM.write(6,59);
  delay(500);}  
  if (jamOFF_R2>23)
  {EEPROM.write(7,23);
  delay(500);}  
  if (menitOFF_R2>59)
  {EEPROM.write(8,59);
  delay(500);}  
//------------------------- R2 --------------------------
//------------------------- R3 --------------------------
  jamON_R3 = EEPROM.read(9);
  delay(500);
  menitON_R3 = EEPROM.read(10);
  delay(500);
  jamOFF_R3 = EEPROM.read(11);
  delay(500);
  menitOFF_R3 = EEPROM.read(12);
  delay(500);
    
  if (jamON_R3>23)
  {EEPROM.write(9,23);
  delay(500);}  
  if (menitON_R3>59)
  {EEPROM.write(10,59);
  delay(500);}  
  if (jamOFF_R3>23)
  {EEPROM.write(11,23);
  delay(500);}  
  if (menitOFF_R3>59)
  {EEPROM.write(12,59);
  delay(500);}  
//------------------------- R3 --------------------------
//------------------------- R4 --------------------------
  jamON_R4 = EEPROM.read(13);
  delay(500);
  menitON_R4 = EEPROM.read(14);
  delay(500);
  jamOFF_R4 = EEPROM.read(15);
  delay(500);
  menitOFF_R4 = EEPROM.read(16);
  delay(500);
    
  if (jamON_R4>23)
  {EEPROM.write(13,23);
  delay(500);}  
  if (menitON_R4>59)
  {EEPROM.write(14,59);
  delay(500);}  
  if (jamOFF_R4>23)
  {EEPROM.write(15,23);
  delay(500);}  
  if (menitOFF_R4>59)
  {EEPROM.write(16,59);
  delay(500);}  
//------------------------- R4 --------------------------


  digitalWrite(BZ,LOW);
  digitalWrite(BLINK,HIGH);
  delay(50);
  digitalWrite(BZ,HIGH);    
  digitalWrite(BLINK,LOW);

  digitalWrite(BZ,LOW);
  digitalWrite(BLINK,HIGH);
  delay(50);
  digitalWrite(BZ,HIGH);    
  digitalWrite(BLINK,LOW);   


//------------------------- seteldetik --------------------------  
  seteldetik = EEPROM.read(33);
  delay(500);  
  if (seteldetik>30)
  EEPROM.write(33,30);
//------------------------- seteldetik -------------------------- 
  



//------------eeprom
  digitalWrite(BZ,LOW);
  digitalWrite(BLINK,HIGH);
  delay(200);
  digitalWrite(BZ,HIGH);    
  digitalWrite(BLINK,LOW);
 
  digitalWrite(BZ,LOW);
  digitalWrite(BLINK,HIGH);
  delay(200);
  digitalWrite(BZ,HIGH);    
  digitalWrite(BLINK,LOW);
  

//------------------------ lcd ---------------------------
    lcd.clear();
    lcd.setCursor(0,0);
    //----------1234567890123456
    lcd.print ("    * IMAC *    ");
    delay(1000); 
    lcd.setCursor(0,1); 
    //----------1234567890123456
    lcd.print ("  --20200421--  ");
    delay(5000);
    delay(10000);
    lcd.clear();  
//------------------------ lcd ---------------------------
      
}






//================================================ awal main loop =======================================
void loop() {  

    //-------- rtc retrieve --------
    if(millis()>millisrtc)
    {    
    jame = RTC.get(DS1307_HR,true); //0-23
    menite = RTC.get(DS1307_MIN,false); //0-59
    detike = RTC.get(DS1307_SEC,false); //0-59    
    hari=RTC.get(DS1307_DOW,false); //1-7 ,1 ahad
    tgl=RTC.get(DS1307_DATE,false); //1-31
    bulan=RTC.get(DS1307_MTH,false); //1-12
    tahun=RTC.get(DS1307_YR,false); //0-49
    millisrtc=millis()+1000;

    if(digitalRead(LS)==HIGH)
    {
      if (LS_Count<=60) LS_Count++;
    }
    else
    {
      LS_Count=0;
    }
    
    }
    //-------- rtc retrieve --------


//------------------------------------------------------------------------------------

        if(jame == 0 && menite==0 && detike > seteldetik && doneSetAcc==0)
      { 
        RTC.stop();  
        RTC.set(DS1307_SEC,0); 
        delay(1000);
        RTC.start(); 
        doneSetAcc = 1;
      }
      
      if (menite > 1 && doneSetAcc ==1)doneSetAcc = 0;
      
        
//------------------------------------------------------------------------------------



//----------------------------------------buzzer---------------------------------------
// beep for change minute
    if(menite!=0 && detike<5 && done==0 && regset<3)
      { 
        digitalWrite(BZ,LOW);
        delay(50);
        digitalWrite(BZ,HIGH);
        done=1;
      }
// beep for change hour      
    if(menite==0 && detike<5 && done==0 && regset<3)
      {
        digitalWrite(BZ,LOW);
        delay(100);
        digitalWrite(BZ,HIGH);
        delay(50);
        digitalWrite(BZ,LOW);
        delay(100);
        digitalWrite(BZ,HIGH);
        done=1;
      }    

    if (detike>5)
    done=0;
//---------------------------------------buzzer--------------------------------


   // REFRESH LCD SETIAP JAM 00
    if(jame==0 && menite==0 && detike>20 && detike<41)
    {
      lcd.clear();
      lcd.noBacklight();
      delay(5000); 
      lcd.backlight();     
    }


    // TAMPILAN NORMAL , REGSET DIBAWAH 3
    if (regset<3 ) 
    { 
    lcd.setCursor(0,0);
    
    lcd.print (tahun);    
    lcd.print("/");    
    if(bulan>=10)
    {lcd.print (bulan);}
    else
    {lcd.print("0");
    lcd.print (bulan);}    
    lcd.print("/");    
    if(tgl>=10)
    {lcd.print (tgl);}
    else
    {lcd.print("0");
    lcd.print (tgl);}

    //level switch counting
    lcd.print (" LSC");
    if(LS_Count>=10)
    {lcd.print (LS_Count);}
    else
    {lcd.print("0");
    lcd.print (LS_Count);}
    

   //--------------------- LCD RAW 2 -----------------------
    lcd.setCursor(0,1);
    lcd.print(dino[hari]);
           
    lcd.setCursor(8,1);        
    // jam menit detik    
    if(jame>=10)
    {lcd.print (jame);}
    else
    {lcd.print("0");
    lcd.print (jame);}
    lcd.print(":");
    if(menite>=10)
    {lcd.print (menite);}
    else
    {lcd.print("0");
    lcd.print (menite);}
    lcd.print(":");    
    if(detike>=10)
    {lcd.print (detike);}
    else
    {lcd.print("0");
    lcd.print (detike);}
    } 

  
  // CEK TOMBOL
  if(doneudset == HIGH && millis() > udset)
  {doneudset = LOW;}    
  if(doneset == HIGH && millis() > incset)
  {doneset = LOW;}
  

  
  //if(analogRead(A0) <=500 && doneset == LOW)
  if(digitalRead(pbset)==HIGH && doneset == LOW)
  {
    cetit();
    bLightLCD=10;
    back0x = LOW;
    back0 = millis()+15000;
    incset = millis()+500;
    doneset = HIGH;
    if(regset>3)
    lcd.clear();
    
    regset++;   
               // 3tahun, 4bulan, 5tanggal 
               // 6hari, 
               // 7jam, 8menit, 9detik,
               
               // 10 jamON_R1
               // 11 menitON_R1
               // 12 jamOff_R1
               // 13 menitOff_R1
               
               // 14 jamON_R2
               // 15 menitON_R2
               // 16 jamOff_R2
               // 17 menitOff_R2

               // 18 jamON_R3
               // 19 menitON_R3
               // 20 jamOff_R3
               // 21 menitOff_R3 
                             
               // 22 jamON_R4
               // 23 menitON_R4
               // 24 jamOff_R4
               // 25 menitOff_R4                               
               
    if(regset>9) //25 
      {
        /*if(regset_menitOFF_R4==HIGH)
        {EEPROM.update(16,menitOFF_R4);
         regset_menitOFF_R4=LOW;}*/
        lcd.clear();
        regset=0;
      }       
  }

//------
  switch (regset) {
    case 0:
      //tampilanLCD();
      break;   
    case 1:
      //tampilanLCD();
      break;
    case 2:
      //tampilanLCD();
      break;      
    case 3:
      S_tahun();
      break;
    case 4:
      S_bulan();
      break;
    case 5:
      S_tanggal();
      break;
    case 6:
      S_hari();
      break;
    case 7:
      S_jam();
      break;
    case 8:
      S_menit();
      break;
    case 9:
      S_detik();
      break;
      
    case 10:  //----------------------------------------------    
      S_jamON_R1();
      break; 
    case 11:  
      /*if (regset_jamON_R1==HIGH)     
      {EEPROM.update(1,jamON_R1); 
      regset_jamON_R1=LOW;
      regset_menitON_R1=HIGH;}*/
      S_menitON_R1();      
      break;
    case 12:  //----------------------------------------------
      /*if (regset_menitON_R1==HIGH)    
      {EEPROM.update(2,menitON_R1);  
      regset_menitON_R1=LOW;
      regset_jamOFF_R1=HIGH;} */
      S_jamOFF_R1();
      break; 
    case 13: 
      /*if (regset_jamOFF_R1==HIGH)    
      {EEPROM.update(3,jamOFF_R1); 
      regset_jamOFF_R1=LOW;
      regset_menitOFF_R1=HIGH;}  */    
      S_menitOFF_R1();
      break;
      
    //R2
    case 14:  //---------------------------------------------- 
      /*if (regset_menitOFF_R1==HIGH)    
      {EEPROM.update(4,menitOFF_R1); 
      regset_menitOFF_R1=LOW;
      regset_jamON_R2=HIGH;}   */
      S_jamON_R2();
      break; 
    case 15: 
      /*if (regset_jamON_R2==HIGH)    
      {EEPROM.update(5,jamON_R2); 
      regset_jamON_R2=LOW;
      regset_menitON_R2=HIGH;}  */  
      S_menitON_R2();
      break;
    case 16:  //----------------------------------------------
      /*if (regset_menitON_R2==HIGH)    
      {EEPROM.update(6,menitON_R2);
      regset_menitON_R2=LOW;
      regset_jamOFF_R2=HIGH;}    */
      S_jamOFF_R2();
      break; 
    case 17: 
      /*if (regset_jamOFF_R2==HIGH)    
      {EEPROM.update(7,jamOFF_R2); 
      regset_jamOFF_R2=LOW;
      regset_menitOFF_R2=HIGH;}  */   
      S_menitOFF_R2();
      break;      
 
    //R3
    case 18:  //----------------------------------------------
      /*if (regset_menitOFF_R2==HIGH)    
      {EEPROM.update(8,menitOFF_R2); 
      regset_menitOFF_R2=LOW;
      regset_jamON_R3=HIGH;}   */
      S_jamON_R3();
      break; 
    case 19:
      /*if (regset_jamON_R3==HIGH)    
      {EEPROM.update(9,jamON_R3);
      regset_jamON_R3=LOW;
      regset_menitON_R3=HIGH;}  */    
      S_menitON_R3();
      break;
    case 20:  //---------------------------------------------- 
      /*if (regset_menitON_R3==HIGH)    
      {EEPROM.update(10,menitON_R3); 
      regset_menitON_R3=LOW;
      regset_jamOFF_R3=HIGH;}  */
      S_jamOFF_R3();
      break; 
    case 21:
      /*if (regset_jamOFF_R3==HIGH)    
      {EEPROM.update(11,jamOFF_R3);
      regset_jamOFF_R3=LOW;
      regset_menitOFF_R3=HIGH;}   */   
      S_menitOFF_R3();
      break; 

    //R4
    case 22:  //----------------------------------------------
      /*if (regset_menitOFF_R3==HIGH)    
      {EEPROM.update(12,menitOFF_R3);
      regset_menitOFF_R3=LOW;
      regset_jamON_R4=HIGH;}  */ 
      S_jamON_R4();
      break; 
    case 23:
      /*if (regset_jamON_R4==HIGH)    
      {EEPROM.update(13,jamON_R4);
      regset_jamON_R4=LOW;
      regset_menitON_R4=HIGH;}  */     
      S_menitON_R4();
      break;
    case 24:  //----------------------------------------------
      /*if (regset_menitON_R4==HIGH)    
      {EEPROM.update(14,menitON_R4);
      regset_menitON_R4=LOW;
      regset_jamOFF_R4=HIGH;}   */
      S_jamOFF_R4();
      break; 
    case 25: 
      /*if (regset_jamOFF_R4==HIGH)    
      {EEPROM.update(15,jamOFF_R4); 
      regset_jamOFF_R4=LOW;
      regset_menitOFF_R4=HIGH;}  */  
      S_menitOFF_R4();
      break;  
       
  }
  
//--------------------------------------------- case reg end ----------------------------------


//---------- delay home for 15sec -------------------------------
  if(back0x == LOW && millis() > back0)
  {  
  cetit();
  back0 = millis()+15000;
  lcd.clear();
  regset = 0;
  back0x = HIGH;
  }  
//---------- delay home for 15sec -------------------------------


jamSaiki = (jame*60)+ menite;

kontrol_R1();
kontrol_R2();
kontrol_R3();
kontrol_R4();

//------------------------- lcd show ---------------------------
if(detike==44 && regset==0)
lcd.clear();

if(detike>=45)
bLightLCD=15;

if(regset==0)
{
  if(bLightLCD>0)
  bLightLCD --;
}

if(bLightLCD==0)
{
  lcd.noBacklight();
}
else
{
  lcd.backlight();
}


//------------------------- lcd show ---------------------------

  if(digitalRead(BLINK)==LOW)
  {digitalWrite(BLINK,HIGH);}
  else
  {digitalWrite(BLINK,LOW);}

//--------------------------------------------------------------


delay(1000); //common delay 1s
}
//================================================ akhir main loop =======================================




//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&



//-------------------------------- subroutine ------------------------------

//-----------------------------------R1-------------------------------
void kontrol_R1()
{  
  //0407 -> 240 - 420
  if (jamSaiki>=240 && jamSaiki<=420)
  {
    jamON_R1_0407=HIGH;
  }
  else
  {
    jamON_R1_0407=LOW;
  }

  //0811 -> 480 - 660
  if (jamSaiki>=480 && jamSaiki<=660)
  {
    jamON_R1_0811=HIGH;
  }
  else
  {
    jamON_R1_0811=LOW;
  }

  //1215 -> 720 - 900
  if (jamSaiki>=720 && jamSaiki<=900)
  {
    jamON_R1_1215=HIGH;
  }
  else
  {
    jamON_R1_1215=LOW;
  }

  //1619 -> 960 - 1140
  if (jamSaiki>=960 && jamSaiki<=1140)
  {
    jamON_R1_1619=HIGH;
  }
  else
  {
    jamON_R1_1619=LOW;
  }  
  
  //2023 -> 1200 - 1380
  if (jamSaiki>=1200 && jamSaiki<=1380)
  {
    jamON_R1_2023=HIGH;
  }
  else
  {
    jamON_R1_2023=LOW;
  }

  
  //0003 -> 1 - 180
  if (jamSaiki>=1 && jamSaiki<=180)
  {
    jamON_R1_0003=HIGH;
  }
  else
  {
    jamON_R1_0003=LOW;
  }

  //on off
  if (jamON_R1_0407==HIGH || jamON_R1_0811==HIGH || jamON_R1_1215==HIGH || jamON_R1_1619==HIGH || jamON_R1_2023==HIGH || jamON_R1_0003==HIGH )    
      {
        digitalWrite(R1,HIGH);   //active low : LOW    
      }  //cek output relay      
    else
      {digitalWrite(R1,LOW);}  
  
  /*
  start_R1 = (jamON_R1*60) + menitON_R1;
  stop_R1 = (jamOFF_R1*60) + menitOFF_R1;

  
  if(stop_R1<start_R1)
  {    
    if (LS_Count>58 && (jamSaiki >= start_R1 || jamSaiki < stop_R1) || (jamON_R1_1015==HIGH))    
      {
        digitalWrite(R1,HIGH);   //active low : LOW    
      }  //cek output relay      
    else
      {digitalWrite(R1,LOW);}   
  }

  if(stop_R1>start_R1)
  {
    if (LS_Count>58 && (jamON_R1_1015==HIGH) ||(jamSaiki >= start_R1 && jamSaiki < stop_R1))
      {
        digitalWrite(R1,HIGH);        
      }  //cek output relay   
    else
      {digitalWrite(R1,LOW);}        
  } 
  */ 
}
//-----------------------------------R1-------------------------------
//-----------------------------------R2-------------------------------
void kontrol_R2()
{  
  //20200427-1240
  if(digitalRead(R1)==HIGH)
  {
    digitalWrite(R2,HIGH);
  }
  else
  {
    digitalWrite(R2,LOW);
  }
  
  /*
  start_R2 = (jamON_R2*60) + menitON_R2;
  stop_R2 = (jamOFF_R2*60) + menitOFF_R2;
  
  if(stop_R2<start_R2)
  {
    if (jamSaiki >= start_R2 || jamSaiki < stop_R2)    
      {digitalWrite(R2,HIGH);}  //cek output relay      
    else
      {digitalWrite(R2,LOW);}   
  }

  if(stop_R2>start_R2)
  {
    if (jamSaiki >= start_R2 && jamSaiki < stop_R2)
      {digitalWrite(R2,HIGH);}  
    else
      {digitalWrite(R2,LOW);}        
  } 
  */ 
}
//-----------------------------------R2-------------------------------
//-----------------------------------R3-------------------------------
void kontrol_R3()
{  
  start_R3 = (jamON_R3*60) + menitON_R3;
  stop_R3 = (jamOFF_R3*60) + menitOFF_R3;
  if(stop_R3<start_R3)
  {
    if (jamSaiki >= start_R3 || jamSaiki < stop_R3)    
      {digitalWrite(R3,HIGH);}  //cek output relay      
    else
      {digitalWrite(R3,LOW);}   
  }

  if(stop_R3>start_R3)
  {
    if (jamSaiki >= start_R3 && jamSaiki < stop_R3)
      {digitalWrite(R3,HIGH);}  
    else
      {digitalWrite(R3,LOW);}        
  }  
}
//-----------------------------------R3-------------------------------
//-----------------------------------R4-------------------------------
void kontrol_R4()
{  
  start_R4 = (jamON_R4*60) + menitON_R4;
  stop_R4 = (jamOFF_R4*60) + menitOFF_R4;
  if(stop_R4<start_R4)
  {
    if (jamSaiki >= start_R4 || jamSaiki < stop_R4)    
      {digitalWrite(R4,HIGH);}  //cek output relay      
    else
      {digitalWrite(R4,LOW);}   
  }

  if(stop_R4>start_R4)
  {
    if (jamSaiki >= start_R4 && jamSaiki < stop_R4)
      {digitalWrite(R4,HIGH);}  
    else
      {digitalWrite(R4,LOW);}        
  }  
}
//-----------------------------------R4-------------------------------
//=============================================== sub routine =================================================
//=============================================== sub routine =================================================
//=============================================== sub routine =================================================


//---------------------------------- awal routine setting rtc -----------------------------------------
//-----------------------------------------------------------
void S_tahun()
{
//-------------- regset = 3  setting tahun
  tahunx = tahun - 2000;   
  //if(analogRead(A1)<=500 && doneudset == LOW)//increase
  if(digitalRead(pbup)==HIGH && doneudset == LOW)
  {
    cetit();
    back0x = LOW;
    back0 = millis()+15000;
    udset = millis()+500;
    doneudset = HIGH;
    tahunx++;
    if(tahunx >49)
      {tahunx = 1;} 
    RTC.stop();     
    RTC.set(DS1307_YR,tahunx);       //set the year 
    RTC.start();   
  }
  //if(analogRead(A2)<=500 && doneudset == LOW)//decrease
  if(digitalRead(pbdown)==HIGH && doneudset == LOW)
  {
    cetit();
    back0x = LOW;
    back0 = millis()+15000;
    udset = millis()+500;
    doneudset = HIGH;
    if (tahunx > 0)
    {tahunx--;}
    else
    {tahunx = 49;} 
    RTC.stop();     
    RTC.set(DS1307_YR,tahunx);       //set the year 
    RTC.start();  
  }
  
    lcd.clear();
    // tahun bulan tanggal
    // 0-15, 2017/07/12
    lcd.setCursor(3,0);
    lcd.print (tahun);    
    lcd.print("/");    
    if(bulan>=10)
    {lcd.print (bulan);}
    else
    {lcd.print("0");
    lcd.print (bulan);}
    
    lcd.print("/");
    
    if(tgl>=10)
    {lcd.print (tgl);}
    else
    {lcd.print("0");
    lcd.print (tgl);}
    
    lcd.setCursor(0,1); 
    lcd.print("Set Tahun:");
    lcd.print(tahun); 
}
//


//-----------------------------------------------------------
void S_bulan()
{
//-------------- regset = 4  setting bulan
   
  //if(analogRead(A1)<=500 && doneudset == LOW)//increase
  if(digitalRead(pbup)==HIGH && doneudset == LOW)
  {
    cetit();
    back0x = LOW;
    back0 = millis()+15000;
    udset = millis()+500;
    doneudset = HIGH;
    bulan++;
    if(bulan >12)
      {bulan = 1;} 
    
    RTC.stop();     
    RTC.set(DS1307_MTH,bulan);       //set the month 
    RTC.start();   
  }
  //if(analogRead(A2)<=500 && doneudset == LOW)
  if(digitalRead(pbdown)==HIGH && doneudset == LOW)
  {
    cetit();
    back0x = LOW;
    back0 = millis()+15000;
    udset = millis()+500;
    doneudset = HIGH;
    bulan--;
    if(bulan <1)
      {bulan = 12;} 
    
    RTC.stop();     
    RTC.set(DS1307_MTH,bulan);       //set the month 
    RTC.start();  
  }
    lcd.clear();
    // tahun bulan tanggal
    // 0-15, 2017/07/12
    lcd.setCursor(3,0);
    lcd.print (tahun);    
    lcd.print("/");    
    if(bulan>=10)
    {lcd.print (bulan);}
    else
    {lcd.print("0");
    lcd.print (bulan);}
    
    lcd.print("/");
    
    if(tgl>=10)
    {lcd.print (tgl);}
    else
    {lcd.print("0");
    lcd.print (tgl);}
    
    lcd.setCursor(0,1); 
    lcd.print("Set Bulan:");
    lcd.print(bulan);
}
//

//-----------------------------------------------------------
void S_tanggal()
{
//-------------- regset = 5  setting tanggal
   
  //if(analogRead(A1)<=500 && doneudset == LOW)//increase
  if(digitalRead(pbup)==HIGH && doneudset == LOW)
  {
    cetit();
    back0x = LOW;
    back0 = millis()+15000;
    udset = millis()+500;
    doneudset = HIGH;
    tgl++;
    if(tgl >31)
      {tgl = 1;} 
    RTC.stop();     
    RTC.set(DS1307_DATE,tgl);       //set the date 
    RTC.start();  
  }
  //if(analogRead(A2)<=500 && doneudset == LOW)
  if(digitalRead(pbdown)==HIGH && doneudset == LOW)
  {
    cetit();
    back0x = LOW;
    back0 = millis()+15000;
    udset = millis()+500;
    doneudset = HIGH;
    tgl--;
    if(tgl <1)
      {tgl = 31;} 
    RTC.stop();     
    RTC.set(DS1307_DATE,tgl);       //set the date 
    RTC.start();
  }
    lcd.clear();
    // tahun bulan tanggal
    // 0-15, 2017/07/12
    lcd.setCursor(3,0);
    lcd.print (tahun);    
    lcd.print("/");    
    if(bulan>=10)
    {lcd.print (bulan);}
    else
    {lcd.print("0");
    lcd.print (bulan);}
    
    lcd.print("/");
    
    if(tgl>=10)
    {lcd.print (tgl);}
    else
    {lcd.print("0");
    lcd.print (tgl);}
    
    lcd.setCursor(0,1); 
    lcd.print("Set Tgl:");
    lcd.print(tgl);   
}
//

//-----------------------------------------------------------
void S_hari()
{
//-------------- regset = 6  setting hari
   
  //if(analogRead(A1)<=500 && doneudset == LOW)//increase
  if(digitalRead(pbup)==HIGH && doneudset == LOW)
  {
    cetit();
    back0x = LOW;
    back0 = millis()+15000;
    udset = millis()+500;
    doneudset = HIGH;
    hari++;
    if(hari >7)
      {hari = 1;} 
    RTC.stop();     
    RTC.set(DS1307_DOW,hari);       //set the day of week  
    RTC.start(); 
  }
  //if(analogRead(A2)<=500 && doneudset == LOW)
  if(digitalRead(pbdown)==HIGH && doneudset == LOW)
  {
    cetit();
    back0x = LOW;
    back0 = millis()+15000;
    udset = millis()+500;
    doneudset = HIGH;
    hari--;
    if(hari <1)
      {hari = 7;} 
    RTC.stop();     
    RTC.set(DS1307_DOW,hari);       //set the day of week 
    RTC.start();   
  }
    lcd.clear();
    // tahun bulan tanggal
    // 0-15, 2017/07/12
    lcd.setCursor(0,0);
    lcd.print (dino[hari]);    
    
    lcd.setCursor(0,1); 
    lcd.print("Set Hari:");
    lcd.print(dino[hari]);  
}
//



//0909090
//-----------------------------------------------------------
void S_jam()
{
//-------------- regset = 7  setting jam

  //if(analogRead(A1)<=500 && doneudset == LOW)//increase
  if(digitalRead(pbup)==HIGH && doneudset == LOW)
  {
    cetit();
    back0x = LOW;
    back0 = millis()+15000;
    udset = millis()+500;
    doneudset = HIGH;
    jame++;
    if(jame >23)
      {jame = 0;} 
    RTC.stop();     
    RTC.set(DS1307_HR,jame);       //set the hour 
    RTC.start();  
  }
  //if(analogRead(A2)<=500 && doneudset == LOW)
  if(digitalRead(pbdown)==HIGH && doneudset == LOW)
  {
    cetit();
    back0x = LOW;
    back0 = millis()+15000;
    udset = millis()+500;
    doneudset = HIGH;
    if(jame > 0)
    {jame--;}
    else   
    {jame = 23;} 
    RTC.stop();     
    RTC.set(DS1307_HR,jame);       //set the hour 
    RTC.start();  
  }
    lcd.clear();
    lcd.setCursor(4,0);
    if(jame>=10)
    {lcd.print (jame);}
    else
    {lcd.print("0");
    lcd.print (jame);}
    
    lcd.print(":");    
    if(menite>=10)
    {lcd.print (menite);}
    else
    {lcd.print("0");
    lcd.print (menite);}
    
    lcd.print(":");
    
    if(detike>=10)
    {lcd.print (detike);}
    else
    {lcd.print("0");
    lcd.print (detike);}
    
    lcd.setCursor(0,1); 
    lcd.print("Set Jam:");
    lcd.print(jame);   
}
//

//-----------------------------------------------------------
void S_menit()
{
//-------------- regset = 8  setting menit

  //if(analogRead(A1)<=500 && doneudset == LOW)//increase
  if(digitalRead(pbup)==HIGH && doneudset == LOW)
  {
    cetit();
    back0x = LOW;
    back0 = millis()+15000;
    udset = millis()+500;
    doneudset = HIGH;
    menite++;
    if(menite >59)
      {menite = 0;} 
    RTC.stop();     
    RTC.set(DS1307_MIN,menite);       //set the minute 
    RTC.start();   
  }
  //if(analogRead(A2)<=500 && doneudset == LOW)
  if(digitalRead(pbdown)==HIGH && doneudset == LOW)
  {
    cetit();
    back0x = LOW;
    back0 = millis()+15000;
    udset = millis()+500;
    doneudset = HIGH;
    if(menite > 0)
    {menite--;}
    else    
    {menite = 59;} 
    RTC.stop();     
    RTC.set(DS1307_MIN,menite);       //set the minute 
    RTC.start();  
  }
    lcd.clear();
    lcd.setCursor(4,0);
    if(jame>=10)
    {lcd.print (jame);}
    else
    {lcd.print("0");
    lcd.print (jame);}
    
    lcd.print(":");    
    if(menite>=10)
    {lcd.print (menite);}
    else
    {lcd.print("0");
    lcd.print (menite);}
    
    lcd.print(":");
    
    if(detike>=10)
    {lcd.print (detike);}
    else
    {lcd.print("0");
    lcd.print (detike);}
    
    lcd.setCursor(0,1); 
    lcd.print("Set Menit:");
    lcd.print(menite); 
}
//


//-----------------------------------------------------------
void S_detik()
{
//-------------- regset = 9  setting detik
  
  //if(analogRead(A1)<=500 && doneudset == LOW)//increase
  if(digitalRead(pbup)==HIGH && doneudset == LOW)
  {
    cetit();
    back0x = LOW;
    back0 = millis()+15000;
    udset = millis()+500;
    doneudset = HIGH;
    RTC.stop();     
    RTC.set(DS1307_SEC,0);       //set the second 0
    RTC.start();   
  }
  //if(analogRead(A2)<=500 && doneudset == LOW)
  if(digitalRead(pbdown)==HIGH && doneudset == LOW)
  {
    cetit();
    back0x = LOW;
    back0 = millis()+15000;
    udset = millis()+500;
    doneudset = HIGH;    
    RTC.stop();     
    RTC.set(DS1307_SEC,0);       //set the second 0
    RTC.start();  
  }
    lcd.clear();
    lcd.setCursor(4,0);
    if(jame>=10)
    {lcd.print (jame);}
    else
    {lcd.print("0");
    lcd.print (jame);}
    
    lcd.print(":");    
    if(menite>=10)
    {lcd.print (menite);}
    else
    {lcd.print("0");
    lcd.print (menite);}
    
    lcd.print(":");
    
    if(detike>=10)
    {lcd.print (detike);}
    else
    {lcd.print("0");
    lcd.print (detike);}
    
    lcd.setCursor(0,1); 
    lcd.print("Set Detik 0:");
    lcd.print(detike);    
}
//---------------------------------- akhir routine setting rtc -----------------------------------------


//======================================================================================================
//----------------------------------- start routine setting Relay --------------------------------------

//R1
//-----------------------------------------------------------
               // 10 jamON_R1, eeprom 1
               // 11 menitON_R1, eeprom 2
               // 12 jamOff_R1, eeprom 3
               // 13 menitOff_R1, eeprom 4
               // a1 inc, 
               // a2 dec,
               
void S_jamON_R1()
{
//-------------- regset = 10  setting jam ON R1
  
  //if(analogRead(A1)<=500 && doneudset == LOW)//increase
  if(digitalRead(pbup)==HIGH && doneudset == LOW)
  {
    cetit();
    back0x = LOW;
    back0 = millis()+15000;
    udset = millis()+500;
    doneudset = HIGH;    
    jamON_R1++;
    if(jamON_R1 >23)
      {jamON_R1 = 0;}
      EEPROM.write(1,jamON_R1); //20200420pindah ke case  
  }
  //if(analogRead(A2)<=500 && doneudset == LOW)
  if(digitalRead(pbdown)==HIGH && doneudset == LOW)
  {
    cetit();
    back0x = LOW;
    back0 = millis()+15000;
    udset = millis()+500;
    doneudset = HIGH;    
    if(jamON_R1 > 0 )
    {jamON_R1--;}
    else    
    {jamON_R1 = 23;}
    EEPROM.write(1,jamON_R1);  //20200420pindah ke case
  }
    lcd.setCursor(0,0);
    lcd.print("R1");
    lcd.setCursor(0,1);
    lcd.print("Jam On");
    
    lcd.setCursor(11,1);
    if(jamON_R1>=10)
    {lcd.print (jamON_R1);}
    else
    {lcd.print("0");
    lcd.print (jamON_R1);}    
    lcd.print(":");    
    if(menitON_R1>=10)
    {lcd.print (menitON_R1);}
    else
    {lcd.print("0");
    lcd.print (menitON_R1);}     
}


//-----------------------------------------------------------
void S_menitON_R1()
{
//-------------- regset = 11  setting menit ON R1
  
  //if(analogRead(A1)<=500 && doneudset == LOW)//increase
  if(digitalRead(pbup)==HIGH && doneudset == LOW)
  {
    cetit();
    back0x = LOW;
    back0 = millis()+15000;
    udset = millis()+500;
    doneudset = HIGH;    
    menitON_R1++;
    if(menitON_R1 >59)
      {menitON_R1 = 0;}
      EEPROM.write(2,menitON_R1);//pindah ke case   
  }
  //if(analogRead(A2)<=500 && doneudset == LOW)
  if(digitalRead(pbdown)==HIGH && doneudset == LOW)
  {
    cetit();
    back0x = LOW;
    back0 = millis()+15000;
    udset = millis()+1000;
    doneudset = HIGH;    
    if(menitON_R1 > 0 )
    {menitON_R1--;}
    else    
    {menitON_R1 = 59;}
     EEPROM.write(2,menitON_R1);//pindah ke case   
  }
    lcd.setCursor(0,0);
    lcd.print("R1");
    lcd.setCursor(0,1);
    lcd.print("Menit On");
    
    lcd.setCursor(11,1);
    if(jamON_R1>=10)
    {lcd.print (jamON_R1);}
    else
    {lcd.print("0");
    lcd.print (jamON_R1);}    
    lcd.print(":");    
    if(menitON_R1>=10)
    {lcd.print (menitON_R1);}
    else
    {lcd.print("0");
    lcd.print (menitON_R1);}     
}


//-----------------------------------------------------------
void S_jamOFF_R1()
{
//-------------- regset = 12  setting jam OFF R1
  
  //if(analogRead(A1)<=500 && doneudset == LOW)//increase
  if(digitalRead(pbup)==HIGH && doneudset == LOW)
  {
    cetit();
    back0x = LOW;
    back0 = millis()+15000;
    udset = millis()+500;
    doneudset = HIGH;    
    jamOFF_R1++;
    if(jamOFF_R1 >23)
      {jamOFF_R1 = 0;}
      EEPROM.write(3,jamOFF_R1);//pindah ke case   
  }
  //if(analogRead(A2)<=500 && doneudset == LOW)
  if(digitalRead(pbdown)==HIGH && doneudset == LOW)
  {
    cetit();
    back0x = LOW;
    back0 = millis()+15000;
    udset = millis()+1000;
    doneudset = HIGH;    
    if(jamOFF_R1 > 0 )
    {jamOFF_R1--;}
    else    
    {jamOFF_R1 = 23;}
     EEPROM.write(3,jamOFF_R1);//   
  }
    lcd.setCursor(0,0);
    lcd.print("R1");
    lcd.setCursor(0,1);
    lcd.print("Jam Off");
    
    lcd.setCursor(11,1);
    if(jamOFF_R1>=10)
    {lcd.print (jamOFF_R1);}
    else
    {lcd.print("0");
    lcd.print (jamOFF_R1);}    
    lcd.print(":");    
    if(menitOFF_R1>=10)
    {lcd.print (menitOFF_R1);}
    else
    {lcd.print("0");
    lcd.print (menitOFF_R1);}   
}
//


//-----------------------------------------------------------
void S_menitOFF_R1()
{
//-------------- regset = 13  setting menit OFF R1
  
  //if(analogRead(A1)<=500 && doneudset == LOW)//increase
  if(digitalRead(pbup)==HIGH && doneudset == LOW)
  {
    cetit();
    back0x = LOW;
    back0 = millis()+15000;
    udset = millis()+1000;
    doneudset = HIGH;    
    menitOFF_R1++;
    if(menitOFF_R1 >59)
      {menitOFF_R1 = 0;}
      EEPROM.write(4,menitOFF_R1);   
  }
  //if(analogRead(A2)<=500 && doneudset == LOW)
  if(digitalRead(pbdown)==HIGH && doneudset == LOW)
  {
    cetit();
    back0x = LOW;
    back0 = millis()+15000;
    udset = millis()+1000;
    doneudset = HIGH;    
    if(menitOFF_R1 > 0 )
    {menitOFF_R1--;}
    else    
    {menitOFF_R1 = 59;}
     EEPROM.write(4,menitOFF_R1);   
  }
    lcd.setCursor(0,0);
    lcd.print("R1");
    lcd.setCursor(0,1);
    lcd.print("Menit Off");
    
    lcd.setCursor(11,1);
    if(jamOFF_R1>=10)
    {lcd.print (jamOFF_R1);}
    else
    {lcd.print("0");
    lcd.print (jamOFF_R1);}    
    lcd.print(":");    
    if(menitOFF_R1>=10)
    {lcd.print (menitOFF_R1);}
    else
    {lcd.print("0");
    lcd.print (menitOFF_R1);}     
}
//
//--------------------------------------R1---------------------------------------
//--------------------------------------R2---------------------------------------
//R2
//-----------------------------------------------------------
               // 14 jamON_R1, eeprom 5
               // 15 menitON_R1, eeprom 6
               // 16 jamOff_R1, eeprom 7
               // 17 menitOff_R1, eeprom 8
               // a1 inc, 
               // a2 dec,
               
void S_jamON_R2()
{
//-------------- regset = 14  setting jam ON R2
  
  //if(analogRead(A1)<=500 && doneudset == LOW)//increase
  if(digitalRead(pbup)==HIGH && doneudset == LOW)
  {
    cetit();
    back0x = LOW;
    back0 = millis()+15000;
    udset = millis()+500;
    doneudset = HIGH;    
    jamON_R2++;
    if(jamON_R2 >23)
      {jamON_R2 = 0;}
     EEPROM.write(5,jamON_R2);   
  }
  //if(analogRead(A2)<=500 && doneudset == LOW)
  if(digitalRead(pbdown)==HIGH && doneudset == LOW)
  {
    cetit();
    back0x = LOW;
    back0 = millis()+15000;
    udset = millis()+500;
    doneudset = HIGH;    
    if(jamON_R2 > 0 )
    {jamON_R2--;}
    else    
    {jamON_R2 = 23;}
      EEPROM.write(5,jamON_R2);   
  }
    lcd.setCursor(0,0);
    lcd.print("R2");
    lcd.setCursor(0,1);
    lcd.print("Jam On");
    
    lcd.setCursor(11,1);
    if(jamON_R2>=10)
    {lcd.print (jamON_R2);}
    else
    {lcd.print("0");
    lcd.print (jamON_R2);}    
    lcd.print(":");    
    if(menitON_R2>=10)
    {lcd.print (menitON_R2);}
    else
    {lcd.print("0");
    lcd.print (menitON_R2);}     
}


//-----------------------------------------------------------
void S_menitON_R2()
{
//-------------- regset = 15  setting menit ON R2
  
  //if(analogRead(A1)<=500 && doneudset == LOW)//increase
  if(digitalRead(pbup)==HIGH && doneudset == LOW)
  {
    cetit();
    back0x = LOW;
    back0 = millis()+15000;
    udset = millis()+500;
    doneudset = HIGH;    
    menitON_R2++;
    if(menitON_R2 >59)
      {menitON_R2 = 0;}
     EEPROM.write(6,menitON_R2);   
  }
  //if(analogRead(A2)<=500 && doneudset == LOW)
  if(digitalRead(pbdown)==HIGH && doneudset == LOW)
  {
    cetit();
    back0x = LOW;
    back0 = millis()+15000;
    udset = millis()+1000;
    doneudset = HIGH;    
    if(menitON_R2 > 0 )
    {menitON_R2--;}
    else    
    {menitON_R2 = 59;}
     EEPROM.write(6,menitON_R2);   
  }
    lcd.setCursor(0,0);
    lcd.print("R2");
    lcd.setCursor(0,1);
    lcd.print("Menit On");
    
    lcd.setCursor(11,1);
    if(jamON_R2>=10)
    {lcd.print (jamON_R2);}
    else
    {lcd.print("0");
    lcd.print (jamON_R2);}    
    lcd.print(":");    
    if(menitON_R2>=10)
    {lcd.print (menitON_R2);}
    else
    {lcd.print("0");
    lcd.print (menitON_R2);}     
}


//-----------------------------------------------------------
void S_jamOFF_R2()
{
//-------------- regset = 16  setting jam OFF R2
  
  //if(analogRead(A1)<=500 && doneudset == LOW)//increase
  if(digitalRead(pbup)==HIGH && doneudset == LOW)
  {
    cetit();
    back0x = LOW;
    back0 = millis()+15000;
    udset = millis()+500;
    doneudset = HIGH;    
    jamOFF_R2++;
    if(jamOFF_R2 >23)
      {jamOFF_R2 = 0;}
     EEPROM.write(7,jamOFF_R2);   
  }
  //if(analogRead(A2)<=500 && doneudset == LOW)
  if(digitalRead(pbdown)==HIGH && doneudset == LOW)
  {
    cetit();
    back0x = LOW;
    back0 = millis()+15000;
    udset = millis()+1000;
    doneudset = HIGH;    
    if(jamOFF_R2 > 0 )
    {jamOFF_R2--;}
    else    
    {jamOFF_R2 = 23;}
     EEPROM.write(7,jamOFF_R2);   
  }
    lcd.setCursor(0,0);
    lcd.print("R2");
    lcd.setCursor(0,1);
    lcd.print("Jam Off");
    
    lcd.setCursor(11,1);
    if(jamOFF_R2>=10)
    {lcd.print (jamOFF_R2);}
    else
    {lcd.print("0");
    lcd.print (jamOFF_R2);}    
    lcd.print(":");    
    if(menitOFF_R2>=10)
    {lcd.print (menitOFF_R2);}
    else
    {lcd.print("0");
    lcd.print (menitOFF_R2);}   
}
//


//-----------------------------------------------------------
void S_menitOFF_R2()
{
//-------------- regset = 17  setting menit OFF R2
  
  //if(analogRead(A1)<=500 && doneudset == LOW)//increase
  if(digitalRead(pbup)==HIGH && doneudset == LOW)
  {
    cetit();
    back0x = LOW;
    back0 = millis()+15000;
    udset = millis()+1000;
    doneudset = HIGH;    
    menitOFF_R2++;
    if(menitOFF_R2 >59)
      {menitOFF_R2 = 0;}
     EEPROM.write(8,menitOFF_R2);   
  }
  //if(analogRead(A2)<=500 && doneudset == LOW)
  if(digitalRead(pbdown)==HIGH && doneudset == LOW)
  {
    cetit();
    back0x = LOW;
    back0 = millis()+15000;
    udset = millis()+1000;
    doneudset = HIGH;    
    if(menitOFF_R2 > 0 )
    {menitOFF_R2--;}
    else    
    {menitOFF_R2 = 59;}
     EEPROM.write(8,menitOFF_R2);   
  }
    lcd.setCursor(0,0);
    lcd.print("R2");
    lcd.setCursor(0,1);
    lcd.print("Menit Off");
    
    lcd.setCursor(11,1);
    if(jamOFF_R2>=10)
    {lcd.print (jamOFF_R2);}
    else
    {lcd.print("0");
    lcd.print (jamOFF_R2);}    
    lcd.print(":");    
    if(menitOFF_R2>=10)
    {lcd.print (menitOFF_R2);}
    else
    {lcd.print("0");
    lcd.print (menitOFF_R2);}     
}
//
//--------------------------------------R2---------------------------------------
//--------------------------------------R3---------------------------------------
//R3
//-----------------------------------------------------------
               // 18 jamON_R3, eeprom 9
               // 19 menitON_R3, eeprom 10
               // 20 jamOff_R3, eeprom 11
               // 21 menitOff_R3, eeprom 12
               // a1 inc, 
               // a2 dec,
               
void S_jamON_R3()
{
//-------------- regset = 18  setting jam ON R3
  
  //if(analogRead(A1)<=500 && doneudset == LOW)//increase
  if(digitalRead(pbup)==HIGH && doneudset == LOW)
  {
    cetit();
    back0x = LOW;
    back0 = millis()+15000;
    udset = millis()+500;
    doneudset = HIGH;    
    jamON_R3++;
    if(jamON_R3 >23)
      {jamON_R3 = 0;}
     EEPROM.write(9,jamON_R3);   
  }
  //if(analogRead(A2)<=500 && doneudset == LOW)
  if(digitalRead(pbdown)==HIGH && doneudset == LOW)
  {
    cetit();
    back0x = LOW;
    back0 = millis()+15000;
    udset = millis()+500;
    doneudset = HIGH;    
    if(jamON_R3 > 0 )
    {jamON_R3--;}
    else    
    {jamON_R3 = 23;}
     EEPROM.write(9,jamON_R3);   
  }
    lcd.setCursor(0,0);
    lcd.print("R3");
    lcd.setCursor(0,1);
    lcd.print("Jam On");
    
    lcd.setCursor(11,1);
    if(jamON_R3>=10)
    {lcd.print (jamON_R3);}
    else
    {lcd.print("0");
    lcd.print (jamON_R3);}    
    lcd.print(":");    
    if(menitON_R3>=10)
    {lcd.print (menitON_R3);}
    else
    {lcd.print("0");
    lcd.print (menitON_R3);}     
}


//-----------------------------------------------------------
void S_menitON_R3()
{
//-------------- regset = 19  setting menit ON R3
  
  //if(analogRead(A1)<=500 && doneudset == LOW)//increase
  if(digitalRead(pbup)==HIGH && doneudset == LOW)
  {
    cetit();
    back0x = LOW;
    back0 = millis()+15000;
    udset = millis()+500;
    doneudset = HIGH;    
    menitON_R3++;
    if(menitON_R3 >59)
      {menitON_R3 = 0;}
     EEPROM.write(10,menitON_R3);   
  }
  //if(analogRead(A2)<=500 && doneudset == LOW)
  if(digitalRead(pbdown)==HIGH && doneudset == LOW)
  {
    cetit();
    back0x = LOW;
    back0 = millis()+15000;
    udset = millis()+1000;
    doneudset = HIGH;    
    if(menitON_R3 > 0 )
    {menitON_R3--;}
    else    
    {menitON_R3 = 59;}
     EEPROM.write(10,menitON_R3);   
  }
    lcd.setCursor(0,0);
    lcd.print("R3");
    lcd.setCursor(0,1);
    lcd.print("Menit On");
    
    lcd.setCursor(11,1);
    if(jamON_R3>=10)
    {lcd.print (jamON_R3);}
    else
    {lcd.print("0");
    lcd.print (jamON_R3);}    
    lcd.print(":");    
    if(menitON_R3>=10)
    {lcd.print (menitON_R3);}
    else
    {lcd.print("0");
    lcd.print (menitON_R3);}     
}


//-----------------------------------------------------------
void S_jamOFF_R3()
{
//-------------- regset = 20  setting jam OFF R3
  
  //if(analogRead(A1)<=500 && doneudset == LOW)//increase
  if(digitalRead(pbup)==HIGH && doneudset == LOW)
  {
    cetit();
    back0x = LOW;
    back0 = millis()+15000;
    udset = millis()+500;
    doneudset = HIGH;    
    jamOFF_R3++;
    if(jamOFF_R3 >23)
      {jamOFF_R3 = 0;}
     EEPROM.write(11,jamOFF_R3);   
  }
  //if(analogRead(A2)<=500 && doneudset == LOW)
  if(digitalRead(pbdown)==HIGH && doneudset == LOW)
  {
    cetit();
    back0x = LOW;
    back0 = millis()+15000;
    udset = millis()+1000;
    doneudset = HIGH;    
    if(jamOFF_R3 > 0 )
    {jamOFF_R3--;}
    else    
    {jamOFF_R3 = 23;}
     EEPROM.write(11,jamOFF_R3);   
  }
    lcd.setCursor(0,0);
    lcd.print("R3");
    lcd.setCursor(0,1);
    lcd.print("Jam Off");
    
    lcd.setCursor(11,1);
    if(jamOFF_R3>=10)
    {lcd.print (jamOFF_R3);}
    else
    {lcd.print("0");
    lcd.print (jamOFF_R3);}    
    lcd.print(":");    
    if(menitOFF_R3>=10)
    {lcd.print (menitOFF_R3);}
    else
    {lcd.print("0");
    lcd.print (menitOFF_R3);}   
}
//


//-----------------------------------------------------------
void S_menitOFF_R3()
{
//-------------- regset = 21  setting menit OFF R3
  
  //if(analogRead(A1)<=500 && doneudset == LOW)//increase
  if(digitalRead(pbup)==HIGH && doneudset == LOW)
  {
    cetit();
    back0x = LOW;
    back0 = millis()+15000;
    udset = millis()+1000;
    doneudset = HIGH;    
    menitOFF_R3++;
    if(menitOFF_R3 >59)
      {menitOFF_R3 = 0;}
     EEPROM.write(12,menitOFF_R3);   
  }
  //if(analogRead(A2)<=500 && doneudset == LOW)
  if(digitalRead(pbdown)==HIGH && doneudset == LOW)
  {
    cetit();
    back0x = LOW;
    back0 = millis()+15000;
    udset = millis()+1000;
    doneudset = HIGH;    
    if(menitOFF_R3 > 0 )
    {menitOFF_R3--;}
    else    
    {menitOFF_R3 = 59;}
     EEPROM.write(12,menitOFF_R3);   
  }
    lcd.setCursor(0,0);
    lcd.print("R3");
    lcd.setCursor(0,1);
    lcd.print("Menit Off");
    
    lcd.setCursor(11,1);
    if(jamOFF_R3>=10)
    {lcd.print (jamOFF_R3);}
    else
    {lcd.print("0");
    lcd.print (jamOFF_R3);}    
    lcd.print(":");    
    if(menitOFF_R3>=10)
    {lcd.print (menitOFF_R3);}
    else
    {lcd.print("0");
    lcd.print (menitOFF_R3);}     
}
//
//--------------------------------------R3---------------------------------------
//--------------------------------------R4---------------------------------------
//R4
//-----------------------------------------------------------
               // 22 jamON_R3, eeprom 13
               // 23 menitON_R3, eeprom 14
               // 24 jamOff_R3, eeprom 15
               // 25 menitOff_R3, eeprom 16
               // a1 inc, 
               // a2 dec,
               
void S_jamON_R4()
{
//-------------- regset = 22  setting jam ON R4
  
  //if(analogRead(A1)<=500 && doneudset == LOW)//increase
  if(digitalRead(pbup)==HIGH && doneudset == LOW)
  {
    cetit();
    back0x = LOW;
    back0 = millis()+15000;
    udset = millis()+500;
    doneudset = HIGH;    
    jamON_R4++;
    if(jamON_R4 >23)
      {jamON_R4 = 0;}
     EEPROM.write(13,jamON_R4);   
  }
  //if(analogRead(A2)<=500 && doneudset == LOW)
  if(digitalRead(pbdown)==HIGH && doneudset == LOW)
  {
    cetit();
    back0x = LOW;
    back0 = millis()+15000;
    udset = millis()+500;
    doneudset = HIGH;    
    if(jamON_R4 > 0 )
    {jamON_R4--;}
    else    
    {jamON_R4 = 23;}
     EEPROM.write(13,jamON_R4);   
  }
    lcd.setCursor(0,0);
    lcd.print("R4");
    lcd.setCursor(0,1);
    lcd.print("Jam On");
    
    lcd.setCursor(11,1);
    if(jamON_R4>=10)
    {lcd.print (jamON_R4);}
    else
    {lcd.print("0");
    lcd.print (jamON_R4);}    
    lcd.print(":");    
    if(menitON_R4>=10)
    {lcd.print (menitON_R4);}
    else
    {lcd.print("0");
    lcd.print (menitON_R4);}     
}


//-----------------------------------------------------------
void S_menitON_R4()
{
//-------------- regset = 23  setting menit ON R4
  
  //if(analogRead(A1)<=500 && doneudset == LOW)//increase
  if(digitalRead(pbup)==HIGH && doneudset == LOW)
  {
    cetit();
    back0x = LOW;
    back0 = millis()+15000;
    udset = millis()+500;
    doneudset = HIGH;    
    menitON_R4++;
    if(menitON_R4 >59)
      {menitON_R4 = 0;}
     EEPROM.write(14,menitON_R4);   
  }
  //if(analogRead(A2)<=500 && doneudset == LOW)
  if(digitalRead(pbdown)==HIGH && doneudset == LOW)
  {
    cetit();
    back0x = LOW;
    back0 = millis()+15000;
    udset = millis()+1000;
    doneudset = HIGH;    
    if(menitON_R4 > 0 )
    {menitON_R4--;}
    else    
    {menitON_R4 = 59;}
     EEPROM.write(14,menitON_R4);   
  }
    lcd.setCursor(0,0);
    lcd.print("R4");
    lcd.setCursor(0,1);
    lcd.print("Menit On");
    
    lcd.setCursor(11,1);
    if(jamON_R4>=10)
    {lcd.print (jamON_R4);}
    else
    {lcd.print("0");
    lcd.print (jamON_R4);}    
    lcd.print(":");    
    if(menitON_R4>=10)
    {lcd.print (menitON_R4);}
    else
    {lcd.print("0");
    lcd.print (menitON_R4);}     
}


//-----------------------------------------------------------
void S_jamOFF_R4()
{
//-------------- regset = 24  setting jam OFF R4
  
  //if(analogRead(A1)<=500 && doneudset == LOW)//increase
  if(digitalRead(pbup)==HIGH && doneudset == LOW)
  {
    cetit();
    back0x = LOW;
    back0 = millis()+15000;
    udset = millis()+500;
    doneudset = HIGH;    
    jamOFF_R4++;
    if(jamOFF_R4 >23)
      {jamOFF_R4 = 0;}
     EEPROM.write(15,jamOFF_R4);   
  }
  //if(analogRead(A2)<=500 && doneudset == LOW)
  if(digitalRead(pbdown)==HIGH && doneudset == LOW)
  {
    cetit();
    back0x = LOW;
    back0 = millis()+15000;
    udset = millis()+1000;
    doneudset = HIGH;    
    if(jamOFF_R4 > 0 )
    {jamOFF_R4--;}
    else    
    {jamOFF_R4 = 23;}
     EEPROM.write(15,jamOFF_R4);   
  }
    lcd.setCursor(0,0);
    lcd.print("R4");
    lcd.setCursor(0,1);
    lcd.print("Jam Off");
    
    lcd.setCursor(11,1);
    if(jamOFF_R4>=10)
    {lcd.print (jamOFF_R4);}
    else
    {lcd.print("0");
    lcd.print (jamOFF_R4);}    
    lcd.print(":");    
    if(menitOFF_R4>=10)
    {lcd.print (menitOFF_R4);}
    else
    {lcd.print("0");
    lcd.print (menitOFF_R4);}   
}
//


//-----------------------------------------------------------
void S_menitOFF_R4()
{
//-------------- regset = 25 setting menit OFF R4
  
  //if(analogRead(A1)<=500 && doneudset == LOW)//increase
  if(digitalRead(pbup)==HIGH && doneudset == LOW)
  {
    cetit();
    back0x = LOW;
    back0 = millis()+15000;
    udset = millis()+1000;
    doneudset = HIGH;    
    menitOFF_R4++;
    if(menitOFF_R4 >59)
      {menitOFF_R4 = 0;}
     EEPROM.write(16,menitOFF_R4);   
  }
  //if(analogRead(A2)<=500 && doneudset == LOW)
  if(digitalRead(pbdown)==HIGH && doneudset == LOW)
  {
    cetit();
    back0x = LOW;
    back0 = millis()+15000;
    udset = millis()+1000;
    doneudset = HIGH;    
    if(menitOFF_R4 > 0 )
    {menitOFF_R4--;}
    else    
    {menitOFF_R4 = 59;}
     EEPROM.write(16,menitOFF_R4);   
  }
    lcd.setCursor(0,0);
    lcd.print("R4");
    lcd.setCursor(0,1);
    lcd.print("Menit Off");
    
    lcd.setCursor(11,1);
    if(jamOFF_R4>=10)
    {lcd.print (jamOFF_R4);}
    else
    {lcd.print("0");
    lcd.print (jamOFF_R4);}    
    lcd.print(":");    
    if(menitOFF_R4>=10)
    {lcd.print (menitOFF_R4);}
    else
    {lcd.print("0");
    lcd.print (menitOFF_R4);}     
}
//
//--------------------------------------R4---------------------------------------


void cetit()
{
  digitalWrite(BZ,LOW);
  digitalWrite(BLINK,HIGH);
  delay(50);
  digitalWrite(BZ,HIGH);    
  digitalWrite(BLINK,LOW);
}


//----------------------------------
