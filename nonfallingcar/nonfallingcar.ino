// -------- IR --------
int ir1 = 7;   // LEFT IR
int ir2 = 8;   // RIGHT IR
int ir3 = A2;  // BACK IR
// -------- LED --------
int led1 = A0;
int led2 = A1;
int led3 = 3;
// -------- ULTRASONIC --------
int trigPin = 9;
int echoPin = 10;
// -------- MOTOR --------
int ENA = 11;
int IN1 = 4;
int IN2 = 2;
int ENB = 6;
int IN3 = 12;
int IN4 = 13;
long duration;
int distance;
void setup() {
pinMode(ir1, INPUT);
pinMode(ir2, INPUT);
pinMode(ir3, INPUT);
pinMode(led1, OUTPUT);
pinMode(led2, OUTPUT);
pinMode(led3, OUTPUT);
pinMode(trigPin, OUTPUT);
pinMode(echoPin, INPUT);
pinMode(ENA, OUTPUT);
pinMode(IN1, OUTPUT);
pinMode(IN2, OUTPUT);
pinMode(ENB, OUTPUT);
pinMode(IN3, OUTPUT);
pinMode(IN4, OUTPUT);
digitalWrite(ENA, LOW);
digitalWrite(ENB, LOW);
digitalWrite(IN1, LOW);
digitalWrite(IN2, LOW);
digitalWrite(IN3, LOW);
digitalWrite(IN4, LOW);
}
int readDistance() {
digitalWrite(trigPin, LOW);
delayMicroseconds(4);
digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);
long d = pulseIn(echoPin, HIGH, 30000);
if (d == 0) return 999;
return d * 0.034 / 2;
}
// ===== DEBOUNCED IR =====
bool readIRDebounced(int pin) {
int count = 0;
for (int i = 0; i < 5; i++) {
if (digitalRead(pin) == LOW) count++;
delay(6);
}
return (count >= 4);
}
void loop() {
  distance = readDistance();
// ===== IR READ =====
bool leftOK  = readIRDebounced(ir1);
bool rightOK = readIRDebounced(ir2);
bool backOK  = readIRDebounced(ir3);
digitalWrite(led1, leftOK  ? HIGH : LOW);
digitalWrite(led2, rightOK ? HIGH : LOW);
// ===== 1. OBSTACLE — highest priority =====
if (distance < 10) {
digitalWrite(led3, HIGH);    // ON — object detected
hardStop();
delay(300);
reverseMotor();
delay(400);
hardStop();
delay(100);
turnLeft();
delay(350);
hardStop();
delay(100);
digitalWrite(led3, LOW);     // OFF — obstacle handled
return;
}
digitalWrite(led3, LOW);       // OFF — no obstacle
// ===== 2. BACK IR — no surface at back =====
if (!backOK) {
hardStop();
delay(3000);
forwardMotor();
delay(600);
hardStop();
delay(100);
return;
}
// ===== 3. FRONT/SIDE EDGE =====
if (!leftOK || !rightOK) {
hardStop();
delay(150);
reverseMotor();
delay(500);
hardStop();
delay(100);
turnLeft();
delay(1500);
hardStop();
delay(100);
bool leftNow  = readIRDebounced(ir1);
bool rightNow = readIRDebounced(ir2);
if (!leftNow || !rightNow) {
reverseMotor();
delay(400);
hardStop();
delay(100);
turnRight();
delay(800);
hardStop();
delay(100);
}
return;
}
// ===== 4. NORMAL FORWARD =====
forwardMotor();
}
// ======== MOTOR FUNCTIONS ========
void hardStop() {
digitalWrite(IN1, LOW);
digitalWrite(IN2, LOW);
digitalWrite(IN3, LOW);
digitalWrite(IN4, LOW);
digitalWrite(ENA, LOW);
digitalWrite(ENB, LOW);
}
void forwardMotor() {
digitalWrite(IN1, HIGH);
digitalWrite(IN2, LOW);
digitalWrite(IN3, HIGH);
digitalWrite(IN4, LOW);
analogWrite(ENA, 70);
analogWrite(ENB, 70);
}
void reverseMotor() {
digitalWrite(IN1, LOW);
digitalWrite(IN2, HIGH);
digitalWrite(IN3, LOW);
digitalWrite(IN4, HIGH);
analogWrite(ENA, 75);
analogWrite(ENB, 75);
}
void turnLeft() {
digitalWrite(IN1, LOW);
digitalWrite(IN2, HIGH);
analogWrite(ENA, 70);
digitalWrite(IN3, HIGH);
digitalWrite(IN4, LOW);
analogWrite(ENB, 70);
}
void turnRight() {
digitalWrite(IN1, HIGH);
digitalWrite(IN2, LOW);
analogWrite(ENA, 70);
digitalWrite(IN3, LOW);
digitalWrite(IN4, HIGH);
analogWrite(ENB, 70);
}