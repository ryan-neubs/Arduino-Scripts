// colorwheelLED.ino
// By Ryan Neubauer
// This script uses a joystick like an RGB colorwheel for RGB LEDs.
// Note: If you don't have an RGB LED, you can use indivudal red, green, and blue LEDS.
// That's what I had to do when I forgot to use resistors and burnt my RGB LED bulbs. Whoops...

int redPin = 11;
int greenPin = 10;
int bluePin = 9;
int xAxis = A0;
int yAxis = A1;
int r, g, b;


void setup() {
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  pinMode(xAxis, INPUT);
  pinMode(yAxis, INPUT);
  Serial.begin(9600);
}

float getAngle(int x, int y){
  float angle = atan2(x, y) * (180 / PI);
  if (angle < 0) { // Normalizing the angle range so we aren't given a negative angle.
    angle += 360.0;
  }
  return angle;
}

float getSaturation(float x, float y) {
  float distance = sqrt(sq(x) + sq(y));
  float radius = 512.0;
  if (distance <= 0) {
    return 0.0;
  }
  return min(distance/radius, 1.0);
}

void hsvToRGB(float hue, float saturation, float value, int &r, int &g, int &b) {
  float chroma = value * saturation; 
  int sector = int(hue/60);
  float x = chroma * (1.0 - abs(fmod((hue/60.0), 2.0) - 1.0)); // Calculate the adjuste chroma or 'intermediate value'
  float m = value - chroma;
  float r1 = 0, g1 = 0, b1 = 0;

  if (sector == 0) {
    r1 = chroma;
    g1 = x;
    b1 = 0;
  } else if (sector == 1) {
    r1 = x;
    g1 = chroma;
    b1 = 0;
  } else if (sector == 2) {
    r1 = 0;
    g1 = chroma;
    b1 = x;
  } else if (sector == 3) {
    r1 = 0;
    g1 = x;
    b1 = chroma;
  } else if (sector == 4) {
    r1 = x;
    g1 = 0;
    b1 = chroma;
  } else {
    r1 = chroma;
    g1 = 0;
    b1 = x;
  }

  r = (int)constrain((r1 + m) * 255.0, 0, 255);
  g = (int)constrain((g1 + m) * 255.0, 0, 255);
  b = (int)constrain((b1 + m) * 255.0, 0, 255);

}

void loop() {
  // put your main code here, to run repeatedly:
  int xRead = analogRead(xAxis);
  int yRead = analogRead(yAxis);

  float xRange = map(xRead, 0, 1023, -512.0, 512.0);
  float yRange = -map(yRead, 0, 1023, -512.0, 512.0);

  if (abs(xRange) <= 5.0) {
    xRange = 0.0;
  } 
  if (abs(yRange) <= 5.0) {
    yRange = 0.0;
  }

  float joystickAngle = getAngle(xRange, yRange);

  float hue = joystickAngle; // hue directly corresponds to the 6 color sections. It will directly correspond to the angle
  float saturation = getSaturation(xRange, yRange); // This controls color intensity. This value will correspond to the distance from the center of the color wheel.
  const float value = 1.0; // Value is the 'brightness' of our color. We don't want to manipulate this for our color wheel

  hsvToRGB(hue, saturation, value, r, g, b);

  analogWrite(redPin, r);
  analogWrite(greenPin, g);
  analogWrite(bluePin, b);

  Serial.print("X coord: ");
  Serial.print(xRange);
  Serial.print(" Y coord: ");
  Serial.print(yRange);
  Serial.print(" Hue/Angle: ");
  Serial.print(hue);
  Serial.print(" Saturation: ");
  Serial.print(saturation);
  Serial.print(" R: ");
  Serial.print(r);
  Serial.print(" G: ");
  Serial.print(g);
  Serial.print(" B: ");
  Serial.print(b);
  Serial.print("\n");

  delay(25);

}
