#include <Servo.h>
#include <Keypad.h>
Servo ServoMotor;
char* password1 = "23169416";
char* password2 = "0000";
String data;
int position = 0;
unsigned long unlockTime = 0; // Variable pour stocker le temps de déverrouillage

const byte ROWS = 5; // nombre de lignes du clavier
const byte COLS = 4; // nombre de colonnes du clavier

char keys[ROWS][COLS] = {
  {'F','F','#','*'},
  {'1','2','3','^'},
  {'4','5','6','v'},
  {'7','8','9','C'},
  {'<','0','>','Ent'}
};

byte rowPins[ROWS] = {2,3,4,5,6}; //connect to the row pinouts of the kpd
byte colPins[COLS] = {10,9,8,7}; //connect to the column pinouts of the kpd
char digitArray[13]; // to store up to 12 characters entered and null character
int digitCount = 0; // to keep track of the number of characters entered

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

int RedpinLock = 12;
int GreenpinUnlock = 13;

String enteredCode = ""; // Variable pour stocker le code saisi

void setup() {
  ServoMotor.attach(11);
  pinMode(RedpinLock, OUTPUT);
  pinMode(GreenpinUnlock, OUTPUT);
  LockedPosition(true);
  Serial.begin(9600); // Initialise la communication série
}

void loop() {
  char key = keypad.getKey();
  
  if (key != NO_KEY) {
    //Serial.println(key); // Affiche la valeur de la touche dans le moniteur série

    if (key == 't') { // Si la touche 't' est pressée, vérifie le code saisi
   digitArray[digitCount] = '\0'; // set last character to null
      Serial.write(digitArray,digitCount);
    digitCount = 0; // reset digitCount
    for (int i = 0; i < 13; i++) { // clear digitArray
      digitArray[i] = '\0';
    }


    } else if (key == 'C') { // Si la touche 'C' est pressée, réinitialise le code saisi
      enteredCode = "";
          for (int i = 0; i < 13; i++) { // clear digitArray
      digitArray[i] = '\0';
    }
    } else if (key != 'Ent') { // Ignore la touche 'Ent' (Entrée)
      enteredCode += key; // Ajoute la touche saisie au code entré
          digitArray[digitCount] = key;
    digitCount++;
    }
  }
    if (Serial.available()) { // if serial data is available
    char read = Serial.read(); // read the incoming byte from serial
    if (read == '1') { // if the byte is '0'
      checkPassword(1); // Appelle la fonction pour vérifier le code saisi
    }
    else if (read == '0')
    {
    digitalWrite(RedpinLock, LOW);
    delay(100);
    digitalWrite(RedpinLock, HIGH);
    delay(100);
    digitalWrite(RedpinLock, LOW);
    delay(100);
    digitalWrite(RedpinLock, HIGH);
    delay(100);
    digitalWrite(RedpinLock, LOW);
    delay(100);
    digitalWrite(RedpinLock, HIGH);


      }
  }




  // Vérifie si le déverrouillage est en cours et si le temps écoulé dépasse 5 secondes
  if (millis() - unlockTime > 5000) {
    LockedPosition(true); // Verrouille à nouveau après 5 secondes
  }

  delay(100);
}

void checkPassword(int j) {
  if (j==1) { // Vérifie si le code saisi correspond à l'un des mots de passe
   // Serial.println("Code correct !");
    unlockTime = millis(); // Enregistre le moment du déverrouillage
    position = 0; // Réinitialise la position si le code est correct
    LockedPosition(false); // Déverrouille
    data = enteredCode; // Stocke le code saisi dans la variable data
    //Serial.println(data); // Affiche le code saisi dans le moniteur série
  } else {
    //Serial.println("Code incorrect !");
    position = 0; // Réinitialise la position si le code est incorrect
    LockedPosition(true); // Verrouille à nouveau
  }
  enteredCode = ""; // Réinitialise le code saisi
}

void LockedPosition(int locked) {
  if (locked) {
    digitalWrite(RedpinLock, HIGH);
    digitalWrite(GreenpinUnlock, LOW);
    ServoMotor.write(11);
  } else {
    digitalWrite(RedpinLock, LOW);
    digitalWrite(GreenpinUnlock, HIGH);
    ServoMotor.write(90);
  }
}
