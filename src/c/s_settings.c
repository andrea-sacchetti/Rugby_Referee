#include "s_settings.h"
#include <pebble.h>

// Deafult apps settings
long match_duration = 40 * 60;
long card_duration = 10 * 60;
long injury_duration = 15 * 60;
/*int max_match_duration = 60 * 60;
int min_match_duration = 1 * 60;
int max_card_duration = 20 * 60;
int min_card_duration = 1 * 60;
int max_injury_duration = 30 * 60;
int min_injury_duration = 1 * 60;*/
// Initialize events and GUI
static int time_unit = 60;
int number_events = 0;
int events[MAX_EVENTS][3];
static char event_types[7][11] = {"End Period ", "Try", "Conv", "Pen", "Drop", "Card", "Inj"};
char team_identifier[] = "AB";
static int last_shown_event = 0;
//int settings_window_selection = 1;
static int send_message;
static long last_button_press = 0;
static long time_out_GUI = 10;
long next_expiring_timer;
// Notes buffer
int number_notes;
int notes_size = 0;
char notes[MAX_NOTES_LENGTH];
// Subs
int subs_a = 0;
int subs_b = 0;
Window *m_window;
int current_period = 0;
static int i_batch;

// BEGIN AUTO-GENERATED UI CODE; DO NOT MODIFY - SLIGHTLY MODIFIED!
static Window *s_window;
static GFont s_res_gothic_14;
static GFont s_res_gothic_18_bold;
static TextLayer *s_match_history;
static TextLayer *s_history_title;
static TextLayer *s_subs;
static TextLayer *s_subs_a;
static TextLayer *s_subs_b;
static BitmapLayer *s_line_1;


static void initialise_ui(void) {
  //s_window = window_create();

  #ifdef PBL_SDK_2
  window_set_fullscreen(s_window, true);
  #endif
    
  s_res_gothic_14 = fonts_get_system_font(FONT_KEY_GOTHIC_14);
  s_res_gothic_18_bold = fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD);

  // s_match_history
  s_match_history = text_layer_create(GRect(0, 45, 144, 122));
  text_layer_set_text(s_match_history, " ");
  text_layer_set_font(s_match_history, s_res_gothic_14);
  text_layer_set_text_color(s_match_history, GColorBlack);
  text_layer_set_background_color(s_match_history, GColorClear);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_match_history);
  
  // s_history_title
  s_history_title = text_layer_create(GRect(0, 25, 144, 20));
  text_layer_set_text(s_history_title, "Match History");
  text_layer_set_text_alignment(s_history_title, GTextAlignmentCenter);
  text_layer_set_font(s_history_title, s_res_gothic_18_bold);
  text_layer_set_text_color(s_history_title, GColorBlack);
  text_layer_set_background_color(s_history_title, GColorClear);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_history_title);
  
  // s_subs_a
  s_subs_a = text_layer_create(GRect(0, 1, 20, 18));
  text_layer_set_text(s_subs_a, "0");
  text_layer_set_text_alignment(s_subs_a, GTextAlignmentCenter);
  text_layer_set_font(s_subs_a, s_res_gothic_14);
  text_layer_set_text_color(s_subs_a, GColorWhite);
  text_layer_set_background_color(s_subs_a, GColorBlack);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_subs_a);
  
  // s_subs_b
  s_subs_b = text_layer_create(GRect(126, 1, 20, 18));
  text_layer_set_text(s_subs_b, "0");
  text_layer_set_text_alignment(s_subs_b, GTextAlignmentCenter);
  text_layer_set_font(s_subs_b, s_res_gothic_14);
  text_layer_set_text_color(s_subs_b, GColorWhite);
  text_layer_set_background_color(s_subs_b, GColorBlack);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_subs_b);
  
  // s_set_injuries
  s_subs = text_layer_create(GRect(0, 0, 144, 24));
  text_layer_set_text(s_subs, "Subs");
  text_layer_set_text_alignment(s_subs, GTextAlignmentCenter);
  text_layer_set_font(s_subs, s_res_gothic_18_bold);
  text_layer_set_text_color(s_subs, GColorBlack);
  text_layer_set_background_color(s_subs, GColorClear);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_subs);

  // s_line_1
  s_line_1 = bitmap_layer_create(GRect(0, 21, 144, 2));
  bitmap_layer_set_background_color(s_line_1, GColorBlack);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_line_1);
}

static void destroy_ui(void) {
  //window_destroy(s_window);
  text_layer_destroy(s_match_history);
  text_layer_destroy(s_history_title);
  text_layer_destroy(s_subs_a);
  text_layer_destroy(s_subs_b);
  text_layer_destroy(s_subs);
  bitmap_layer_destroy(s_line_1);
}
// END AUTO-GENERATED UI CODE

