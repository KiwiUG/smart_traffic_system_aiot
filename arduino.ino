const int PIN_MAPPING[4][2] = {
  // {Red, Green} pins for each lane
  {8, 10},  // Lane 1 (Primary)
  {12, 6},  // Lane 2 (Right)
  {4, 5},   // Lane 3 (Opposite)
  {7, 9}    // Lane 4 (Left)
};

void setup() {
  // Initialize all pins
  for (int lane = 0; lane < 4; lane++) {
      pinMode(PIN_MAPPING[lane][0], OUTPUT);
      pinMode(PIN_MAPPING[lane][1], OUTPUT);
      digitalWrite(PIN_MAPPING[lane][0], HIGH); // Start red
      digitalWrite(PIN_MAPPING[lane][1], LOW);
  }
  Serial.begin(9600);
}

void loop() {
  if (Serial.available() > 0) {
      int command = Serial.parseInt();
      
      // Emergency stop
      if (command == -1) {
          setAllRed();
          return;
      }
      
      // Valid lane command
      if (command >= 1 && command <= 4) {
          changeLights(command - 1); // Convert to 0-based index
      }
  }
}

void setAllRed() {
  for (int lane = 0; lane < 4; lane++) {
      digitalWrite(PIN_MAPPING[lane][0], HIGH);
      digitalWrite(PIN_MAPPING[lane][1], LOW);
  }
}

void changeLights(int lane) {
  setAllRed();
  delay(500); // All-red interval
  
  digitalWrite(PIN_MAPPING[lane][0], LOW);
  digitalWrite(PIN_MAPPING[lane][1], HIGH);
  
  // Also activate opposite lane
  int opposite_lane = getOppositeLane(lane);
  digitalWrite(PIN_MAPPING[opposite_lane][0], LOW);
  digitalWrite(PIN_MAPPING[opposite_lane][1], HIGH);
}

int getOppositeLane(int lane) {
  const int OPPOSITE_MAP[4] = {2, 3, 0, 1}; // 0↔2, 1↔3
  return OPPOSITE_MAP[lane];
}