#ifndef BOARD_HAS_PSRAM
#error "Please enable PSRAM !!!"
#endif

#include <Arduino.h>
#include <esp_task_wdt.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "epd_driver.h"
#include "firasans.h"
#include "esp_adc_cal.h"
#include <Wire.h>
#include "logo.h"

#define BATT_PIN            36
#define S2    34
#define S3    35
#define S4    39
#define S6    0


uint8_t *framebuffer;
int vref = 1100;

void setup()
{
  Serial.begin(115200);

  // Correct the ADC reference voltage
  esp_adc_cal_characteristics_t adc_chars;
  esp_adc_cal_value_t val_type = esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, 1100, &adc_chars);
  if (val_type == ESP_ADC_CAL_VAL_EFUSE_VREF) {
    Serial.printf("eFuse Vref:%u mV", adc_chars.vref);
    vref = adc_chars.vref;
  }

  epd_init();

  framebuffer = (uint8_t *)ps_calloc(sizeof(uint8_t), EPD_WIDTH * EPD_HEIGHT / 2);
  if (!framebuffer) {
    Serial.println("alloc memory failed !!!");
    while (1);
  }
  memset(framebuffer, 0xFF, EPD_WIDTH * EPD_HEIGHT / 2);


  Rect_t area = {
    .x = 0,
    .y = 0,
    .width = logo_width,
    .height = logo_height,
  };

  epd_poweron();
  epd_clear();
  epd_draw_grayscale_image(area, (uint8_t *)logo_data);
  epd_poweroff();


  int cursor_x = 200;
  int cursor_y = 300;

  char *string1 = "➸ 16 color grayscale  😀 \n";
  char *string2 = "➸ Use with 4.7\" EPDs 😍 \n";
  char *string3 = "➸ High-quality font rendering ✎🙋";
  char *string4 = "➸ ~630ms for full frame draw 🚀\n";

  epd_poweron();
  writeln((GFXfont *)&FiraSans, string1, &cursor_x, &cursor_y, NULL);
  delay(500);
  cursor_x = 200;
  cursor_y += 50;
  writeln((GFXfont *)&FiraSans, string2, &cursor_x, &cursor_y, NULL);
  delay(500);
  cursor_x = 200;
  cursor_y += 50;
  writeln((GFXfont *)&FiraSans, string3, &cursor_x, &cursor_y, NULL);
  delay(500);
  cursor_x = 200;
  cursor_y += 50;
  writeln((GFXfont *)&FiraSans, string4, &cursor_x, &cursor_y, NULL);
  delay(500);
  epd_poweroff();

  esp_deep_sleep_start();

}

void loop()
{
  
}