// Kill UI on exit
static void handle_window_unload(Window* window) {
  /*persist_write_int(NUM_MATCHLENGTH_PKEY, match_duration);
  persist_write_int(NUM_CARDLENGTH_PKEY, card_duration);
  persist_write_int(NUM_INJURYLENGTH_PKEY, injury_duration);*/
  tick_timer_service_unsubscribe();
  window_destroy(s_window);
  destroy_ui();
  window_stack_push(m_window, true);
}

// Send a key-int pair to the phone - DEPRECATED
/*DictionaryIterator *iter;
time_t send_time_s = 0;
uint16_t send_time_ms = 0;
time_t last_send_time_s = 0;
uint16_t last_send_time_ms = 0;
int max_dt = 200;*/
// Send a key-str pair to the phone
/*static DictionaryIterator *iter;
static time_t send_time_s = 0;
static uint16_t send_time_ms = 0;
static time_t last_send_time_s = 0;
static uint16_t last_send_time_ms = 0;
static int max_dt = 200;

void send_str(int key, const char* value) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "trying to send %d, %s", key, value);
  DictionaryIterator *iter;
  app_message_outbox_begin(&iter);
  AppMessageResult r;
  dict_write_cstring(iter, key, value);
  r = app_message_outbox_send();
  while(r != APP_MSG_OK){
    dict_write_cstring(iter, key, value);
    r = app_message_outbox_send();
  }
  APP_LOG(APP_LOG_LEVEL_DEBUG, "result: %d", r);
  iter = NULL;
}*/

// Send a key-int pair to the phone - DEPRECATED
/*DictionaryIterator *iter;
time_t send_time_s = 0;
uint16_t send_time_ms = 0;
time_t last_send_time_s = 0;
uint16_t last_send_time_ms = 0;
int max_dt = 200;*/

/*void send_int(int key, int value) {
  app_message_outbox_begin(&iter);
  dict_write_int(iter, key, &value, sizeof(int), true);
  time_ms(&send_time_s, &send_time_ms);
  int dt = (send_time_s - last_send_time_s) * 1000 + (send_time_ms - last_send_time_ms);
  last_send_time_s = send_time_s;
  last_send_time_ms = send_time_ms;
  if(dt > max_dt){
    app_message_outbox_send();
    iter = NULL;
  }
}*/

// Send a key-int pair to the phone - DEPRECATED
/*void send_int(int key, int value) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "trying to send %d, %d", key, value);
  DictionaryIterator *iter;
  app_message_outbox_begin(&iter);
  dict_write_int(iter, key, &value, sizeof(int), true);
  AppMessageResult r;
  time_ms(&send_time_s, &send_time_ms);
  int dt = (send_time_s - last_send_time_s) * 1000 + (send_time_ms - last_send_time_ms);
  last_send_time_s = send_time_s;
  last_send_time_ms = send_time_ms;
  if(dt > max_dt){
    r = app_message_outbox_send();
    iter = NULL;
    APP_LOG(APP_LOG_LEVEL_DEBUG, "result: %d", r);
  }
//  r = app_message_outbox_send();
//  APP_LOG(APP_LOG_LEVEL_DEBUG, "result: %d", r);
}*/

/*void send_block(int key[10], int value[10], int n_entries) {
  DictionaryIterator *iter;
  app_message_outbox_begin(&iter);
  int i;
  for(i = 0; i < n_entries; i++){
    int value_i = value[i];
    dict_write_int(iter, key[i], &value_i, sizeof(int), true);
  }
  app_message_outbox_send();
}*/

