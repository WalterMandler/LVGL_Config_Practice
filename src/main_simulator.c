#include "basic_button.h"
#include "lvgl.h"
#include <SDL.h>
#include <Windows.h>
#include <stdio.h>

static void configure_simulator() {
	// Use SDL to create a display for lvgl.
	lv_display_t* display1 = lv_sdl_window_create(480, 320);
	lv_display_set_default(display1);

	// Move the position of the SDL window.
	SDL_Window* sdl_window1 = lv_sdl_window_get_window(display1);
	uint8_t N_computer_monitors = SDL_GetNumVideoDisplays();
	if (N_computer_monitors == 1) {
		SDL_SetWindowPosition(sdl_window1, 1000, 400);
	} else {
		SDL_DisplayMode computer_monitor_info;
		SDL_GetDesktopDisplayMode(0, &computer_monitor_info);
		SDL_SetWindowPosition(sdl_window1, computer_monitor_info.w + 250, 400);
	}

	// Create the mouse input device.
	lv_indev_t* mouse = lv_sdl_mouse_create();
	lv_indev_set_display(mouse, display1);

	// Create a default group for widgets.
	lv_group_t* default_widget_group = lv_group_create();
	lv_group_set_default(default_widget_group);
}

int main(int argc, char** argv) {
	// Unused command line arguments (required by SDL).
	(void)argc;
	(void)argv;

	// Initialize LVGL.
	lv_init();

	// Configure the simulator.
	configure_simulator();

	// Create the GUI.
	basic_button();

	// Periodically call the lvgl timer handler.
	while (1) {
		lv_timer_handler();
		Sleep(10);
	}

	return 0;
}
