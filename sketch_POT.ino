int Notes[] = {659, 659, 0, 659, 0, 523, 659, 0, 784};
int Duration[] = {100, 100, 100, 100, 100, 100, 100, 100, 200};

const int antenna = 39;         // pin for wand
const int poten = 34;           // Potentiometer for sensitivity
const int temp = 32;            // Thermistor
const int button = 14;          // Mute Button
const int vibMotor = 13;        // Haptic Motor
const int buzz = 25;            // Passive Buzzer
const int LED[] = {18, 19, 21}; // LEDs

boolean isMuted = false;
boolean lastButton = HIGH;

void setup() {
  Serial.begin(115200); 
  
  pinMode(button, INPUT_PULLUP);
  pinMode(vibMotor, OUTPUT);
  pinMode(buzz, OUTPUT);
  
  for (int i = 0; i < 3; i++) {
    pinMode(LED[i], OUTPUT);
  }

  // Startup melody
  for (int i = 0; i < 9; i++){
    if (Notes[i] == 0) {
      noTone(buzz);
    } else {
      tone(buzz, Notes[i], Duration[i]);
    }
    delay(Duration[i] * 1.2);
  }
}

void loop() {
  
  if (Serial.available() > 0) {
    char incoming = Serial.read(); 

    if (incoming == 'm' || incoming == 'M') {
      isMuted = !isMuted;
      Serial.print("KEYBOARD COMMAND: Mute is ");
      Serial.println(isMuted ? "ON" : "OFF");
    }

    if (incoming == '1') playMelody(1);
    if (incoming == '2') playMelody(2); 
    if (incoming == '3') playMelody(3); 
  }

 
  boolean currentButton = digitalRead(button);
  if (lastButton == HIGH && currentButton == LOW) {
    isMuted = !isMuted; 
    delay(50); // Debounce
  }
  lastButton = currentButton;

  int emiValue = analogRead(antenna);
  int threshold = analogRead(poten);
  int tempValue = analogRead(temp);

 
  Serial.print("EMI: "); Serial.print(emiValue);
  Serial.print(" | THR: "); Serial.print(threshold);
  Serial.print(" | TEMP: "); Serial.println(tempValue);

  if (emiValue > threshold) {
    digitalWrite(LED[0], HIGH);
    
    if (!isMuted) {
      digitalWrite(vibMotor, HIGH);
      tone(buzz, 440, 20); 
      delay(25);
      tone(buzz, 466, 20);
    }

    if (emiValue > (threshold + 500)) digitalWrite(LED[1], HIGH);
    else digitalWrite(LED[1], LOW);

    if (emiValue > (threshold + 1000)) digitalWrite(LED[2], HIGH);
    else digitalWrite(LED[2], LOW);

  } else {
    for (int i = 0; i < 3; i++) digitalWrite(LED[i], LOW);
    digitalWrite(vibMotor, LOW);
    noTone(buzz);
  }

  delay(40); 
}


void playMelody(int choice) {
  if (choice == 1) {                                        // Alert
    tone(buzz, 880, 100); delay(150); tone(buzz, 440, 100);
  } 
  else if (choice == 2) {                                   // Discovery 
    tone(buzz, 523, 100); delay(100); tone(buzz, 784, 100);
  }
  else if (choice == 3) {                                    // High Intensity Warning
    for(int i=0; i<3; i++) { tone(buzz, 1000, 50); delay(70); }
  }
  noTone(buzz); 
}