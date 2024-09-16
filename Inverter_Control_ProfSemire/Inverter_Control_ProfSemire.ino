#define ATS 6
#define Load1 7
#define Load2 8
#define voltageLevel A7
#define InverterSwitch 13
#define Mains A5
float val_M = 0;
float val_B = 0;
//float real_val
// include the library code:
#include <LiquidCrystal.h>

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
const int numReadings = 200;
float readVoltage() {
  float sum = 0.0;
  for (int i = 0; i < numReadings; i++) {
    float val_B = ((analogRead(voltageLevel) * 5.0) / 1024.0) * 11.0;
    sum += val_B;
    delay(10); // Small delay to allow for a more accurate average (adjust as needed)
  }
  return sum / numReadings;
}

void setup() {
  // set up the LCD's number of columns and rows:
  Serial.begin(9600);
  pinMode(ATS, OUTPUT);
  pinMode(Load1, OUTPUT);
  pinMode(Load2, OUTPUT);
  pinMode(InverterSwitch, OUTPUT);
  pinMode (voltageLevel, INPUT);
  pinMode (Mains, INPUT);
  digitalWrite(ATS, LOW);
  digitalWrite(Load1, LOW);
  digitalWrite(Load2, LOW);
  digitalWrite(ATS, LOW);
  digitalWrite(InverterSwitch, LOW);

  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("Welcome!");
  delay(2000);
  lcd.clear();


}

void loop() {
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  val_M = (analogRead(Mains) * 5) / 1024.0;
  float averageVoltage = readVoltage();
  Serial.print("Average Voltage: ");
  Serial.println(averageVoltage);
  
  lcd.setCursor(1, 0);
  lcd.print("BAT: ");
  lcd.setCursor(6, 0);
  lcd.print(averageVoltage);
  Serial.println(val_M);
//  Serial.println (val_B);
  delay(1000); // Wait 1 second before next reading

  if (val_M < 1) {
    digitalWrite(InverterSwitch, HIGH);
    digitalWrite(ATS, HIGH);
    Serial.println("Inverter, ON");
//    digitalWrite(Load1, LOW);
//    digitalWrite(Load2, LOW);
    if (averageVoltage >= 12 ) {
      digitalWrite(Load1, LOW);
      digitalWrite(Load2, LOW);
      Serial.println("both Load, ON");
    }
    if (averageVoltage > 11 && averageVoltage < 12 ) {
      digitalWrite(Load1, HIGH);
      Serial.println("Load1, Off");
    }
    else if (averageVoltage < 11 ) {
      digitalWrite(Load1, HIGH);
      digitalWrite(Load2, HIGH);
      digitalWrite(InverterSwitch, HIGH);
      Serial.println("Load2, Off");
    }
//    else {
//      digitalWrite(Load1, HIGH);
//      digitalWrite(Load2, HIGH);
//      Serial.println("both Load, Off");
//    }
  }
  else {
    digitalWrite(InverterSwitch, LOW);
    digitalWrite(ATS, LOW);
    Serial.println("Inverter, OFF");
    digitalWrite(ATS, LOW);
    digitalWrite(Load1, LOW);
    digitalWrite(Load2, LOW);
  }
}
