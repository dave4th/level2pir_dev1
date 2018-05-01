/*
 * The MIT License
 * 
 * Copyright 2018 Davide (mail4davide@gmail.com)
 * 
 * Permission is hereby granted, free of charge, to any person 
 * obtaining a copy of this software and associated 
 * documentation files (the "Software"), to deal in the 
 * Software without restriction, including without limitation 
 * the rights to use, copy, modify, merge, publish, distribute, 
 * sublicense, and/or sell copies of the Software, and to 
 * permit persons to whom the Software is furnished to do so, 
 * subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall 
 * be included in all copies or substantial portions of the 
 * Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY 
 * KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE 
 * WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR 
 * PURPOSE AND NONINFRINGEMENT. 
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE 
 * FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN 
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF 
 * OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
 * DEALINGS IN THE SOFTWARE.
 * 
 * ============================================================
 * 
 * level2pir_dev1 - Device PIR1 per Centralina livello 2
 * 
 * Il programma nasce per attiny85, ma funziona per certo
 * anche sull'arduino uno.
 * 
 * Questo programma utilizza le librerie:
 * - Manchester
 * http://mchr3k.github.io/arduino-libs-manchester/
 * https://github.com/mchr3k/arduino-libs-manchester
 * 
 * Le variabili "data" 0 ed 1, DEVONO essere scritte su piu`
 * righe, non so perche`, ma se non si fa` cosi`, 
 * non funziona !!
 * Mi sono spinto fino a 28 (32) caratteri e sembra funzionare,
 * ma ho preferito optare per una soluzione piu` "snella" 
 * (per ora).
 * 
 * Forse meglio avere delay differenti per l'invio dati
 * fra i vari sensori, purtroppo la funzione random porta
 * via troppi bit di programma e non e` stato possibile
 * implementarla.
 * Per questo ho aggiunto la variabile "Tinvio".
 * 
 * L'invio del dato viene effettuato 10 volte, per sicurezza,
 * sin'ora ci sono stati alcuni disturbi e/o malfunzionamenti
 * che hanno pregiudicato spoesso la ricezione del dato, con
 * 10 tentativi, qualcosa e` sempre arrivato ;P
 * (adesso par funzionare decisamente meglio, ma qualche
 * trasmissione viene sempre persa).
 * 
 * Eliminare comando led nella versione finale ?
 * (per risparmiare energia)
 * 
 * Aggiungere pulsante di test nella versione finale.
 * 
 */

// Manchester library
#include <Manchester.h>


#define TX_PIN  4  // pin where your transmitter is connected
#define LED_PIN 3  // pin for blinking LED
#define PIR_PIN 0  // pin where your PIR is connected

// Tempo ritardo "riavvio", dopo invio di un dato, e` in millesimi di secondo.
int restart=10000;  // 1000 = 1s
// Tempo ritardo invio fra dati
int Tinvio=100;

int PIRstate;            // the current reading from the input PIR

// Cambiare valore ID, adesso e` PIR1, mettre 2 se secondo, 3 terzo e cosi` via
// (o altre numerazioni/sigle differenti, per discriminare il sensore)
// dato da inviare, 0 per segnale pir basso, 1 per segnale pir alto
uint8_t data0[8] = {8,
                   'P', 'I', 'R', '1',
                   ',', '0'
                   };
uint8_t data1[8] = {8,
                   'P', 'I', 'R', '1',
                   ',', '1'
                   };


void setup() {
  // put your setup code here, to run once:
  //Serial.begin(9600);
  
  pinMode(PIR_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);

  /* 
   * Funziona se disattivata la linea successiva e se a 600 la velocita` 
   */
  //man.workAround1MhzTinyCore(); //add this in order for transmitter to work with 1Mhz Attiny85/84
  man.setupTransmit(TX_PIN, MAN_600);
}


void loop() {
  // legge lo stato del PIR
  int reading = digitalRead(PIR_PIN);

  // se l'ingresso e` cambiato
  if (reading != PIRstate) {
    PIRstate = reading;
    
    if (PIRstate == LOW) {
      digitalWrite(LED_PIN, LOW);
      for (int i=1; i<=10; i++) {
        man.transmitArray(8, data0);
        delay(Tinvio);
      }
    delay(restart);
    }
    if (PIRstate == HIGH) {
      digitalWrite(LED_PIN, HIGH);
      for (int i=1; i<=10; i++) {
        man.transmitArray(8, data1);
        delay(Tinvio);
      }
    delay(restart);
    }
  }
}
