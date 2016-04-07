#include <pebble.h>
static Window *s_main_window;
static TextLayer *s_time_layer;
static TextLayer *s_date_layer;
static GFont s_time_font;
static GFont s_time_font_date;
static BitmapLayer *s_background_layer;
static GBitmap *s_background_bitmap;
static void update_time() {
  // Get a tm structure
  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);


  // Create a long-lived buffer
static char buffer[] = "00:00:00";
static char bufferd[] = "AAA BBB-00";
  // Write the current hours and minutes into the buffer
  if(clock_is_24h_style() == true) {
    // Use 24 hour format
    strftime(buffer, sizeof("00:00:00"), "%H:%M:%S", tick_time);
  } else {
    // Use 12 hour format
    strftime(buffer, sizeof("00:00:00"), "%I:%M:%S", tick_time);
  }
  strftime(bufferd,sizeof("AAA BBB 00"),"%a %b-%d",tick_time);
  // Display this time on the TextLayer
  text_layer_set_text(s_time_layer, buffer);
  text_layer_set_text(s_date_layer, bufferd);
}
static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}
static void main_window_unload(Window *window) {
    // Destroy TextLayer
  text_layer_destroy(s_time_layer);
  text_layer_destroy(s_date_layer);
    // Destroy GBitmap
gbitmap_destroy(s_background_bitmap);

// Destroy BitmapLayer
bitmap_layer_destroy(s_background_layer);
}
static void main_window_load(Window *window) {
  // Create GBitmap, then set to created BitmapLayer
s_background_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_WIN_1);
s_background_layer = bitmap_layer_create(GRect(0, 0, 144, 168));
bitmap_layer_set_bitmap(s_background_layer, s_background_bitmap);
layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(s_background_layer));
  
  // Create time TextLayer
  s_time_layer = text_layer_create(GRect(0, 45, 144, 50));
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_text_color(s_time_layer, GColorBlack);
  text_layer_set_text(s_time_layer, "00:00:00");
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);
  
    // Create date TextLayer
	s_date_layer = text_layer_create(GRect(0, 70,144, 50));
  text_layer_set_background_color(s_date_layer, GColorClear);
  text_layer_set_text_color(s_date_layer, GColorBlack);
  text_layer_set_text(s_date_layer, "___ ___ __");
  text_layer_set_text_alignment(s_date_layer, GTextAlignmentCenter);
  
// Create GFont
s_time_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_PERFECT_DOS_27));
s_time_font_date = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_PERFECT_DOS_18));

// Apply to TextLayer
text_layer_set_font(s_time_layer, s_time_font);
text_layer_set_font(s_date_layer, s_time_font_date);
  // Add it as a child layer to the Window's root layer
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_date_layer));
}
static void init(){
  // Create main Window element and assign to pointer
  s_main_window = window_create();

  // Set handlers to manage the elements inside the Window
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });

  // Show the Window on the watch, with animated=true
  window_stack_push(s_main_window, true);
  // Register with TickTimerService
tick_timer_service_subscribe(SECOND_UNIT, tick_handler);
  update_time();
}
static void deinit()
{
  window_destroy(s_main_window);
}
int main(void){
  init();
  app_event_loop();
  deinit();
}

//foobar