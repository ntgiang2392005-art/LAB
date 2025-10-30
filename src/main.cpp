#include <Arduino.h>
#define FLOW_PIN 23
volatile uint32_t pulseCount = 0;
void IRAM_ATTR flowISR(){
  pulseCount++;
}
void flowTask(void* pvParameters) {
  const float fmin=7.5;
  
  while (true){
    vTaskDelay(pdMS_TO_TICKS(1000));
    uint32_t count =pulseCount ;
    pulseCount =0 ;
    float flowrate=(count/fmin);
Serial.print("Tan so xung : ");
Serial.print(count);
Serial.print("\n Hz Luu luong : ");
Serial.print (flowrate);
Serial.print (" L/min");
  }
}
void setup (){
  Serial.begin(115200);
  pinMode(FLOW_PIN,INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(FLOW_PIN),flowISR,RISING);
  xTaskCreate(flowTask,"FlowTask",2048,NULL,1,NULL);
  Serial.println("RTOS Flow Sensor started!");
}
void loop (){
  vTaskDelay(portMAX_DELAY);
}