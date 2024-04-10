#include "supun-final.hpp"

// Networking
WiFiConnectionHandler ArduinoIoTPreferredConnection(WIFI_SSID, WIFI_PASS);

// Variables
bool alarm_on = true;

// Sensor globals
float sensor = 0;
bool reset_btn = false;

void onResetBtnChange()
{
  if (!reset_btn)
    return ;
  Serial.println("___ RESET ___");
  alarm_on = false;
}

void pinModes()
{
  pinMode(BUZZER_PIN, OUTPUT);
}

void initProperties()
{
  ArduinoCloud.setBoardId(DEVICE_LOGIN_NAME);
  ArduinoCloud.setSecretDeviceKey(DEVICE_SECRET);
  // ArduinoCloud.addProperty(sensor, READ, 5 * SECONDS, NULL);
  ArduinoCloud.addProperty(sensor, READ, ON_CHANGE, NULL);
  ArduinoCloud.addProperty(reset_btn, READWRITE, ON_CHANGE, onResetBtnChange);
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("___ SETUP ___");
  pinModes();
  initProperties();
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);
  Serial.println("___ WAITING CONNECTION ___");
  while (ArduinoCloud.connected())
    delay(1000);
  Serial.println("___ CONNECTED ___");
  #if LOG_LEVEL > 0
    setDebugMessageLevel(LOG_LEVEL);
    ArduinoCloud.printDebugInfo();
  #endif
}

// Operates in a deffrent loop until it returns false.
bool alarm_cb()
{
  if (!alarm_on)
    return false;
  ArduinoCloud.update();
  return true;
}

void loop() {
  ArduinoCloud.update();
  sensor++;
  if (sensor == 10)
    sensor = 0;
  alarm(alarm_cb);
}
