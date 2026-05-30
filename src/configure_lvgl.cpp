#include <Adafruit_FT6206.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7796S.h>
#include <Arduino.h>
#include <lvgl.h>

// Create the display object for the LCD.
// Hosyond MSP4031, 4.0in, 320x480, RGB565, ST7796S display driver (SPI), FT6336U touchscreen driver (I2C)
enum lcd_enums {
	LCD_CS = D7,
	LCD_RST = D6,
	LCD_RS = D8,
	LCD_WIDTH = 320,  // this is the native width of the display (no rotation)
	LCD_HEIGHT = 480, // this is the native height of the display (no rotation)
	LCD_NPIXELS = LCD_WIDTH * LCD_HEIGHT,
	LCD_NBYTES = LCD_NPIXELS * 2,
};
static Adafruit_ST7796S adafruit_display(LCD_CS, LCD_RS, LCD_RST);

// Create image buffer for the display.
enum display_buffer_enums {
	DISPLAY_BUFFER_NPIXELS = LCD_WIDTH * LCD_HEIGHT / 8,
	DISPLAY_BUFFER_NBYTES = DISPLAY_BUFFER_NPIXELS * 2
};
static uint16_t display_buffer[DISPLAY_BUFFER_NPIXELS] = {0};

// Create the touchscreen object.
static Adafruit_FT6206 adafruit_touchscreen = Adafruit_FT6206();

// This is the function that LVGL can call to get the system time in milliseconds.
// For arduino, the normal millis() function needs a wrapper because it returns an unsigned long while lvgl expects uint32_t.
static uint32_t arduino_get_millis() {
	return millis();
}

// This is the function that LVGL uses to flush the display buffer to the actual LCD.
static void adafruit_flush_cb(lv_display_t* display, const lv_area_t* area, uint8_t* raw_display_buffer_pointer) {
	int16_t x = area->x1;
	int16_t y = area->y1;
	int16_t w = lv_area_get_width(area);
	int16_t h = lv_area_get_height(area);
	uint16_t* display_buffer_pointer = (uint16_t*)raw_display_buffer_pointer; // convert the pointer to 16-bit to match the RGB565 format
	adafruit_display.drawRGBBitmap(x, y, display_buffer_pointer, w, h);
	lv_display_flush_ready(display); // Inform LVGL that flushing is complete so the display buffer can be modified again.
}

// This is the function that LVGL uses to get coordinates from the touchscreen.
static void adafruit_touchscreen_cb(lv_indev_t* lv_touchscreen, lv_indev_data_t* lv_touchscreen_data) {
	if (adafruit_touchscreen.touched()) {
		TS_Point touch_coordinates = adafruit_touchscreen.getPoint();
		lv_touchscreen_data->point.x = touch_coordinates.y;									 // flip the x and y axes.
		lv_touchscreen_data->point.y = map(touch_coordinates.x, 0, LCD_WIDTH, LCD_WIDTH, 0); // use the arduino map function to effectively flip the direction of the axis.
		lv_touchscreen_data->state = LV_INDEV_STATE_PRESSED;
	} else {
		lv_touchscreen_data->state = LV_INDEV_STATE_RELEASED;
	}
}

void configure_lvgl() {
	// Initialize the displsay.
	uint32_t spi_clock_frequency = 40000000;
	adafruit_display.init(LCD_WIDTH, LCD_HEIGHT, 0, 0, ST7796S_BGR);
	adafruit_display.setSPISpeed(spi_clock_frequency);
	adafruit_display.setRotation(1);
	adafruit_display.invertDisplay(true);
	Serial.printf("Display Initialized at SPI Clock Frequency of %d Hz\n", spi_clock_frequency);

	// Initialize the touchscreen.
	adafruit_touchscreen.begin();
	Serial.println("Touchscreen Initialized");

	// Initialize lvgl and assign a timing callback.
	lv_init();
	lv_tick_set_cb(arduino_get_millis);

	// Create a display for lvgl, set the color format, assign a buffer, and assign a flush callback.
	// Remember we rotated the screen. The native width is now the height, and the native height is now the width.
	lv_display_t* lv_display = lv_display_create(LCD_HEIGHT, LCD_WIDTH);
	lv_display_set_color_format(lv_display, LV_COLOR_FORMAT_RGB565);
	lv_display_set_buffers(lv_display, display_buffer, NULL, DISPLAY_BUFFER_NBYTES, LV_DISPLAY_RENDER_MODE_PARTIAL);
	lv_display_set_flush_cb(lv_display, adafruit_flush_cb);

	// Add the touchscreen as an input device to lvgl.
	lv_indev_t* lv_touchscreen = lv_indev_create();
	lv_indev_set_type(lv_touchscreen, LV_INDEV_TYPE_POINTER);
	lv_indev_set_read_cb(lv_touchscreen, adafruit_touchscreen_cb);
	
	Serial.println("LVGL Initialized and Configured");
}
