#include <Servo.h>

Servo servoScan; // Lower scanning servo
Servo servoFire; // Upper servo with fan

int flamePin = A0; 
int relayPin = 7;

// --- RGB LED PIN DEFINITIONS ---
// Common Cathode: LOW for ON, HIGH for OFF
int redPin = 4; 
int greenPin = 3;
int bluePin = 5; 
// -------------------------------

int scanAngle = 30;
int direction = 1;

int threshold = 300; // Adjust after testing

bool fireDetected = false;

void setup() {
    Serial.begin(9600);

    servoScan.attach(9);
    servoFire.attach(10);
    pinMode(relayPin, OUTPUT);

    pinMode(redPin, OUTPUT);
    pinMode(greenPin, OUTPUT);
    pinMode(bluePin, OUTPUT);

    digitalWrite(relayPin, LOW);

    servoScan.write(scanAngle);
    servoFire.write(90);

    // Normal (green)
    setLedColor(0, 1, 0); 
}

void loop() {

    int flameValue = analogRead(flamePin);
    Serial.println(flameValue);

    // ---------- FIRE DETECTED ----------
    if (flameValue < threshold) {

        fireDetected = true;

        // LED RED
        setLedColor(1, 0, 0);

        // Stop lower servo at fire position
        servoScan.write(scanAngle);

        // Move upper servo to detected direction
        servoFire.write(scanAngle);

        // Turn ON relay + fan
        digitalWrite(relayPin, HIGH);

        // *** Run the fan for EXACTLY 2 seconds ***
        delay(2000);

        // Turn OFF relay after 2 seconds
        digitalWrite(relayPin, LOW);

        // After action, return upper servo to center
        servoFire.write(90);

        // Reset LED to green
        setLedColor(0, 1, 0);

        // Stop this loop iteration and continue scanning
        return;
    }

    // ---------- NO FIRE ----------
    fireDetected = false;
    digitalWrite(relayPin, LOW);
    servoFire.write(90);

    setLedColor(0, 1, 0);

    // ---------- SCANNING MOVEMENT ----------
    scanAngle += direction;

    if (scanAngle >= 150) direction = -1;
    if (scanAngle <= 30) direction = 1;

    servoScan.write(scanAngle);
    delay(30);
}


// --- RGB LED helper (Common Cathode) ---
void setLedColor(int red, int green, int blue) {
    digitalWrite(redPin, red ? LOW : HIGH);
    digitalWrite(greenPin, green ? LOW : HIGH);
    digitalWrite(bluePin, blue ? LOW : HIGH);
}
