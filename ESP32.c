#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// ——— CONFIG ———
#define DHTPIN      2       // Data pin for DHT11 (GPIO2)
#define DHTTYPE     DHT11   // DHT 11 sensor

DHT dht(DHTPIN, DHTTYPE);

// ——— PIN ASSIGNMENTS ———
#define LED_PIN     18      // LED on GPIO18
#define BUZZER_PIN  19      // Buzzer on GPIO19

// ——— OLED SETUP ———
#define SCREEN_WIDTH  128
#define SCREEN_HEIGHT  64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
  // Initialize Serial communication for debugging
  Serial.begin(115200);
  delay(2000);  // allow sensor to stabilize
  
  Serial.println("Adafruit DHT11 test");

  // Initialize the DHT sensor
  dht.begin();
  
  // Initialize I2C (defaults: SDA=21, SCL=22 for ESP32)
  Wire.begin();

  // Initialize OLED display
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("SSD1306 init failed!");
    while (1) delay(100);
  }
  display.clearDisplay();
  display.display();

  // Configure LED and Buzzer pins as outputs and set initial state LOW
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  digitalWrite(BUZZER_PIN, LOW);
}

void loop() {
  // Read humidity and temperature
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  // Check if any reads failed and print an error message.
  bool valid = !isnan(h) && !isnan(t);

  if (valid) {
    // Turn LED ON when both readings are valid
    digitalWrite(LED_PIN, HIGH);
  } else {
    digitalWrite(LED_PIN, LOW);
  }

  // Beep the buzzer for 100ms
  digitalWrite(BUZZER_PIN, HIGH);
  delay(100);
  digitalWrite(BUZZER_PIN, LOW);

  // Display results on the OLED
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);

  if (valid) {
    display.printf("T:%.1fC\nH:%.1f%%", t, h);
    Serial.printf("🌡️  Temp: %.1f°C   💧 Humidity: %.1f%%\n", t, h);
  } else {
    display.println("Sensor Err!");
    Serial.println("❌ Failed to read from DHT sensor!");
  }
  display.display();

  // Wait 2 seconds between readings (DHT11 max sample rate)
  delay(2000);
}
