// C++ code
//
void setup()
{
  pinMode(13, OUTPUT);
}

void loop()
{
  digitalWrite(13, HIGH);
  delay(1000); // Wait for 1000 millisecond(s)
  digitalWrite(13, LOW);
  delay(1000); // Wait for 1000 millisecond(s)
  digitalWrite(13, HIGH);
  delay(1000); // Wait for 1000 millisecond(s)
  digitalWrite(13, LOW);
  delay(1000); // Wait for 1000 millisecond(s)
}

// Pins for the traffic lights
const int RED_PIN[4] = {2, 3, 4, 5};
const int YELLOW_PIN[4] = {6, 7, 8, 9};
const int GREEN_PIN[4] = {10, 11, 12, 13};

// Pins for the ultrasonic sensors
const int TRIGGER_PIN[4] = {22, 23, 24, 25};
const int ECHO_PIN[4] = {26, 27, 28, 29};

// Pins for the IR sensors
const int IR_PIN[12] = {30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41};
const int NUM_IR_SENSORS = 12;

// Pin for the pedestrian button and crossing light
const int PED_PIN = 42;
const int PED_CROSSING_PIN = 43;

// Traffic count array for each road
int traffic_count[4] = {0, 0, 0, 0};

// Constants for each IR sensor on each road
const int P1_IR = 0;
const int P2_IR = 1;
const int P3_IR = 2;
const int P4_IR = 3;

void setup() {
  // Initialize the traffic light pins as outputs
  for (int i = 0; i < 4; i++) {
    pinMode(RED_PIN[i], OUTPUT);
    pinMode(YELLOW_PIN[i], OUTPUT);
    pinMode(GREEN_PIN[i], OUTPUT);
  }

  // Initialize the ultrasonic sensor pins
  for (int i = 0; i < 4; i++) {
    pinMode(TRIGGER_PIN[i], OUTPUT);
    pinMode(ECHO_PIN[i], INPUT);
  }

  // Initialize the IR sensor pins
  for (int i = 0; i < NUM_IR_SENSORS; i++) {
    pinMode(IR_PIN[i], INPUT);
  }

  // Initialize the pedestrian button and crossing light pins
  pinMode(PED_PIN, INPUT_PULLUP);
  pinMode(PED_CROSSING_PIN, OUTPUT);

  // Set the initial state of the traffic lights
  digitalWrite(RED_PIN[0], HIGH);
  digitalWrite(GREEN_PIN[1], HIGH);
}

void loop() {
  // Measure the distance for each ultrasonic sensor
  for (int i = 0; i < 4; i++) {
    // Send a pulse to the ultrasonic sensor
    digitalWrite(TRIGGER_PIN[i], LOW);
    delayMicroseconds(2);
    digitalWrite(TRIGGER_PIN[i], HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIGGER_PIN[i], LOW);

    // Measure the duration of the echo pulse
    long duration = pulseIn(ECHO_PIN[i], HIGH);

    // Calculate the distance based on the duration
    int distance = duration * 0.034 / 2;

    // Increment the traffic count for the corresponding road
    if (distance < 50) {
      traffic_count[i]++;
    }
  }

  // Check if the pedestrian button is pressed
  if (digitalRead(PED_PIN) == HIGH) {
    // If the pedestrian button is pressed, turn on the pedestrian crossing light
    digitalWrite(PED_CROSSING_PIN, HIGH);
  } else {
    digitalWrite(PED_CROSSING_PIN, LOW);
  }

  // Determine the road with the maximum traffic count
  int max_traffic_road = 0;
  int max_traffic_count = 0;

  for (int i = 0; i < 4; i++) {
if (traffic_count[i] > max_traffic_count) {
max_traffic_road = i;
max_traffic_count = traffic_count[i];
}
}

// Calculate the total traffic count for all roads
int total_traffic_count = traffic_count[0] + traffic_count[1] + traffic_count[2] + traffic_count[3];

// Calculate the percentage of traffic for each road
int percentage_traffic[4];
for (int i = 0; i < 4; i++) {
percentage_traffic[i] = traffic_count[i] * 100 / total_traffic_count;
}

// Determine the maximum percentage of traffic among all roads
int max_percentage_traffic = 0;
for (int i = 0; i < 4; i++) {
if (percentage_traffic[i] > max_percentage_traffic) {
max_percentage_traffic = percentage_traffic[i];
}
}

// Determine the congestion level based on the maximum percentage of traffic
int congestion_level;
if (max_percentage_traffic <= 25) {
congestion_level = 0; // Low congestion
} else if (max_percentage_traffic <= 50) {
congestion_level = 1; // Moderate congestion
} else if (max_percentage_traffic <= 75) {
congestion_level = 2; // High congestion
} else {
congestion_level = 3; // Very high congestion
}

// Control the traffic lights based on the congestion level and traffic count for each road
if (congestion_level == 0) {
// If the congestion level is low, cycle through all traffic lights at equal intervals
for (int i = 0; i < 4; i++) {
digitalWrite(GREEN_PIN[i], HIGH);
delay(5000);
digitalWrite(GREEN_PIN[i], LOW);
digitalWrite(YELLOW_PIN[i], HIGH);
delay(2000);
digitalWrite(YELLOW_PIN[i], LOW);
digitalWrite(RED_PIN[i], HIGH);
}
} else {
// If the congestion level is moderate, high, or very high, prioritize the road with the maximum traffic count
digitalWrite(GREEN_PIN[max_traffic_road], HIGH);
delay(5000);
digitalWrite(GREEN_PIN[max_traffic_road], LOW);
digitalWrite(YELLOW_PIN[max_traffic_road], HIGH);
delay(2000);
digitalWrite(YELLOW_PIN[max_traffic_road], LOW);
digitalWrite(RED_PIN[max_traffic_road], HIGH);
// Turn on the green light for the other roads if they have low traffic
for (int i = 0; i < 4; i++) {
  if (i != max_traffic_road && percentage_traffic[i] < 25) {
    digitalWrite(GREEN_PIN[i], HIGH);
    delay(5000);
    digitalWrite(GREEN_PIN[i], LOW);
    digitalWrite(YELLOW_PIN[i], HIGH);
    delay(2000);
    digitalWrite(YELLOW_PIN[i], LOW);
    digitalWrite(RED_PIN[i], HIGH);
  }
}
}

// Reset the traffic count after one cycle
for (int i = 0; i < 4; i++) {
traffic_count[i] = 0;
}
}
