#ifndef ds
#define ds //condition d'inclusion
#include <OneWire.h>

//differnetes constantes
#define PIN_DS 2
#define UPDATE 0x44 
#define LECTURE 0xBE
OneWire DS(PIN_DS);  //creation d'un objet Onewire

void getTemperature(float *temperature){

    byte addr[8]={0}; // tableau d'adresse
    byte data[9]={0}; // tableau des donnees  

    DS.reset_search(); //remise a zero du compteur de recherche 
    DS.search(addr);
           

 /*   for(int i= 0; i<8; i++){
      Serial.print(addr[i]);  AFFICHAGE DE L'ADRESSE DU CAPTEUR
    }*/

    // UPDATE DES DONNES DE LA ROM DU CAPTEUR
   DS.reset();     //remise a zero de la voie One_wire
   DS.select(addr); // selection du capteur
   DS.write(UPDATE); // commande pour lire et stocker les donnees dans la RAM du DS18B20
    delay(500);
   
    //RECUPERATION DES DONNES DE LA ROM DU CAPTEUR
   DS.reset(); //on commence a refraichir la communication
   DS.select(addr); //selection du capteur
   DS.write(LECTURE); //Commande pour ouvrir la communication de l'arduino au capteur

   // LECTURE DES OCTETS DANS LA ROM
   for(int j=0; j<9; j++){
      data[j]=DS.read();
   }

     *temperature = ((data[1] << 8) | data[0]) * 0.0625; 
}


#endif //fin de l'inclusion
