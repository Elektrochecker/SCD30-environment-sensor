#include "scene.h"

SCENE_TYPE SCENE_current_scene = overview;

void SCENE_display_current_scene() {

  DISPLAY_clear_frame_buffer();

  DATETIME time = CLOCK_read_time();
  char str_buffer[15] = {0};

  CLOCK_date_tostring(time, str_buffer, 15);
  DISPLAY_write_to_framebuffer(str_buffer, 0, 0);
  CLOCK_tostring(time, str_buffer, 9);
  DISPLAY_write_to_framebuffer(str_buffer, 0, 88);

  switch (SCENE_current_scene) {
  default:
  case overview:
    dtostrf(SENSOR_last_reading.temperature, 4, 1, str_buffer);
    DISPLAY_write_to_framebuffer("temperature:", 3, 0);
    DISPLAY_write_to_framebuffer(str_buffer, 3, 64);
    DISPLAY_write_to_framebuffer(" C", 3, 88);

    dtostrf(SENSOR_last_reading.humidity, 4, 1, str_buffer);
    DISPLAY_write_to_framebuffer("humidity:", 5, 0);
    DISPLAY_write_to_framebuffer(str_buffer, 5, 64);
    DISPLAY_write_to_framebuffer(" %", 5, 88);

    dtostrf(SENSOR_last_reading.co2concentration, 4, 0, str_buffer);
    DISPLAY_write_to_framebuffer("co2:", 7, 0);
    DISPLAY_write_to_framebuffer(str_buffer, 7, 64);
    DISPLAY_write_to_framebuffer(" ppm", 7, 88);
    break;
  case co2graph:
    dtostrf(SENSOR_last_reading.co2concentration, 4, 0, str_buffer);
    DISPLAY_write_to_framebuffer("co2:", 2, 40);
    DISPLAY_write_to_framebuffer(str_buffer, 2, 64);
    DISPLAY_write_to_framebuffer("ppm", 2, 90);

    DISPLAY_write_to_framebuffer("200", 4, 4);
    DISPLAY_write_to_framebuffer("2000", 4, 100);

    DISPLAY_rect_hollow(4, 40, 120, 24);
    float co2 = SENSOR_last_reading.co2concentration;
    if (co2 < 200)
      co2 = 200;
    else if (co2 > 2000)
      co2 = 2000;

    co2 -= 200;

    DISPLAY_rect_dashed_0(4, 40, co2 / (2000 - 200) * 120, 24);
    break;
  }

  DISPLAY_show_frame();
}

void SCNENE_advance() {
  static uint8_t counter = 0;
  counter++;

  if (counter >= SCENE_LIFETIME) {
    SCENE_current_scene++;
    if (SCENE_current_scene == end) {
      SCENE_current_scene = 0;
    }
    counter = 0;
  }
}
