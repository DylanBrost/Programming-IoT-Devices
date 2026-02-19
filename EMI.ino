
int Notes[] = {659, 659, 0, 659, 0, 523, 659, 0, 784};
int Duration[] = {100, 100, 100, 100, 100, 100, 100, 100, 200};

const int antenna = 36;        // VP Pin for wand
const int poten = 34;          // Potentiometer for sensitivity
const int temp = 32;           // Thermistor
const int button = 14;         // Mute Button
const int vibMotor = 13;       // Haptic Motor
const int buzz = 25;           // Passive Buzzer
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
  //startup melody
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
  
  boolean currentButton = digitalRead(button);
  if (lastButton == HIGH && currentButton == LOW) {
    isMuted = !isMuted; 
    delay(50); // Debounce delay
  }
  lastButton = currentButton;

  
  int emiValue = analogRead(antenna);
  int threshold = analogRead(poten);
  int tempValue = analogRead(temp);


  Serial.print("EMI: "); Serial.print(emiValue);
  Serial.print(" | THR: "); Serial.print(threshold);
  Serial.print(" | TEMP: "); Serial.println(tempValue);


  if (emiValue > threshold) {
    // Level 1: First LED
    digitalWrite(LED[0], HIGH);
    
    if (!isMuted) {
  digitalWrite(vibMotor, HIGH);
  tone(buzz, 440, 20); 
  delay(25);
  tone(buzz, 466, 20);
}

    //Level 2: Two LEDS
    if (emiValue > (threshold + 500)) {
      digitalWrite(LED[1], HIGH);
    } else {
      digitalWrite(LED[1], LOW);
    }

    // Level 3: Third LED
    if (emiValue > (threshold + 1000)) {
      digitalWrite(LED[2], HIGH);
    } else {
      digitalWrite(LED[2], LOW);
    }

  } else {
    // All outputs OFF if signal is below threshold
    for (int i = 0; i < 3; i++) digitalWrite(LED[i], LOW);
    digitalWrite(vibMotor, LOW);
    noTone(buzz);
  }

  delay(40); // Scan rate
}