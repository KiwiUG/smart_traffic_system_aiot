// Define the pins for each lane: {Red, Yellow, Green}
const int PIN_MAPPING[4][3] = {
  {2, 3, 4},   // Lane 1 (Primary)
  {5, 6, 7},   // Lane 2 (Right)
  {11, 12, 13}, // Lane 3 (Opposite)
  {8, 9, 10}    // Lane 4 (Left)
};

int currentGreenLane = -1; // -1 means all red (initial state)

void setup() {
  // Initialize all pins
  for (int lane = 0; lane < 4; lane++) {
    pinMode(PIN_MAPPING[lane][0], OUTPUT);  // Red
    pinMode(PIN_MAPPING[lane][1], OUTPUT);  // Yellow
    pinMode(PIN_MAPPING[lane][2], OUTPUT);  // Green
    digitalWrite(PIN_MAPPING[lane][0], HIGH); // Start with Red
    digitalWrite(PIN_MAPPING[lane][1], LOW);  // Yellow off
    digitalWrite(PIN_MAPPING[lane][2], LOW);  // Green off
  }
  Serial.begin(9600);
  Serial.println("Traffic Light Controller Ready!");
}

void loop() {
  if (Serial.available() > 0) {
    int command = Serial.parseInt(); // Read command (1-4 or -1 for emergency stop)
    clearSerialBuffer();

    // Emergency stop: all red
    if (command == -1) {
      setAllRed();
      return;
    }

    // Valid lane command (1-4)
    if (command >= 1 && command <= 4) {
      int newLane = command - 1; // Convert to 0-based index

      // If same lane, do nothing
      if (newLane == currentGreenLane) {
        return;
      }

      // If different lane, transition
      if (currentGreenLane != -1) {
        // Turn current green lanes to yellow, then red
        transitionToRed(currentGreenLane);
      }

      // Turn new lane + opposite to green
      setLaneGreen(newLane);
    }
  }
}

// Clear serial buffer to avoid leftover data
void clearSerialBuffer() {
  while (Serial.available() > 0) {
    Serial.read();
  }
}

// Set all lanes to red (emergency stop)
void setAllRed() {
  for (int lane = 0; lane < 4; lane++) {
    digitalWrite(PIN_MAPPING[lane][0], HIGH); // Red on
    digitalWrite(PIN_MAPPING[lane][1], LOW);  // Yellow off
    digitalWrite(PIN_MAPPING[lane][2], LOW);  // Green off
  }
  currentGreenLane = -1; // Reset state
  Serial.println("EMERGENCY STOP: All Red");
}

// Transition from green → yellow → red
void transitionToRed(int lane) {
  int oppositeLane = getOppositeLane(lane);

  // Turn green → yellow
  digitalWrite(PIN_MAPPING[lane][2], LOW);     // Green off
  digitalWrite(PIN_MAPPING[lane][1], HIGH);    // Yellow on
  digitalWrite(PIN_MAPPING[oppositeLane][2], LOW);
  digitalWrite(PIN_MAPPING[oppositeLane][1], HIGH);

  delay(2000); // Yellow light duration (2 sec)

  // Turn yellow → red
  digitalWrite(PIN_MAPPING[lane][1], LOW);     // Yellow off
  digitalWrite(PIN_MAPPING[lane][0], HIGH);    // Red on
  digitalWrite(PIN_MAPPING[oppositeLane][1], LOW);
  digitalWrite(PIN_MAPPING[oppositeLane][0], HIGH);

  Serial.print("Lane ");
  Serial.print(lane + 1);
  Serial.println(" and opposite now Red");
}

// Set a lane + its opposite to green
void setLaneGreen(int lane) {
  int oppositeLane = getOppositeLane(lane);

  // Turn red → green
  digitalWrite(PIN_MAPPING[lane][0], LOW);     // Red off
  digitalWrite(PIN_MAPPING[lane][2], HIGH);    // Green on
  digitalWrite(PIN_MAPPING[oppositeLane][0], LOW);
  digitalWrite(PIN_MAPPING[oppositeLane][2], HIGH);

  currentGreenLane = lane; // Update current active lane

  Serial.print("Lane ");
  Serial.print(lane + 1);
  Serial.println(" and opposite now Green");
}

// Get the opposite lane (0 ↔ 2, 1 ↔ 3)
int getOppositeLane(int lane) {
  const int OPPOSITE_MAP[4] = {2, 3, 0, 1};
  return OPPOSITE_MAP[lane];
}

//Simulation Code:

// Pin definitions for each lane: {Red, Yellow, Green}
const int PIN_MAPPING[4][3] = {
  {2, 3, 4},   // Lane 1
  {5, 6, 7},   // Lane 2
  {11, 12, 13}, // Lane 3
  {8, 9, 10}    // Lane 4
};

// Timing constants (milliseconds)
const unsigned long ALL_RED_DURATION = 5000;    // 5 seconds
const unsigned long GREEN_DURATION = 30000;     // 30 seconds
const unsigned long YELLOW_DURATION = 3000;     // 3 seconds

void setup() {
  // Initialize all pins
  for (int lane = 0; lane < 4; lane++) {
    for (int light = 0; light < 3; light++) {
      pinMode(PIN_MAPPING[lane][light], OUTPUT);
      digitalWrite(PIN_MAPPING[lane][light], LOW);
    }
  }
  
  // Start with all red
  setAllRed();
  delay(ALL_RED_DURATION);
}

void loop() {
  // Phase 1: Lanes 1 & 3 green
  setLanesGreen(0, 2);          // Lane 1 and 3
  delay(GREEN_DURATION);
  transitionToRed(0, 2);        // Yellow then red
  
  // Phase 2: Lanes 2 & 4 green
  setLanesGreen(1, 3);          // Lane 2 and 4
  delay(GREEN_DURATION);
  transitionToRed(1, 3);        // Yellow then red
}

// Set all lanes to red
void setAllRed() {
  for (int lane = 0; lane < 4; lane++) {
    digitalWrite(PIN_MAPPING[lane][0], HIGH); // Red on
    digitalWrite(PIN_MAPPING[lane][1], LOW);  // Yellow off
    digitalWrite(PIN_MAPPING[lane][2], LOW);  // Green off
  }
}

// Set specific lanes to green
void setLanesGreen(int laneA, int laneB) {
  setAllRed(); // First ensure all are red
  
  // Turn on green for specified lanes
  digitalWrite(PIN_MAPPING[laneA][0], LOW);   // Red off
  digitalWrite(PIN_MAPPING[laneA][2], HIGH);  // Green on
  
  digitalWrite(PIN_MAPPING[laneB][0], LOW);   // Red off
  digitalWrite(PIN_MAPPING[laneB][2], HIGH);  // Green on
}

// Transition lanes from green to red (via yellow)
void transitionToRed(int laneA, int laneB) {
  // Turn green to yellow
  digitalWrite(PIN_MAPPING[laneA][2], LOW);   // Green off
  digitalWrite(PIN_MAPPING[laneA][1], HIGH);  // Yellow on
  
  digitalWrite(PIN_MAPPING[laneB][2], LOW);   // Green off
  digitalWrite(PIN_MAPPING[laneB][1], HIGH);  // Yellow on
  
  delay(YELLOW_DURATION);
  
  // Turn yellow to red
  digitalWrite(PIN_MAPPING[laneA][1], LOW);   // Yellow off
  digitalWrite(PIN_MAPPING[laneA][0], HIGH);  // Red on
  
  digitalWrite(PIN_MAPPING[laneB][1], LOW);   // Yellow off
  digitalWrite(PIN_MAPPING[laneB][0], HIGH);  // Red on
}