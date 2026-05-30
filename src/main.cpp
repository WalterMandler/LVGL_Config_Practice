#include "basic_button.h"
#include "configure_lvgl.h"
#include <Arduino.h>
#include <lvgl.h>

void setup() {
	// Initialize serial communication.
	uint32_t serial_baud_rate = 115200;
	Serial.begin(serial_baud_rate);
	delay(2000);
	Serial.printf("Serial Communication Initialized at Baud Rate of %d\n", serial_baud_rate);

	// Initialize the display and the touchscreen, then configure them for lvgl.
	configure_lvgl();

	// Add the GUI here.
	basic_button();
}

void loop() {
	lv_timer_handler();
	delay(10);
}
