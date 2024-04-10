#include "supun-final.hpp"

// Networking
WiFiConnectionHandler ArduinoIoTPreferredConnection(WIFI_SSID, WIFI_PASS);

// Variables
bool  alarm_on = false;

void pinModes()
{
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(SENSOR_IR_PIN, INPUT);
}

void initProperties()
{
  ArduinoCloud.setBoardId(DEVICE_LOGIN_NAME);
  ArduinoCloud.setSecretDeviceKey(DEVICE_SECRET);
  ArduinoCloud.addProperty(sensor_gas, READ, 2 * SECONDS, NULL);
  ArduinoCloud.addProperty(sensor_gas_threshold, READWRITE, ON_CHANGE, ir_thresh_cb);
  ArduinoCloud.addProperty(reset_btn, READWRITE, ON_CHANGE, reset_btn_cb);
  ArduinoCloud.addProperty(sensor_ir, READ, ON_CHANGE, NULL);
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("___ SETUP ___");
  pinModes();
  initProperties();
  Serial.println("___ WAITING CONNECTION ___");
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);
  while (ArduinoIoTPreferredConnection.check() != NetworkConnectionState::CONNECTED)
    delay(3000);
  Serial.println("___ CONNECTED ___");
  tone(BUZZER_PIN, NOTE_A4, 200);
  delay(100);
  sensor_ir = true;
  #if LOG_LEVEL > 0
    setDebugMessageLevel(LOG_LEVEL);
    ArduinoCloud.printDebugInfo();
  #endif
}

void loop() {
  ArduinoCloud.update();
  while (ArduinoIoTPreferredConnection.check() != NetworkConnectionState::CONNECTED)
  {
    Serial.println("__ DISCONNECTED __");
    // Beep. Beep.. ... Beep. Beep.. :(
    tone(BUZZER_PIN, NOTE_B3, 100);
    delay(300);
    tone(BUZZER_PIN, NOTE_B3, 200);
    delay(1400);
    return ;
  }
  sensor_gas = analogRead(SENSOR_MQ_PIN);
  sensor_ir = !digitalRead(SENSOR_IR_PIN);
  Serial.print("Gas_Sensor:");
  Serial.print(sensor_gas);
  Serial.print(",");
  Serial.print("Gas_Threshold:");
  Serial.print(sensor_gas_threshold);
  Serial.print(",");
  Serial.print("IR_Sensor:");
  Serial.println(sensor_ir);
  if (sensor_gas > sensor_gas_threshold && !sensor_ir)
    alarm_on = true;
  alarm(alarm_cb);
}
