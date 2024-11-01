#include "arduino_secrets.h"
#include "thingProperties.h"
#define relayPin 2  
#define wet 760
#define dry 980
const int thermistorPin = A1;
const float R1 = 10000;
const float T0 = 298.15;
const float B = 3950;  
int val;
void setup() {
  Serial.begin(9600);
  delay(1500);
  
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, HIGH);

  initProperties();

  ArduinoCloud.begin(ArduinoIoTPreferredConnection);

  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();
}

void loop() {
  ArduinoCloud.update();
  
  // Read and map the sensor value to the range 0-100
  val = analogRead(A0);
  sensor = map(constrain(val, wet, dry), dry, wet, 0, 100);
  onRelayChange();
  onTemperatureChange();
}

void onSensorChange() {
}

void onRelayChange() {
  if (relay) {
    if (val > dry){
       digitalWrite(relayPin, LOW);
      delay(3000);
    }else{
     digitalWrite(relayPin, HIGH);
    }
  }else{digitalWrite(relayPin, HIGH);};
}

void onTemperatureChange() {
  int sensorValue = analogRead(thermistorPin);
  float resistance = R1 * (1023.0 / sensorValue - 1);
  float temperatureK = 1 / (log(resistance / R1) / B + 1 / T0);
  temperature = temperatureK - 273.15;

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" C");
  delay(1000);
}

