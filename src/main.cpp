#include <Arduino.h>

#define FLOW_PIN 5  // chân tín hiệu cảm biến

void flowTask(void *pvParameters) {
  bool lastState = LOW;
  uint32_t pulseCount = 0;
  uint32_t lastPulseCount = 0;
  unsigned long lastTime = 0;
  const float pulsesPerLiter = 450.0;  // 450 xung = 1 lít (YF-S201)

  while (true) {
    // Đọc tín hiệu
    bool currentState = digitalRead(FLOW_PIN);
    if (lastState == HIGH && currentState == LOW) {
      pulseCount++;
    }
    lastState = currentState;

    // Cứ mỗi 1 giây tính tần số và lưu lượng
    if (millis() - lastTime >= 1000) {
      uint32_t deltaPulses = pulseCount - lastPulseCount;
      float frequency = deltaPulses; // vì đo trong 1s => f = pulses/1s
      float flowRate = (frequency / pulsesPerLiter) * 60.0; // L/min

      Serial.print("Tan so xung: ");
      Serial.print(frequency);
      Serial.print(" Hz | Luu luong: ");
      Serial.print(flowRate);
      Serial.println(" L/phut");

      lastPulseCount = pulseCount;
      lastTime = millis();
    }

    vTaskDelay(pdMS_TO_TICKS(1)); // delay nhẹ để CPU không bị full load
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(FLOW_PIN, INPUT_PULLUP);

  // Tạo task đọc lưu lượng
  xTaskCreate(flowTask, "FlowTask", 4096, NULL, 1, NULL);
  Serial.println("RTOS Flow Sensor started (No Interrupt)!");
}

void loop() {
  vTaskDelay(portMAX_DELAY); // Không dùng loop chính
}
