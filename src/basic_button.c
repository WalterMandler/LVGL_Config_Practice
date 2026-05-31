#include <lvgl.h>

static void basic_button_cb(lv_event_t* event) {
	lv_obj_t* button = lv_event_get_target(event);
	lv_obj_t* label = lv_obj_get_child(button, 0); // Get the first child of the button which is the label.
	static uint8_t click_count = 0;
	click_count++;
	lv_label_set_text_fmt(label, "My Button: %d", click_count);
}

void basic_button() {
	lv_obj_t* button = lv_button_create(lv_screen_active());
	lv_obj_remove_style_all(button);
	lv_obj_set_style_bg_color(button, lv_color_hex(0xFF0000), LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(button, LV_OPA_100, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_pos(button, 100, 100);
	lv_obj_set_size(button, 150, 50);
	lv_obj_add_event_cb(button, basic_button_cb, LV_EVENT_CLICKED, NULL);

	lv_obj_t* label = lv_label_create(button);
	lv_label_set_text(label, "My Button: 0");
	lv_obj_center(label);
}