// Update GUI upon changes
void update_GUI(){
  // History events
  if(number_events > 0){
    // Add first event to be shown if any
    int lower_limit;
    lower_limit = last_shown_event - 6;
    if(lower_limit < 0) lower_limit = 0;
    static char setbuffer_history[148];
    int seconds;
    seconds = events[lower_limit][0] % 5000;
    if (events[lower_limit][1] < 8){
      if(events[lower_limit][2] == 0){
        snprintf(setbuffer_history, 18, "%02d:%02d %s", seconds / 60, seconds % 60, event_types[events[lower_limit][1]]);
      } else {
        snprintf(setbuffer_history, 18, "%02d:%02d %s Team %c", seconds / 60, seconds % 60, event_types[events[lower_limit][1]], team_identifier[events[lower_limit][2] - 1]);
      }
    } else {
      snprintf(setbuffer_history, 19, "%02d:%02d %.*s", seconds / 60, seconds % 60, events[lower_limit][2], notes + events[lower_limit][1] - 8);      
    }
    // Add up to 6 more events if existing
    if(last_shown_event > lower_limit){
      for(int i = lower_limit + 1; i < last_shown_event + 1; i++){
        static char setbuffer_history_temp[20];
        seconds = events[i][0] % 5000;
        if (events[i][1] < 8){
          if(events[i][2] == 0){
            snprintf(setbuffer_history_temp, 19, "\n%02d:%02d %s", seconds / 60, seconds % 60, event_types[events[i][1]]);
          } else {
            snprintf(setbuffer_history_temp, 19, "\n%02d:%02d %s Team %c", seconds / 60, seconds % 60, event_types[events[i][1]], team_identifier[events[i][2] - 1]);
          }
        }
        else {
          //strncpy(extracted_note, buff+10, 4);
          snprintf(setbuffer_history_temp, 20, "\n%02d:%02d %.*s", seconds / 60, seconds % 60, events[i][2], notes + events[i][1] - 8);
        }
        strcat(setbuffer_history, setbuffer_history_temp);
      }
    }
    text_layer_set_text(s_match_history, setbuffer_history);
  } else {
    // No event whatsoever
    text_layer_set_text(s_match_history, "");
  }
  // Subs
  static char a_subs[] = "00";
  snprintf(a_subs, sizeof("00"), "%d", subs_a);
  text_layer_set_text(s_subs_a, a_subs);
  static char b_subs[] = "00";
  snprintf(b_subs, sizeof("00"), "%d", subs_b);
  text_layer_set_text(s_subs_b, b_subs);
}

// Handle single click up
static void up_single_click_handler_do() {
  if(last_shown_event > 6) last_shown_event--;
  send_message = 0;
  update_GUI();
}
static void up_single_click_handler(ClickRecognizerRef recognizer, void *context) {
  last_button_press = time(NULL);
  up_single_click_handler_do();
  //send_int(settings_window_selection * 10 + 200, send_message);
}

// Handle single click down
static void down_single_click_handler_do() {
  if(last_shown_event < number_events - 1) last_shown_event++;
  send_message = 0;
  update_GUI();
}
static void down_single_click_handler(ClickRecognizerRef recognizer, void *context) {
  last_button_press = time(NULL);
  down_single_click_handler_do();
  //send_int(settings_window_selection * 10 + 201, send_message);
}

void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Outbox send failed!");
}

static void send_batch(int batch_size){
  DictionaryIterator *iter;
  app_message_outbox_begin(&iter);
  // end loop at the right point
  int end_loop;
  if(i_batch + batch_size >= number_events){
    end_loop = number_events;
  } else {
    end_loop = i_batch + batch_size;
  }
  for(int i = i_batch; i < end_loop; i++){
    static char setbuffer_history[13];
    // Standard events
    if (events[i][1] < 8){
      //snprintf(setbuffer_history, 18, "%02d:%02d %s %s", (events[i][0] % 5000) / 60, events[i][0] % 60, event_types[events[i][1]], affected[events[i][2]]);
      //snprintf(setbuffer_history, 12, "%s %s", event_types[events[i][1]], affected[events[i][2]]);
      snprintf(setbuffer_history, 4, "#%d%d", events[i][1], events[i][2]);
    }
    // Notes events
    else {
      //snprintf(setbuffer_history, 18, "%02d:%02d %.*s", (events[i][0] % 5000) / 60, events[i][0] % 60, events[i][2], notes + events[i][1] - 8);
      snprintf(setbuffer_history, 13, "%.*s", events[i][2], notes + events[i][1] - 8);
    }
    dict_write_cstring(iter, i + events[i][0], setbuffer_history);
    //dict_write_cstring(iter, i + (events[i][0] / 5000 + 1) * 1000, setbuffer_history);
  }
  // Close and send outbox
  if(i_batch + batch_size >= number_events){
    dict_write_cstring(iter, 65535, "end");
  }
  //AppMessageResult r = 0;
  dict_write_end(iter);
  //r = app_message_outbox_send();
  app_message_outbox_send();
  //APP_LOG(APP_LOG_LEVEL_INFO, "Outbox batch send success!");  
}

void outbox_sent_callback(DictionaryIterator *iter, void *context) {
  // Prepare outbox
  // Loop through events in 25-sized batches
  /*int i_try = 1;
  while(!(s_send_success)) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Send attempt: %d", i_try);
    psleep(100);
    i_try++;
  }*/
  i_batch += 5;
  if (i_batch < number_events){
    send_batch(5);
  } else {
    APP_LOG(APP_LOG_LEVEL_INFO, "Outbox send success!");
  }
}

