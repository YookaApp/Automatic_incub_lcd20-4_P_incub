#include <LiquidCrystal_I2C.h>
#include <avr/wdt.h> //include a watchdog Librairie
#include <Wire.h>
#include "ds18.h"
#include "dht.h"
#include "rtc.h"
#include "moteur.h"

  // Composants connectés sur l'arduino
#define DHT_PIN 12
// leds pour le control de la temperature
#define RED_LED_T 4
#define GREEN_LED_T 5

//AUTRES
#define BUZZER 3
#define RELAIS_RES 9

unsigned long temp_lcd, temp_RedTemp, temp_buzzer;
bool state_Buzzer= false, state_RedTemp= false, state_GreenMotor = false;  //states to control buzzer and leds

void beginer(){
  
  lcd_tempe.init(); // initialisation de l'afficheur
  Serial.begin(9600);
  Wire.begin(); //initialisation de la voie i2c
  
  pinMode(RED_LED_T , OUTPUT);
  pinMode(GREEN_LED_T, OUTPUT);
  
  pinMode(BUZZER, OUTPUT);
  pinMode(RELAIS_RES , OUTPUT);

  //pins du moteur
  pinMode(PIN_MOTEUR_a, OUTPUT);
  pinMode(PIN_MOTEUR_d, OUTPUT);

  pinMode(PIN_MANUEL_RETOUR, INPUT);
  pinMode(PIN_FIN_de_COURSE_G, INPUT);
  pinMode(PIN_FIN_de_COURSE_D, INPUT);
}

void setup() {
    wdt_enable(WDTO_4S); //config a WTD 
    beginer();
    temp_lcd = millis();
    temp_buzzer=millis();
    temp_RedTemp=millis();
    lcd_tempe.backlight();
    
  //(sec, mins, heur, dow, dom, moi, an) mise a jours de l'heure et de la date
  //update_time(0, 32, 3, 3, 28, 12, 22);

  download_time( &dateTime ); // telecharge l'heure et la date
  HourNow= dateTime.hours;
// setup incubator

  initial_retournement();
  initialisation();
  wdt_reset();
 }

void loop() {
 /* 
  appel_fonction();                   //and that call a functions (download a times and temperature, Humidity)
  if(( millis() - temp_lcd) >= 1000){
    temp_lcd = millis();
    
    readDHT( DHT_PIN, &tempe, &humidy );
    download_time( &dateTime );
    
    affichage();                        // after one seconde , this function update a datas to screen
    Serial.println(dateTime.incremente_hours);

   }*/
      
    if(digitalRead(PIN_FIN_de_COURSE_D))
    digitalWrite(GREEN_LED_T, HIGH);

    else
    digitalWrite(GREEN_LED_T, LOW);
    
  wdt_reset();
}

void appel_fonction(){  
  retournement(); 
  control_temperature();
  control_buzzer(); 
  
   }


void control_temperature(){
  
  if(tempe < 37.45){
   digitalWrite(RELAIS_RES, LOW);
  }

  else if(tempe > 37.55){
   digitalWrite(RELAIS_RES, HIGH);
  }
  
  control_leds_T();
}

void control_leds_T(){

   if((tempe > 36) && (tempe < 38.9)){
    digitalWrite(RED_LED_T, LOW);
    digitalWrite(GREEN_LED_T, HIGH);
  }

  else{
    digitalWrite(GREEN_LED_T, LOW);
        if((millis()-temp_RedTemp) >= 500){
        temp_RedTemp=millis();
        state_RedTemp= !state_RedTemp;
        digitalWrite(RED_LED_T, state_RedTemp);
      }
  }
}


void control_buzzer(){
 
  if((tempe >= 39) || (humidy < 35) || valider ){
    if((millis()-temp_buzzer) >= 500){
        temp_buzzer=millis();
        state_Buzzer= !state_Buzzer;
        digitalWrite(BUZZER, state_Buzzer);
      }
    }
   
 else{ 
    digitalWrite(BUZZER, LOW);
      }
}
