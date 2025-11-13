#include <Arduino.h>
#include <FastLED.h>
#include <MicroOscSlip.h>
#include <M5_PbHub.h>

#define BROCHE_BOUTON_ATOM_LUMIERE 27

#define BROCHE_BOUTON_KEY 32
#define BROCHE_BOUTON_KEY_LUMIERE 26
#define KEY_CHANNEL 0

CRGB pixelAtom;
MicroOscSlip<128> monOsc(&Serial);
M5_PbHub myPbHub;

unsigned long monChronoDepart;

void setup() {
  Serial.begin(115200);
  Wire.begin();
  myPbHub.begin();

  FastLED.addLeds< WS2812,  BROCHE_BOUTON_ATOM_LUMIERE , GRB >(&pixelAtom, 1); 

  myPbHub.setPixelCount( KEY_CHANNEL , 1);

  pixelAtom = CRGB(255,0,0);
  FastLED.show();
  delay(1000);
  pixelAtom = CRGB(255,255,0);
  FastLED.show();
  delay(1000);
  pixelAtom = CRGB(0,255,0);
  FastLED.show();
  delay(1000);
  pixelAtom = CRGB(0,0,0);
  FastLED.show();

}

// FONCTION QUI SERA APPELÉE LORSQU'UN N'IMPORTTE QUEL MESSAGE OSC EST REÇU
// receivedOscMessage est le message reçu
void myOscMessageParser(MicroOscMessage & receivedOscMessage) {
  // Ici, un if et receivedOscMessage.checkOscAddress() est utilisé pour traiter les différents messages
  if (receivedOscMessage.checkOscAddress("/pixel")) {  // MODIFIER /pixel pour l'adresse qui sera reçue
       int premierArgument = receivedOscMessage.nextAsInt(); // Récupérer le premier argument du message en tant que int
       int deuxiemerArgument = receivedOscMessage.nextAsInt(); // SI NÉCESSAIRE, récupérer un autre int
       int troisiemerArgument = receivedOscMessage.nextAsInt(); // SI NÉCESSAIRE, récupérer un autre int

       // UTILISER ici les arguments récupérés
       myPbHub.setPixelColor( KEY_CHANNEL , 0 , premierArgument , deuxiemerArgument , troisiemerArgument );
        
   }
}


void loop() {

  int key = myPbHub.digitalRead(KEY_CHANNEL);

  monOsc.onOscMessageReceived(myOscMessageParser);

  if ( millis() - monChronoDepart >= 20 ) { 
    monChronoDepart = millis();

    monOsc.sendInt("/key", key);


 
  }
}