// Handle long click select
static void select_long_click_handler_do() {
  i_batch = 0;
  send_batch(5);
}
static void select_long_click_handler(ClickRecognizerRef recognizer, void *context) {
  last_button_press = time(NULL);
  vibes_long_pulse();
  select_long_click_handler_do();
}

// Function to register button listeners
static void config_provider(Window *window) {
  // single click / repeat-on-hold config:
  window_single_click_subscribe(BUTTON_ID_DOWN, down_single_click_handler);
  window_single_click_subscribe(BUTTON_ID_UP, up_single_click_handler);
  //window_single_click_subscribe(BUTTON_ID_SELECT, select_single_click_handler);

  // long click config:
  //window_long_click_subscribe(BUTTON_ID_DOWN, 1000, down_long_click_handler,NULL);
  //window_long_click_subscribe(BUTTON_ID_UP, 1000, up_long_click_handler,NULL);
  window_long_click_subscribe(BUTTON_ID_SELECT, 1000, select_long_click_handler,NULL);
}

// Key-press emulation from companion app - DEPRECATED
/*void emulate_key_press_settings(int command_key, int command_value){
  // Settings window - Mode selection
  static int settings_window_selection_old;
  settings_window_selection_old = settings_window_selection;
  if(command_key >= 20){
    settings_window_selection = command_key - 19;
    // Single up
    if(command_value == 0){
      up_single_click_handler_do();
    }
    // Single down
    if(command_value == 1){
      down_single_click_handler_do();
    }
    // Single select
    if(command_value == 2){
      select_single_click_handler_do();
    }
    // Long select
    if(command_value == 5){
      sync_durations(match_duration, card_duration, injury_duration);
    }
  }
  // Handle direct call to set a specific value for the durations
  if(command_key > 100 && command_key < 104){
    settings_window_selection = command_key - 100;
    static long time_to_set;
    time_to_set = (long) (command_value * time_unit);
    switch(command_key){
      // Match duration
      case 101:
      match_duration = time_to_set;
      if(match_duration > max_match_duration) match_duration = max_match_duration;
      if(match_duration < min_match_duration) match_duration = min_match_duration;
      break;
      // Card duration
      case 102:
      card_duration = time_to_set;
      if(card_duration > max_card_duration) card_duration = max_card_duration;
      if(card_duration < min_card_duration) card_duration = min_card_duration;
      break;
      // Injury duration
      case 103:
      injury_duration = time_to_set;
      if(injury_duration > max_injury_duration) injury_duration = max_injury_duration;
      if(injury_duration < min_injury_duration) injury_duration = min_injury_duration;
      break;
    }
    // up_single_click_handler_do();
  }
  if(command_key == 200){
    sync_durations(match_duration, card_duration, injury_duration);
  }
  settings_window_selection = settings_window_selection_old;
}*/

// Key-press emulation from companion app - DEPRECATED
void emulate_key_press_settings(int command_key, int command_value){
  if(command_key == 201){
    select_long_click_handler_do();
  }
  // Handle direct call to set a specific value for the durations
  static long time_to_set;
  time_to_set = (long) (command_value * time_unit);
  if(command_key == 10001){
    match_duration = time_to_set;
  }
  if(command_key == 10002){
    card_duration = time_to_set;
  }
  if(command_key == 10003){
    injury_duration = time_to_set;
  }
}

// Callback for seconds changed (main_loop)
static void tick_handler(struct tm *tick_time, TimeUnits units_changed){
  if(time(NULL) - last_button_press >= time_out_GUI || time(NULL) == next_expiring_timer){
    window_stack_remove(s_window, true);
  }
}

// Show settings window
void show_s_settings(void) {

  //APP_LOG(APP_LOG_LEVEL_DEBUG, "Start setting, Free memory: %d", heap_bytes_free());
  //window_stack_remove(m_window, true);
  // Create, set, and push settings window element
  s_window = window_create();
  initialise_ui();
  window_set_window_handlers(s_window, (WindowHandlers) {
    .unload = handle_window_unload,
  });
  window_stack_push(s_window, true);

  // Register time and click services
  tick_timer_service_subscribe(SECOND_UNIT, tick_handler);
  window_set_click_config_provider(s_window, (ClickConfigProvider) config_provider);

  // Init GUI
  if(number_events > 0) last_shown_event = number_events - 1; else last_shown_event = 0;
  last_button_press = time(NULL);
  update_GUI();
  //APP_LOG(APP_LOG_LEVEL_DEBUG, "Setting started, Free memory: %d", heap_bytes_free());
}

