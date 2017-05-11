#include "s_main_window.h"
#include "s_settings.h"
#include "s_keyboard.h"
#include <pebble.h>

// Keys for persistent storage
#define WAKEUP_REASON 0
#define NUM_LASTSTART_PKEY 1
#define NUM_LASTPAUSE_PKEY 2
#define NUM_CLOCKRUNNING_PKEY 3
#define NUM_SCOREA_PKEY 7
#define NUM_SCOREB_PKEY 12
#define NUM_SCORETYPEA_PKEY 17
#define NUM_SCORETYPEB_PKEY 18
#define NUM_LASTSCOREA_PKEY 19
#define NUM_LASTSCOREB_PKEY 20
#define NUM_NUMBERCARDS_PKEY 21
#define NUM_CARDS_PKEY 22
#define NUM_NUMBERINJURIES_PKEY 32
#define NUM_INJURIES_PKEY 33
#define NUM_MATCHTIME_PKEY 43
#define NUM_PERIODNOTIFIED_PKEY 44
#define NUM_CURRENTPERIOD_PKEY 45
#define PERSIST_KEY_WAKEUP_ID 46
#define SUBS_A_PKEY 47
#define SUBS_B_PKEY 48
#define NUM_TICKLASTPAUSE_PKEY 49
#define NUM_SCOREVALUES_PKEY 50
#define NUM_NUMBEREVENTS_PKEY 54
#define NUM_EVENTS_PKEY 60
#define NUM_IDENTIFIERS_PKEY 55
#define NOTES_SIZE_PKEY 200
#define NOTES_PKEY 201

// Initialize variables
static long current_time = 0;
static long match_time = 0;
static long tick_time_last_start = 0;
static long tick_time_last_pause = 0;
static long time_last_pause = 0;
static long remaining_time = 0;
static int Run = 0;
static int main_window_selection = 1;
static int pressed_message;
static int period_notified = 0;
static char score_type_names[4][11] = {"Try", "Conversion", "Penalty", "Drop"};
static int score_types[4] = {5, 2, 3, 3};
static int score_A[5] = {0, 0, 0, 0, 0};
static int score_B[5] = {0, 0, 0, 0, 0};
static int score_type_A = 4;
static int score_type_B = 4;
static int last_score_A = 4;
static int last_score_B = 4;
static int number_cards = 0;
static int cards[10];
static int number_injuries = 0;
static int injuries[10];
static long last_button_press = 0;
static long GUI_time_out = 10;
static WakeupId s_wakeup_id;
static int returned_text;
static char subs_identifier[1] = "-";

// BEGIN AUTO-GENERATED UI CODE; DO NOT MODIFY * SLIGHTLY MODIFIED! *
//static Window *s_window;
static GFont s_res_gothic_14;
static GFont s_res_gothic_28;
static GFont s_res_gothic_18;
static GFont s_res_gothic_18_bold;
static GFont s_res_gothic_24;
static TextLayer *s_instructions;
static TextLayer *s_result;
static TextLayer *s_drops_A;
static TextLayer *s_pens_A;
static TextLayer *s_convs_A;
static TextLayer *s_tries_A;
static TextLayer *s_drops_B;
static TextLayer *s_pens_B;
static TextLayer *s_convs_B;
static TextLayer *s_tries_B;
static TextLayer *s_next_card;
static TextLayer *s_card_title;
static TextLayer *s_next_injury;
static TextLayer *s_injury_title;
static TextLayer *s_time_left;
static TextLayer *s_time_elapsed;
static TextLayer *s_time;
static TextLayer *s_date;
static BitmapLayer *s_line_1;
static BitmapLayer *s_line_2;
static BitmapLayer *s_line_3;
static BitmapLayer *s_line_4;
static BitmapLayer *s_selection_layer;
// static InverterLayer *s_selection_layer;
//static Window *m_window;

static void initialise_ui(void) {
  //m_window = window_create();
  
  #ifdef PBL_SDK_2
  window_set_fullscreen(m_window, true);
  #endif
    
  s_res_gothic_14 = fonts_get_system_font(FONT_KEY_GOTHIC_14);
  s_res_gothic_28 = fonts_get_system_font(FONT_KEY_GOTHIC_28);
  s_res_gothic_18 = fonts_get_system_font(FONT_KEY_GOTHIC_18);
  s_res_gothic_18_bold = fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD);
  s_res_gothic_24 = fonts_get_system_font(FONT_KEY_GOTHIC_24);

  // s_selection_layer
  s_selection_layer = bitmap_layer_create(GRect(0, 21, 144, 23));
  bitmap_layer_set_background_color(s_selection_layer, GColorBlack);
  layer_add_child(window_get_root_layer(m_window), (Layer *)s_selection_layer);

  // s_instructions
  s_instructions = text_layer_create(GRect(0, 130, 144, 34));
  text_layer_set_text(s_instructions, "U = start/stop, D = note, Long U/D = New Half/Match");
  text_layer_set_font(s_instructions, s_res_gothic_14);
  layer_add_child(window_get_root_layer(m_window), (Layer *)s_instructions);
  
  // s_result
  s_result = text_layer_create(GRect(2, 88, 140, 28));
  text_layer_set_text(s_result, "0 - 0");
  text_layer_set_text_alignment(s_result, GTextAlignmentCenter);
  text_layer_set_font(s_result, s_res_gothic_28);
  text_layer_set_text_color(s_result, GColorBlack);
  text_layer_set_background_color(s_result, GColorClear);
  layer_add_child(window_get_root_layer(m_window), (Layer *)s_result);
  
  // s_drops_A
  s_drops_A = text_layer_create(GRect(14, 103, 18, 18));
  text_layer_set_text(s_drops_A, "0");
  text_layer_set_text_alignment(s_drops_A, GTextAlignmentCenter);
  text_layer_set_font(s_drops_A, s_res_gothic_18);
  text_layer_set_text_color(s_drops_A, GColorBlack);
  text_layer_set_background_color(s_drops_A, GColorClear);
  layer_add_child(window_get_root_layer(m_window), (Layer *)s_drops_A);
  
  // s_pens_A
  s_pens_A = text_layer_create(GRect(0, 103, 18, 18));
  text_layer_set_text(s_pens_A, "0");
  text_layer_set_text_alignment(s_pens_A, GTextAlignmentCenter);
  text_layer_set_font(s_pens_A, s_res_gothic_18);
  text_layer_set_text_color(s_pens_A, GColorBlack);
  text_layer_set_background_color(s_pens_A, GColorClear);
  layer_add_child(window_get_root_layer(m_window), (Layer *)s_pens_A);
  
  // s_convs_A
  s_convs_A = text_layer_create(GRect(14, 84, 18, 18));
  text_layer_set_text(s_convs_A, "0");
  text_layer_set_text_alignment(s_convs_A, GTextAlignmentCenter);
  text_layer_set_font(s_convs_A, s_res_gothic_18);
  text_layer_set_text_color(s_convs_A, GColorBlack);
  text_layer_set_background_color(s_convs_A, GColorClear);
  layer_add_child(window_get_root_layer(m_window), (Layer *)s_convs_A);
  
  // s_tries_A
  s_tries_A = text_layer_create(GRect(0, 84, 18, 18));
  text_layer_set_text(s_tries_A, "0");
  text_layer_set_text_alignment(s_tries_A, GTextAlignmentCenter);
  text_layer_set_font(s_tries_A, s_res_gothic_18_bold);
  text_layer_set_text_color(s_tries_A, GColorBlack);
  text_layer_set_background_color(s_tries_A, GColorClear);
  layer_add_child(window_get_root_layer(m_window), (Layer *)s_tries_A);
  
  // s_drops_B
  s_drops_B = text_layer_create(GRect(126, 103, 18, 18));
  text_layer_set_text(s_drops_B, "0");
  text_layer_set_text_alignment(s_drops_B, GTextAlignmentCenter);
  text_layer_set_font(s_drops_B, s_res_gothic_18);
  text_layer_set_text_color(s_drops_B, GColorBlack);
  text_layer_set_background_color(s_drops_B, GColorClear);
  layer_add_child(window_get_root_layer(m_window), (Layer *)s_drops_B);
  
  // s_pens_B
  s_pens_B = text_layer_create(GRect(112, 103, 18, 18));
  text_layer_set_text(s_pens_B, "0");
  text_layer_set_text_alignment(s_pens_B, GTextAlignmentCenter);
  text_layer_set_font(s_pens_B, s_res_gothic_18);
  text_layer_set_text_color(s_pens_B, GColorBlack);
  text_layer_set_background_color(s_pens_B, GColorClear);
  layer_add_child(window_get_root_layer(m_window), (Layer *)s_pens_B);
  
  // s_convs_B
  s_convs_B = text_layer_create(GRect(126, 84, 18, 18));
  text_layer_set_text(s_convs_B, "0");
  text_layer_set_text_alignment(s_convs_B, GTextAlignmentCenter);
  text_layer_set_font(s_convs_B, s_res_gothic_18);
  text_layer_set_text_color(s_convs_B, GColorBlack);
  text_layer_set_background_color(s_convs_B, GColorClear);
  layer_add_child(window_get_root_layer(m_window), (Layer *)s_convs_B);
  
  // s_tries_B
  s_tries_B = text_layer_create(GRect(112, 84, 18, 18));
  text_layer_set_text(s_tries_B, "0");
  text_layer_set_text_alignment(s_tries_B, GTextAlignmentCenter);
  text_layer_set_font(s_tries_B, s_res_gothic_18_bold);
  text_layer_set_text_color(s_tries_B, GColorBlack);
  text_layer_set_background_color(s_tries_B, GColorClear);
  layer_add_child(window_get_root_layer(m_window), (Layer *)s_tries_B);
  
  // s_next_card
  s_next_card = text_layer_create(GRect(2, 62, 70, 20));
  text_layer_set_text(s_next_card, "None");
  text_layer_set_font(s_next_card, s_res_gothic_18);
  text_layer_set_text_color(s_next_card, GColorBlack);
  text_layer_set_background_color(s_next_card, GColorClear);
  layer_add_child(window_get_root_layer(m_window), (Layer *)s_next_card);
  
  // s_card_title
  s_card_title = text_layer_create(GRect(2, 42, 70, 20));
  text_layer_set_text(s_card_title, "Next Card:");
  text_layer_set_font(s_card_title, s_res_gothic_18);
  text_layer_set_text_color(s_card_title, GColorBlack);
  text_layer_set_background_color(s_card_title, GColorClear);
  layer_add_child(window_get_root_layer(m_window), (Layer *)s_card_title);
  
  // s_next_injury
  s_next_injury = text_layer_create(GRect(70, 62, 70, 20));
  text_layer_set_text(s_next_injury, "None");
  text_layer_set_font(s_next_injury, s_res_gothic_18);
  text_layer_set_text_color(s_next_injury, GColorBlack);
  text_layer_set_background_color(s_next_injury, GColorClear);
  layer_add_child(window_get_root_layer(m_window), (Layer *)s_next_injury);
  
  // s_injury_title
  s_injury_title = text_layer_create(GRect(70, 42, 70, 20));
  text_layer_set_text(s_injury_title, "Next Injury:");
  text_layer_set_font(s_injury_title, s_res_gothic_18);
  text_layer_set_text_color(s_injury_title, GColorBlack);
  text_layer_set_background_color(s_injury_title, GColorClear);
  layer_add_child(window_get_root_layer(m_window), (Layer *)s_injury_title);
  
  // s_time_left
  s_time_left = text_layer_create(GRect(70, 16, 70, 24));
  text_layer_set_text(s_time_left, "40:00");
  text_layer_set_text_color(s_time_left, GColorWhite);
  text_layer_set_background_color(s_time_left, GColorClear);
  text_layer_set_text_alignment(s_time_left, GTextAlignmentCenter);
  text_layer_set_font(s_time_left, s_res_gothic_24);
  layer_add_child(window_get_root_layer(m_window), (Layer *)s_time_left);
  
  // s_time_elapsed
  s_time_elapsed = text_layer_create(GRect(2, 16, 70, 24));
  text_layer_set_text(s_time_elapsed, "0:00");
  text_layer_set_text_color(s_time_elapsed, GColorWhite);
  text_layer_set_background_color(s_time_elapsed, GColorClear);
  text_layer_set_text_alignment(s_time_elapsed, GTextAlignmentCenter);
  text_layer_set_font(s_time_elapsed, s_res_gothic_24);
  layer_add_child(window_get_root_layer(m_window), (Layer *)s_time_elapsed);
  
  // s_time
  s_time = text_layer_create(GRect(100, -2, 44, 20));
  text_layer_set_text(s_time, "13:00");
  text_layer_set_text_alignment(s_time, GTextAlignmentCenter);
  text_layer_set_font(s_time, s_res_gothic_18);
  layer_add_child(window_get_root_layer(m_window), (Layer *)s_time);
  
  // s_date
  s_date = text_layer_create(GRect(0, -2, 100, 20));
  text_layer_set_text(s_date, "Sat 01-01-2015");
  text_layer_set_text_alignment(s_date, GTextAlignmentCenter);
  text_layer_set_font(s_date, s_res_gothic_18);
  layer_add_child(window_get_root_layer(m_window), (Layer *)s_date);
  
  // s_line_1
  s_line_1 = bitmap_layer_create(GRect(0, 19, 144, 2));
  bitmap_layer_set_background_color(s_line_1, GColorBlack);
  layer_add_child(window_get_root_layer(m_window), (Layer *)s_line_1);
  
  // s_line_2
  s_line_2 = bitmap_layer_create(GRect(0, 44, 144, 2));
  bitmap_layer_set_background_color(s_line_2, GColorBlack);
  layer_add_child(window_get_root_layer(m_window), (Layer *)s_line_2);
  
  // s_line_3
  s_line_3 = bitmap_layer_create(GRect(0, 85, 144, 2));
  bitmap_layer_set_background_color(s_line_3, GColorBlack);
  layer_add_child(window_get_root_layer(m_window), (Layer *)s_line_3);
  
  // s_line_4
  s_line_4 = bitmap_layer_create(GRect(0, 128, 144, 2));
  bitmap_layer_set_background_color(s_line_4, GColorBlack);
  layer_add_child(window_get_root_layer(m_window), (Layer *)s_line_4);
  
  // s_selection_layer
  // s_selection_layer = inverter_layer_create(GRect(0, 21, 144, 19));
  // layer_add_child(window_get_root_layer(m_window), (Layer *)s_selection_layer);
}

static void destroy_ui(void) {
  //window_destroy(m_window);
  text_layer_destroy(s_instructions);
  text_layer_destroy(s_result);
  text_layer_destroy(s_drops_A);
  text_layer_destroy(s_pens_A);
  text_layer_destroy(s_convs_A);
  text_layer_destroy(s_tries_A);
  text_layer_destroy(s_drops_B);
  text_layer_destroy(s_pens_B);
  text_layer_destroy(s_convs_B);
  text_layer_destroy(s_tries_B);
  text_layer_destroy(s_next_card);
  text_layer_destroy(s_card_title);
  text_layer_destroy(s_next_injury);
  text_layer_destroy(s_injury_title);
  text_layer_destroy(s_time_left);
  text_layer_destroy(s_time_elapsed);
  text_layer_destroy(s_time);
  text_layer_destroy(s_date);
  bitmap_layer_destroy(s_line_1);
  bitmap_layer_destroy(s_line_2);
  bitmap_layer_destroy(s_line_3);
  bitmap_layer_destroy(s_line_4);
  bitmap_layer_destroy(s_selection_layer);
}
// END AUTO-GENERATED UI CODE

/* // Handling wakeup: delete the ID
static void wakeup_handler(WakeupId id, int32_t reason) {
  persist_delete(PERSIST_KEY_WAKEUP_ID);
}*/

// Search string for specific character
static bool search_string(const char *string, size_t length, char character){
  for (int i = 0; i < (int) length; i++){
    if(string[i] == character){
      return true;
    }
  }
  return false;
}

// Update match timers
static void update_timers(){
  static char setbuffer_elapsed[] = "00:00";
  static char setbuffer_left[] = " 40:00";
  // Format and display elapsed and remaining match time
  snprintf(setbuffer_elapsed, sizeof("00:00"), "%02d:%02d", (int) (match_time / 60), (int) (match_time % 60));
  text_layer_set_text(s_time_elapsed, setbuffer_elapsed);
  // Remaining time may be negative (loss time)
  if (remaining_time < 0){
    snprintf(setbuffer_left, sizeof(" 00:00"), "-%02d:%02d", (int) abs(remaining_time / 60), (int) abs(remaining_time % 60));
  } else {
    snprintf(setbuffer_left, sizeof(" 00:00"), " %02d:%02d", (int) (remaining_time / 60), (int) (remaining_time % 60));
  }
  text_layer_set_text(s_time_left, setbuffer_left);
  // Update card timer with last card if any
  if(number_cards > 0){
    static char setbuffer_card[] = "00:00";
    int delta_time = (int) (cards[0] - match_time);
    if(delta_time < 0) delta_time = 0;
    snprintf(setbuffer_card, sizeof("00:00"), "%02d:%02d", delta_time / 60, delta_time % 60);
    text_layer_set_text(s_next_card, setbuffer_card);
  }
  // Update injury timer with last injury if any
  if(number_injuries > 0){
    static char setbuffer_injury[] = "00:00";
    int delta_time = (int) (injuries[0] - current_time);
    if(delta_time < 0) delta_time = 0;
    snprintf(setbuffer_injury, sizeof("00:00"), "%02d:%02d", delta_time / 60, delta_time % 60);
    text_layer_set_text(s_next_injury, setbuffer_injury);
  }
}

// Find next expiring timer
static int find_next_expiring_timer(){
  int shortest_time = -1;
  // Clock is running: look for period, cards, and injuries
  if(Run == 1){
    shortest_time = remaining_time;
    if(number_cards > 0){
      if(cards[0] - match_time < shortest_time){
        shortest_time = cards[0] - match_time;
      }
    }
    if(number_injuries > 0){
      if(injuries[0] - current_time < shortest_time){
        shortest_time = injuries[0] - current_time;
      }
    }
  // Clock is paused: look only for injuries
  } else {
    if(number_injuries > 0){
      shortest_time = injuries[0] - current_time;
    }    
  }
  return shortest_time;
}

// Write persistent settings
static void write_persist()
{
  // Store all relevant data for safe resume
  // General time settings
  persist_write_int(NUM_MATCHLENGTH_PKEY, match_duration);
  persist_write_int(NUM_CARDLENGTH_PKEY, card_duration);
  persist_write_int(NUM_INJURYLENGTH_PKEY, injury_duration);
  // Current match time values
  persist_write_int(NUM_LASTSTART_PKEY, tick_time_last_start);
  persist_write_int(NUM_TICKLASTPAUSE_PKEY, tick_time_last_pause);
  persist_write_int(NUM_LASTPAUSE_PKEY, time_last_pause);
  persist_write_int(NUM_CLOCKRUNNING_PKEY, Run);
  persist_write_int(NUM_MATCHTIME_PKEY, match_time);
  persist_write_int(NUM_PERIODNOTIFIED_PKEY, period_notified);
  persist_write_int(NUM_CURRENTPERIOD_PKEY, current_period);
  // Score points
  for(int i = 0; i < 4; i++){
    persist_write_int(NUM_SCOREVALUES_PKEY + i, score_types[i]);
  }
  // Score values
  for(int i = 0; i < 5; i++){
    persist_write_int(NUM_SCOREA_PKEY + i, score_A[i]);
    persist_write_int(NUM_SCOREB_PKEY + i, score_B[i]);
  }
  persist_write_int(NUM_SCORETYPEA_PKEY, score_type_A);
  persist_write_int(NUM_SCORETYPEB_PKEY, score_type_B);
  persist_write_int(NUM_LASTSCOREA_PKEY, last_score_A);
  persist_write_int(NUM_LASTSCOREB_PKEY, last_score_B);
  // Cards values
  persist_write_int(NUM_NUMBERCARDS_PKEY, number_cards);
  for(int i = 0; i < number_cards; i++){
    persist_write_int(NUM_CARDS_PKEY + i, cards[i]);
  }
  // Injuries values
  persist_write_int(NUM_NUMBERINJURIES_PKEY, number_injuries);
  for(int i = 0; i < number_injuries; i++){
    persist_write_int(NUM_INJURIES_PKEY + i, injuries[i]);
  }
  // History events values
  persist_write_int(NUM_NUMBEREVENTS_PKEY, number_events);
  for(int i = 0; i < number_events; i++){
    for(int ii = 0; ii < 3; ii++) persist_write_int(NUM_EVENTS_PKEY + 3 * i + ii, events[i][ii]);
  }
  // Notes values
  persist_write_int(NOTES_SIZE_PKEY, notes_size);
  persist_write_string(NOTES_PKEY, notes);
  // Subs values
  persist_write_int(SUBS_A_PKEY, subs_a);
  persist_write_int(SUBS_B_PKEY, subs_b);
  // Identifying chars
  //APP_LOG(APP_LOG_LEVEL_ERROR, "Storing subs identifiers %s", subs_identifier);
  persist_write_string(NUM_IDENTIFIERS_PKEY, subs_identifier);
  //APP_LOG(APP_LOG_LEVEL_ERROR, "Storing team identifiers %s", team_identifier);
  persist_write_string(NUM_IDENTIFIERS_PKEY + 1, team_identifier);

  // Get next expiring timer
  time_t time_now;
  time_now = time(NULL);
  time_t schedule_time;
  int shortest_time;
  shortest_time = find_next_expiring_timer();
  // Set an alarm to wake up if necessary
  if(shortest_time > 0){
    schedule_time = time_now + shortest_time;
    // Check the event is not already scheduled
    if (!wakeup_query(s_wakeup_id, NULL)) {
      // Schedule wakeup event and keep the WakeupId
      s_wakeup_id = wakeup_schedule(schedule_time, WAKEUP_REASON, true);
      persist_write_int(PERSIST_KEY_WAKEUP_ID, s_wakeup_id);
    }
  }
}

// Read settings from persistent storage
static void ReadSettings()
{
  // General time settings
  if(persist_exists(NUM_MATCHLENGTH_PKEY)) match_duration = persist_read_int(NUM_MATCHLENGTH_PKEY);
  if(persist_exists(NUM_CARDLENGTH_PKEY)) card_duration = persist_read_int(NUM_CARDLENGTH_PKEY);
  if(persist_exists(NUM_INJURYLENGTH_PKEY)) injury_duration = persist_read_int(NUM_INJURYLENGTH_PKEY);
  // Current match time values
  if(persist_exists(NUM_LASTSTART_PKEY)) tick_time_last_start = persist_read_int(NUM_LASTSTART_PKEY);
  if(persist_exists(NUM_TICKLASTPAUSE_PKEY)) tick_time_last_pause = persist_read_int(NUM_TICKLASTPAUSE_PKEY);
  if(persist_exists(NUM_LASTPAUSE_PKEY)) time_last_pause = persist_read_int(NUM_LASTPAUSE_PKEY);
  if(persist_exists(NUM_CLOCKRUNNING_PKEY)) Run = persist_read_int(NUM_CLOCKRUNNING_PKEY);
  if(persist_exists(NUM_MATCHTIME_PKEY)) match_time = persist_read_int(NUM_MATCHTIME_PKEY);
  if(persist_exists(NUM_PERIODNOTIFIED_PKEY)) period_notified = persist_read_int(NUM_PERIODNOTIFIED_PKEY);
  if(persist_exists(NUM_CURRENTPERIOD_PKEY)) current_period = persist_read_int(NUM_CURRENTPERIOD_PKEY);
  //send_int(0, Run);
  // Score points
  for(int i = 0; i < 4; i++){
    if(persist_exists(NUM_SCOREVALUES_PKEY + i)) score_types[i] = persist_read_int(NUM_SCOREVALUES_PKEY + i);
  }
  // Score values
  for(int i = 0; i < 5; i++){
    if(persist_exists(NUM_SCOREA_PKEY + i)) score_A[i] = persist_read_int(NUM_SCOREA_PKEY + i);
    if(persist_exists(NUM_SCOREB_PKEY + i)) score_B[i] = persist_read_int(NUM_SCOREB_PKEY + i);
  }
  if(persist_exists(NUM_SCORETYPEA_PKEY)) score_type_A = persist_read_int(NUM_SCORETYPEA_PKEY);
  if(persist_exists(NUM_SCORETYPEB_PKEY)) score_type_B = persist_read_int(NUM_SCORETYPEB_PKEY);
  if(persist_exists(NUM_LASTSCOREA_PKEY)) last_score_A = persist_read_int(NUM_LASTSCOREA_PKEY);
  if(persist_exists(NUM_LASTSCOREB_PKEY)) last_score_B = persist_read_int(NUM_LASTSCOREB_PKEY);
  // Cards values
  if(persist_exists(NUM_NUMBERCARDS_PKEY)) {
    number_cards = persist_read_int(NUM_NUMBERCARDS_PKEY);
    for(int i = 0; i < number_cards; i++){
      if(persist_exists(NUM_CARDS_PKEY + i)) cards[i] = persist_read_int(NUM_CARDS_PKEY + i);
    }
  }
  // Injuries values
  if(persist_exists(NUM_NUMBERINJURIES_PKEY)) {
    number_injuries = persist_read_int(NUM_NUMBERINJURIES_PKEY);
    for(int i = 0; i < number_injuries; i++){
      if(persist_exists(NUM_INJURIES_PKEY + i)) injuries[i] = persist_read_int(NUM_INJURIES_PKEY + i);
    }
  }
  // History events values
  if(persist_exists(NUM_NUMBEREVENTS_PKEY)) number_events = persist_read_int(NUM_NUMBEREVENTS_PKEY);
  for(int i = 0; i < number_events; i++){
    for(int ii = 0; ii < 3; ii++) if(persist_exists(NUM_EVENTS_PKEY + 3 * i + ii)) events[i][ii] = persist_read_int(NUM_EVENTS_PKEY + 3 * i + ii);
  }
  // Notes values
  if(persist_exists(NOTES_SIZE_PKEY)) notes_size = persist_read_int(NOTES_SIZE_PKEY);
  if(persist_exists(NOTES_PKEY)) persist_read_string(NOTES_PKEY, notes, MAX_NOTES_LENGTH);
  // Subs values
  if(persist_exists(SUBS_A_PKEY)) subs_a = persist_read_int(SUBS_A_PKEY);
  if(persist_exists(SUBS_B_PKEY)) subs_b = persist_read_int(SUBS_B_PKEY);

  // Identifying chars
  if(persist_exists(NUM_IDENTIFIERS_PKEY)) persist_read_string(NUM_IDENTIFIERS_PKEY, subs_identifier, 2);
  //APP_LOG(APP_LOG_LEVEL_ERROR, "Read subs identifier %c", subs_identifier[0]);
  if(persist_exists(NUM_IDENTIFIERS_PKEY + 1)) persist_read_string(NUM_IDENTIFIERS_PKEY + 1, team_identifier, 3);
  //APP_LOG(APP_LOG_LEVEL_ERROR, "Read team identifiers %s", team_identifier);

  // Sync with phone app - DEPRECATED
  /*send_int(0, Run);
  send_int(211, match_duration);
  send_int(221, card_duration);
  send_int(231, injury_duration);*/
  /*int key[] = {0, 211, 221, 231};
  int value[] = {Run, match_duration, card_duration, injury_duration};
  send_block(key, value, 4);*/
}

// Update score layer
static void update_scores(){
  static char setbuffer_score[] = "000 - 000";
  snprintf(setbuffer_score, sizeof("000 - 000"), "%d - %d", score_A[4], score_B[4]);
  text_layer_set_text(s_result, setbuffer_score);
  static char setbuffer_triesA[] = "00";
  snprintf(setbuffer_triesA, sizeof("00"), "%d", score_A[0]);
  text_layer_set_text(s_tries_A, setbuffer_triesA);
  static char setbuffer_convsA[] = "00";
  snprintf(setbuffer_convsA, sizeof("00"), "%d", score_A[1]);
  text_layer_set_text(s_convs_A, setbuffer_convsA);
  static char setbuffer_pensA[] = "00";
  snprintf(setbuffer_pensA, sizeof("00"), "%d", score_A[2]);
  text_layer_set_text(s_pens_A, setbuffer_pensA);
  static char setbuffer_dropsA[] = "00";
  snprintf(setbuffer_dropsA, sizeof("00"), "%d", score_A[3]);
  text_layer_set_text(s_drops_A, setbuffer_dropsA);
  static char setbuffer_triesB[] = "00";
  snprintf(setbuffer_triesB, sizeof("00"), "%d", score_B[0]);
  text_layer_set_text(s_tries_B, setbuffer_triesB);
  static char setbuffer_convsB[] = "00";
  snprintf(setbuffer_convsB, sizeof("00"), "%d", score_B[1]);
  text_layer_set_text(s_convs_B, setbuffer_convsB);
  static char setbuffer_pensB[] = "00";
  snprintf(setbuffer_pensB, sizeof("00"), "%d", score_B[2]);
  text_layer_set_text(s_pens_B, setbuffer_pensB);
  static char setbuffer_dropsB[] = "00";
  snprintf(setbuffer_dropsB, sizeof("00"), "%d", score_B[3]);
  text_layer_set_text(s_drops_B, setbuffer_dropsB);
}

// Kill UI on disappearing
static void handle_window_disappear(Window* window) {
  //APP_LOG( APP_LOG_LEVEL_INFO, "Main disappear: %zu", heap_bytes_free());
  write_persist();
  destroy_ui();
  tick_timer_service_unsubscribe();
}

// Kill UI and window on unload
static void handle_window_unload(Window* window) {
  //write_persist();
  //destroy_ui();
  //window_destroy(m_window);
  //tick_timer_service_unsubscribe();
}

// Check all timers for expiration
static void check_timers(){
  // check if next injury exists and update
  if(number_injuries > 0){
    static char setbuffer_injury[] = "00:00";
    int delta_time;
    delta_time = (int) (injuries[0] - current_time);
    if(delta_time < 0) delta_time = 0;
    snprintf(setbuffer_injury, sizeof("00:00"), "%02d:%02d", delta_time / 60, delta_time % 60);
    text_layer_set_text(s_next_injury, setbuffer_injury);
    // check if next injury expires: vibrate and light up
    if(current_time >= injuries[0]){
      vibes_long_pulse();
      light_enable_interaction();
      text_layer_set_text(s_instructions, "Injury expired");
      // "pop" first injury
      if(number_injuries > 1){
        for(int i = 0; i < number_injuries - 1; i++){
          injuries[i] = injuries[i + 1];
        }
      }
      injuries[number_injuries - 1] = 0;
      number_injuries--;
      }
    // No injuries: Reset text
  } else {
    text_layer_set_text(s_next_injury, "None");
  }
  // Vibrate every 15 seconds if clock is paused
  if (Run == 0 && tick_time_last_pause != 0){
    if(((long) time(NULL) - tick_time_last_pause + 1) % 15 == 0){
      vibes_short_pulse();
    }
  }
  // Update match and cards time if clock is running
  if (Run == 1){
    match_time = current_time - tick_time_last_start + time_last_pause;
    remaining_time = match_duration - match_time;
    // check if period expires
    if((match_time >= match_duration) && (period_notified == 0)){
      period_notified = 1;
      vibes_long_pulse();
      light_enable_interaction();
      text_layer_set_text(s_instructions, "End of period");
    }
    // check if next card exists and update
    if(number_cards > 0){
      static char setbuffer_card[] = "00:00";
      int delta_time;
      delta_time = (int) (cards[0] - match_time);
      if(delta_time < 0) delta_time = 0;
      snprintf(setbuffer_card, sizeof("00:00"), "%02d:%02d", delta_time / 60, delta_time % 60);
      text_layer_set_text(s_next_card, setbuffer_card);
    // check if next card expires: vibrate and light up
      if(match_time >= cards[0]){
        vibes_long_pulse();
        light_enable_interaction();
        text_layer_set_text(s_instructions, "Card expired");
      // "pop" first card
        if(number_cards > 1){
          for(int i = 0; i < number_cards - 1; i++){
            cards[i] = cards[i + 1];
          }
        }
        cards[number_cards - 1] = 0;
        number_cards--;
      }
    // No cards: Reset text
    } else {
      text_layer_set_text(s_next_card, "None");
    }
  }  
}

// Callback from tertiary keyboard
static void T3_callback( const char* result, size_t result_length, void* extra )
{
  // Add event time and note position in notes buffer if within limits
  returned_text = 1;
  if(number_events < MAX_EVENTS){
    events[number_events][0] = ((int) match_time) + 5000 * current_period;
    events[number_events][1] = notes_size + 8;
    // Append keyboard result to notes buffer, crop if needed
    for (int i = 0; i < (int) result_length; i++){
      if (notes_size < MAX_NOTES_LENGTH){
        notes[notes_size] = result[i];
        notes_size++;
      }
    }
    // Add appropriate note length, crop if out of bounds
    if (notes_size < MAX_NOTES_LENGTH){
      events[number_events][2] = result_length;
    }
    else {
      events[number_events][2] = MAX_NOTES_LENGTH + 8 - events[number_events][1];      
    }
    // Update counter
    number_events++;
    // Check if substitution
    if (search_string(result, result_length, subs_identifier[0])){
      if (search_string(result, result_length, team_identifier[0])){
        subs_a += 1;
      }
      if (search_string(result, result_length, team_identifier[1])){
        subs_b += 1;
      }
    }
  }
}

// Handle single click up
static void up_single_click_handler_do() {
  pressed_message = match_time;
  // Time selection
  if (main_window_selection == 1){
    // Toggle clock state and vibrate
    if (Run == 1){
      Run = 0;
      time_last_pause = match_time;
      tick_time_last_pause = (long) time(NULL);
      pressed_message = -match_time;
    }
    else{
      Run = 1;
      tick_time_last_start = time(NULL);
      pressed_message = match_time;
    }
    vibes_short_pulse();
  }
  // Cards selection
  if (main_window_selection == 2){
    // Add a card
    vibes_short_pulse();
    if(number_cards < 10){
      cards[number_cards] = match_time + card_duration;
      number_cards++;
      if(number_cards > 10) number_cards = 10;
      // Update card timer
      static char setbuffer_card[] = "00:00";
      snprintf(setbuffer_card, sizeof("00:00"), "%02d:%02d", (int) (cards[0] - match_time) / 60, (int) (cards[0] - match_time) % 60);
      text_layer_set_text(s_next_card, setbuffer_card);
      text_layer_set_text(s_instructions, "Card added");
      // Force time stop and acquire tertiary keyboard input
      Run = 0;
      time_last_pause = match_time;
      if(match_time != 0){
        tick_time_last_pause = (long) time(NULL);
      }
      pressed_message = -match_time;
      vibes_short_pulse();
      next_expiring_timer = time(NULL) + find_next_expiring_timer();
      returned_text = 0;
      tertiary_text_prompt( "Input your note", T3_callback, NULL );
      if(returned_text == 0){
      // Log card event
        if(number_events < MAX_EVENTS){
          events[number_events][0] = ((int) match_time) + 5000 * current_period;
          events[number_events][1] = 5;
          events[number_events][2] = 0;
          number_events++;
        }        
      }
    }
  }
  // Score selection
  if (main_window_selection == 3){
    score_type_B = 4;
    // Switch between score types
    if(score_type_A > 3){
      score_type_A = 0;
    } else{
      score_type_A++;
    }
    // Store currently selected score type in instructions layer
    static char score_instruction[] = "Conversion Team A";
    if(score_type_A > 3){
      static char temp_buffer[] = "U/D (1-4) = Team A/B (T/C/P/D), Long = Undo last";
      snprintf(temp_buffer, sizeof(temp_buffer), "U/D (1-4) = Team %c/%c (T/C/P/D), Long = Undo last", team_identifier[0], team_identifier[1]);
      //text_layer_set_text(s_instructions, "U/D (1-4) = Team A/B (T/C/P/D), Long = Undo last");
      text_layer_set_text(s_instructions, temp_buffer);
    } else {
      snprintf(score_instruction, sizeof("Conversion Team A"), "%s Team %c", score_type_names[score_type_A], team_identifier[0]);
      //snprintf(score_instruction, sizeof("Conversion Team A"), "%s Team A", score_type_names[score_type_A]);
      text_layer_set_text(s_instructions, score_instruction);
    }
  }
}
static void up_single_click_handler(ClickRecognizerRef recognizer, void *context) {
  last_button_press = time(NULL);
  up_single_click_handler_do();
  if(main_window_selection != 3){
    //send_int(main_window_selection * 10 + 100, pressed_message);
  }
}

// Handle single click down
static void down_single_click_handler_do() {
  // Time selection
    if (main_window_selection == 1){
      // Force time stop and acquire tertiary keyboard input
      Run = 0;
      time_last_pause = match_time;
      if(match_time != 0){
        tick_time_last_pause = (long) time(NULL);
      }
      pressed_message = -match_time;
      vibes_short_pulse();
      next_expiring_timer = time(NULL) + find_next_expiring_timer();
      tertiary_text_prompt( "Input your note", T3_callback, NULL );
    }
  // Cards selection
  if (main_window_selection == 2){
    vibes_short_pulse();
    if(number_injuries < 10){
      // Add an injury
      injuries[number_injuries] = current_time + injury_duration;
      number_injuries++;
      if(number_injuries > 10) number_injuries = 10;
      // Update injury timer
      static char setbuffer_card[] = "00:00";
      snprintf(setbuffer_card, sizeof("00:00"), "%02d:%02d", (int) (injuries[0] - current_time) / 60, (int) (injuries[0] - current_time) % 60);
      text_layer_set_text(s_next_injury, setbuffer_card);
      text_layer_set_text(s_instructions, "Injury added");
      // Log injury event
      if(number_events < MAX_EVENTS){
        events[number_events][0] = ((int) match_time) + 5000 * current_period;
        events[number_events][1] = 6;
        events[number_events][2] = 0;
        number_events++;
      }
      // Force time stop and acquire tertiary keyboard input
      Run = 0;
      time_last_pause = match_time;
      if(match_time != 0){
        tick_time_last_pause = (long) time(NULL);
      }
      pressed_message = -match_time;
      vibes_short_pulse();
      next_expiring_timer = time(NULL) + find_next_expiring_timer();
      returned_text = 0;
      tertiary_text_prompt( "Input your note", T3_callback, NULL );
      if(returned_text == 0){
        if(number_events < MAX_EVENTS){
          events[number_events][0] = ((int) match_time) + 5000 * current_period;
          events[number_events][1] = 6;
          events[number_events][2] = 0;
          number_events++;
        }
      }
    }
  }
  // Score selection
  if (main_window_selection == 3){
    score_type_A = 4;
    // Switch between score types
    if(score_type_B > 3){
      score_type_B = 0;
    } else{
      score_type_B++;
    }
    // Store currently selected score type in instructions layer
    static char score_instruction[] = "Conversion Team B";
    if(score_type_B > 3){
      static char temp_buffer[] = "U/D (1-4) = Team A/B (T/C/P/D), Long = Undo last";
      snprintf(temp_buffer, sizeof(temp_buffer), "U/D (1-4) = Team %c/%c (T/C/P/D), Long = Undo last", team_identifier[0], team_identifier[1]);
      //text_layer_set_text(s_instructions, "U/D (1-4) = Team A/B (T/C/P/D), Long = Undo last");
      text_layer_set_text(s_instructions, temp_buffer);
    } else {
      snprintf(score_instruction, sizeof("Conversion Team A"), "%s Team %c", score_type_names[score_type_B], team_identifier[1]);
      //snprintf(score_instruction, sizeof("Conversion Team B"), "%s Team B", score_type_names[score_type_B]);
      text_layer_set_text(s_instructions, score_instruction);
    }
  }
}
static void down_single_click_handler(ClickRecognizerRef recognizer, void *context) {
  last_button_press = time(NULL);
  down_single_click_handler_do();
  if(main_window_selection != 3){
    //send_int(main_window_selection * 10 + 101, match_time);
  }
}

// Handle single click select
static void select_single_click_handler_do() {
  pressed_message = match_time;
  // Switch from timer to cards/injuries
  if (main_window_selection == 1){
    // Move selection layer and show instructions
    layer_set_frame((Layer *)s_selection_layer, GRect(0, 46, 144, 39));
    text_layer_set_text_color(s_injury_title, GColorWhite);
    text_layer_set_text_color(s_next_injury, GColorWhite);
    text_layer_set_text_color(s_card_title, GColorWhite);
    text_layer_set_text_color(s_next_card, GColorWhite);
    text_layer_set_text_color(s_time_left, GColorBlack);
    text_layer_set_text_color(s_time_elapsed, GColorBlack);
    text_layer_set_text(s_instructions, "U/D = add card/injury,          Long U/D = Delete last c/i");
    main_window_selection = 2;
  }
  // Switch from cards/injuries to score
  else if (main_window_selection == 2){
    // Move selection layer and show instructions
    text_layer_set_text_color(s_injury_title, GColorBlack);
    text_layer_set_text_color(s_next_injury, GColorBlack);
    text_layer_set_text_color(s_card_title, GColorBlack);
    text_layer_set_text_color(s_next_card, GColorBlack);
    text_layer_set_text_color(s_result, GColorWhite);
    text_layer_set_text_color(s_drops_A, GColorWhite);
    text_layer_set_text_color(s_convs_A, GColorWhite);
    text_layer_set_text_color(s_tries_A, GColorWhite);
    text_layer_set_text_color(s_pens_A, GColorWhite);
    text_layer_set_text_color(s_drops_B, GColorWhite);
    text_layer_set_text_color(s_convs_B, GColorWhite);
    text_layer_set_text_color(s_tries_B, GColorWhite);
    text_layer_set_text_color(s_pens_B, GColorWhite);
    layer_set_frame((Layer *)s_selection_layer, GRect(0, 87, 144, 41));
    static char temp_buffer[] = "U/D (1-4) = Team A/B (T/C/P/D), Long = Undo last";
    snprintf(temp_buffer, sizeof(temp_buffer), "U/D (1-4) = Team %c/%c (T/C/P/D), Long = Undo last", team_identifier[0], team_identifier[1]);
    text_layer_set_text(s_instructions, temp_buffer);
    //text_layer_set_text(s_instructions, "U/D (1-4) = Team A/B (T/C/P/D), Long = Undo last");
    main_window_selection = 3;
  }
  // Switch from score to timer
  else if (main_window_selection == 3){
    // Leaving score layer: update score arrays
    // Team A
    pressed_message = 8; 
    if (score_type_A < 4){
      pressed_message = score_type_A;
      score_A[score_type_A]++;
      score_A[4] += score_types[score_type_A];
      last_score_A = score_type_A;
      // Log score event team A
      if(number_events < MAX_EVENTS){
        events[number_events][0] = ((int) match_time) + 5000 * current_period;
        events[number_events][1] = score_type_A + 1;
        events[number_events][2] = 1;
        number_events++;
      }
    }
    // Team B
    if (score_type_B < 4){
      pressed_message = score_type_B + 4;
      score_B[score_type_B] ++;
      score_B[4] += score_types[score_type_B];
      last_score_B = score_type_B;
      // Log score event team B
      if(number_events < MAX_EVENTS){
        events[number_events][0] = ((int) match_time) + 5000 * current_period;
        events[number_events][1] = score_type_B + 1;
        events[number_events][2] = 2;
        number_events++;
      }
    }
    update_scores();
    // reset score types
    score_type_A = 4;
    score_type_B = 4;
    // Move selection layer and show instructions
    text_layer_set_text_color(s_result, GColorBlack);
    text_layer_set_text_color(s_drops_A, GColorBlack);
    text_layer_set_text_color(s_convs_A, GColorBlack);
    text_layer_set_text_color(s_tries_A, GColorBlack);
    text_layer_set_text_color(s_pens_A, GColorBlack);
    text_layer_set_text_color(s_drops_B, GColorBlack);
    text_layer_set_text_color(s_convs_B, GColorBlack);
    text_layer_set_text_color(s_tries_B, GColorBlack);
    text_layer_set_text_color(s_pens_B, GColorBlack);
    text_layer_set_text_color(s_time_left, GColorWhite);
    text_layer_set_text_color(s_time_elapsed, GColorWhite);
    layer_set_frame((Layer *)s_selection_layer, GRect(0, 21, 144, 23));
    text_layer_set_text(s_instructions, "U = start/stop, D = note, Long U/D = New Half/Match");
    main_window_selection = 1;
  }
}
static void select_single_click_handler(ClickRecognizerRef recognizer, void *context) {
  //static int old_window_selection;
  //old_window_selection = main_window_selection;
  last_button_press = time(NULL);
  select_single_click_handler_do();
  //send_int(old_window_selection * 10 + 102, pressed_message);
}

// Reset GUI to timer selection
static void reset_GUI(){
  last_button_press = time(NULL);
  score_type_A = 4;
  score_type_B = 4;
  main_window_selection = 3;
  text_layer_set_text_color(s_injury_title, GColorBlack);
  text_layer_set_text_color(s_next_injury, GColorBlack);
  text_layer_set_text_color(s_card_title, GColorBlack);
  text_layer_set_text_color(s_next_card, GColorBlack);
  select_single_click_handler_do();
}

// Remove the latest card/injury event from history
static void remove_card_injury_event(int i_event_type){
  // Find latest card/injury event for this team
  static int index_to_remove = -1;
  for(int i = 0; i < number_events; i++){
    if(events[i][1] == i_event_type){
      index_to_remove = i;
    }
  }
  // "pop" the event
  if(index_to_remove > -1){
    number_events--;
    for(int i = index_to_remove; i < number_events; i++){
      for(int ii = 0; ii < 3; ii++) events[i][ii] = events[i + 1][ii];
    }
  }
}

// Remove the latest team score event from history
static void remove_score_event(int i_team){
  // Find latest score event for this team
  static int index_to_remove = -1;
  for(int i = 0; i < number_events; i++){
    if(events[i][1] > 0 && events[i][1] < 5 && events[i][2] == i_team){
      index_to_remove = i;
    }
  }
  // "pop" the event
  if(index_to_remove > -1){
    number_events--;
    for(int i = index_to_remove; i < number_events; i++){
      for(int ii = 0; ii < 3; ii++) events[i][ii] = events[i + 1][ii];
    }
  }
}

// Handle long click up
static void up_long_click_handler_do() {
  // Time selection
  pressed_message = match_time;
  if (main_window_selection == 1){
    // Reset period, prepare for next one
    vibes_long_pulse();
    Run = 0;
    period_notified = 0;
    tick_time_last_pause = 0;
    // Take remaining cards from previous period
    if(number_cards > 0){
      for(int i = 0; i < number_cards; i++){
        cards[i] = cards[i] - match_time;
      }
    }
    // Log end-of-period event
    if(number_events < MAX_EVENTS){
      events[number_events][0] = ((int) match_time) + 5000 * current_period;
      events[number_events][1] = 0;
      events[number_events][2] = 0;
      number_events++;
    }
    // Reset counters and GUI
    match_time = 0;
    remaining_time = match_duration;
    time_last_pause = 0;
    update_timers();
    current_period++;
  }
  // Cards selection
  if (main_window_selection == 2){
    vibes_short_pulse();
    // Undo last card and remove from history
    if(number_cards > 0){
      number_cards--;
      cards[number_cards] = 0;
      if(number_cards == 0){
        text_layer_set_text(s_next_card, "None");
      }
      remove_card_injury_event(5);
      text_layer_set_text(s_instructions, "Card removed");
    }
  }
  // Score selection
  if (main_window_selection == 3){
    vibes_short_pulse();
    // Undo last team A score and remove from history
    if(last_score_A < 4){
      score_A[last_score_A]--;
      score_A[4] -= score_types[last_score_A];
      last_score_A = 4;
      remove_score_event(1);
      update_scores();
    }
  }
}
static void up_long_click_handler(ClickRecognizerRef recognizer, void *context) {
  last_button_press = time(NULL);
  up_long_click_handler_do();
  //send_int(main_window_selection * 10 + 103, pressed_message);
}

// Handle long click down
static void down_long_click_handler_do() {
  pressed_message = match_time;
  // Time selection
  if (main_window_selection == 1){
    // Reset match, prepare for next one
    vibes_long_pulse();
    Run = 0;
    period_notified = 0;
    current_period = 0;
    // Reset all cards, injuries, and GUI
    for(int i = 0; i < 10; i++){
      cards[i] = 0;
      injuries[i] = 0;
    }
    number_cards = 0;
    number_injuries = 0;
    text_layer_set_text(s_next_card, "None");
    text_layer_set_text(s_next_injury, "None");
    // Reset scores and GUI
    for(int i = 0; i < 5; i++){
      score_A[i] = 0;
      score_B[i] = 0;
    }
    score_type_A = 4;
    score_type_B = 4;
    last_score_A = 4;
    last_score_B = 4;
    update_scores();
    // Reset history events
    for(int i = 0; i < number_events; i++){
      for(int ii = 0; ii < 3; ii++) events[i][ii] = 0;
    }
    number_events = 0;
    subs_a = 0;
    subs_b = 0;
    // Reset counters and GUI
    match_time = 0;
    remaining_time = match_duration;
    time_last_pause = 0;
    tick_time_last_pause = 0;
    update_timers();
    // Reset persistent data if existing
    // Current match time values
    if(persist_exists(NUM_LASTSTART_PKEY)) persist_delete(NUM_LASTSTART_PKEY);
    if(persist_exists(NUM_LASTPAUSE_PKEY)) persist_delete(NUM_LASTPAUSE_PKEY);
    if(persist_exists(NUM_TICKLASTPAUSE_PKEY)) persist_delete(NUM_TICKLASTPAUSE_PKEY);
    if(persist_exists(NUM_CLOCKRUNNING_PKEY)) persist_delete(NUM_CLOCKRUNNING_PKEY);
    if(persist_exists(NUM_MATCHTIME_PKEY)) persist_delete(NUM_MATCHTIME_PKEY);
    if(persist_exists(NUM_PERIODNOTIFIED_PKEY)) persist_delete(NUM_PERIODNOTIFIED_PKEY);
    if(persist_exists(NUM_CURRENTPERIOD_PKEY)) persist_delete(NUM_CURRENTPERIOD_PKEY);
    // Score values
    for(int i = 0; i < 5; i++){
      if(persist_exists(NUM_SCOREA_PKEY + i)) persist_delete(NUM_SCOREA_PKEY + i);
      if(persist_exists(NUM_SCOREB_PKEY + i)) persist_delete(NUM_SCOREB_PKEY + i);
    }
    if(persist_exists(NUM_SCORETYPEA_PKEY)) persist_delete(NUM_SCORETYPEA_PKEY);
    if(persist_exists(NUM_SCORETYPEB_PKEY)) persist_delete(NUM_SCORETYPEB_PKEY);
    if(persist_exists(NUM_LASTSCOREA_PKEY)) persist_delete(NUM_LASTSCOREA_PKEY);
    if(persist_exists(NUM_LASTSCOREB_PKEY)) persist_delete(NUM_LASTSCOREB_PKEY);
    // Cards values
    if(persist_exists(NUM_NUMBERCARDS_PKEY)) {
      persist_delete(NUM_NUMBERCARDS_PKEY);
      for(int i = 0; i < number_cards; i++){
        if(persist_exists(NUM_CARDS_PKEY + i)) persist_delete(NUM_CARDS_PKEY + i);
      }
    }
    // Injuries values
    if(persist_exists(NUM_NUMBERINJURIES_PKEY)) {
      number_injuries = persist_read_int(NUM_NUMBERINJURIES_PKEY);
      for(int i = 0; i < number_injuries; i++){
        if(persist_exists(NUM_INJURIES_PKEY + i)) injuries[i] = persist_read_int(NUM_INJURIES_PKEY + i);
      }
    }
    // History events values
    if(persist_exists(NUM_NUMBEREVENTS_PKEY)) persist_delete(NUM_NUMBEREVENTS_PKEY);
    for(int i = 0; i < number_events; i++) {
      for(int ii = 0; ii < 3; ii++) if(persist_exists(NUM_EVENTS_PKEY + 3 * i + ii)) persist_delete(NUM_EVENTS_PKEY + 3 * i + ii);
    }
    // Notes values
    if(persist_exists(NOTES_SIZE_PKEY)) persist_delete(NOTES_SIZE_PKEY);
    if(persist_exists(NOTES_PKEY)) persist_delete(NOTES_PKEY);
    // Subs values
    if(persist_exists(SUBS_A_PKEY)) persist_delete(SUBS_A_PKEY);
    if(persist_exists(SUBS_B_PKEY)) persist_delete(SUBS_B_PKEY);
  }
  // Cards selection
  if (main_window_selection == 2){
    vibes_short_pulse();
    // Undo last injury and remove from history
    if(number_injuries > 0){
      number_injuries--;
      injuries[number_injuries] = 0;
      if(number_injuries == 0){
        text_layer_set_text(s_next_injury, "None");
      }
       remove_card_injury_event(6);
      text_layer_set_text(s_instructions, "Injury removed");
    }
  }
  // Score selection
  if (main_window_selection == 3){
    vibes_short_pulse();
    // Undo last team B score and remove from history
    if(last_score_B < 4){
      score_B[last_score_B]--;
      score_B[4] -= score_types[last_score_B];
      last_score_B = 4;
      remove_score_event(2);
      update_scores();
    }
  }
}
static void down_long_click_handler(ClickRecognizerRef recognizer, void *context) {
  last_button_press = time(NULL);
  down_long_click_handler_do();
  //send_int(main_window_selection * 10 + 104, pressed_message);
}

// Handle long click select: switch to setting window
static void select_long_click_handler_do() {
  Run = 0;
  time_last_pause = match_time;
  if(match_time != 0){
    tick_time_last_pause = (long) time(NULL);
  }
  pressed_message = -match_time;
  vibes_short_pulse();
  next_expiring_timer = time(NULL) + find_next_expiring_timer();
  //window_stack_remove(m_window, true);
  show_s_settings();
}
static void select_long_click_handler(ClickRecognizerRef recognizer, void *context) {
  select_long_click_handler_do();
  //send_int(main_window_selection * 10 + 105, match_time);
}

// Function to register button listeners
static void config_provider(Window *window)
{
 // single click
  window_single_click_subscribe(BUTTON_ID_DOWN, down_single_click_handler);
  window_single_click_subscribe(BUTTON_ID_UP, up_single_click_handler);
  window_single_click_subscribe(BUTTON_ID_SELECT, select_single_click_handler);

  // long click
  window_long_click_subscribe(BUTTON_ID_DOWN, 1000, down_long_click_handler,NULL);
  window_long_click_subscribe(BUTTON_ID_UP, 1000, up_long_click_handler,NULL);
  window_long_click_subscribe(BUTTON_ID_SELECT, 1000, select_long_click_handler,NULL);
}

// Update current time and date
static void update_time() {
  // Get a tm structure
  time_t temp; 
  temp = time(NULL); 
  struct tm *tick_time;
  tick_time = localtime(&temp);

  // Create a long-lived buffer
  static char buffer_time[] = "00:00";
  static char buffer_date[] = "Sat 01-01-2015";

  // Write the current hours and minutes into the buffer
  if(clock_is_24h_style() == true) {
    // Use 24 hour format
    strftime(buffer_time, sizeof("00:00"), "%H:%M", tick_time);
  } else {
    // Use 12 hour format
    strftime(buffer_time, sizeof("00:00"), "%I:%M", tick_time);
  }
  strftime(buffer_date, sizeof("Sat 01-01-2015"), "%a %d-%m-%y", tick_time);
  // Display this time on the TextLayer
  text_layer_set_text(s_time, buffer_time);
  text_layer_set_text(s_date, buffer_date);
}

// Return to timer after a certain time
static void check_if_inactive(){
  if(current_time - last_button_press >= GUI_time_out){
    if(main_window_selection != 1){
      reset_GUI();
    }
  }
}

// Callback for seconds changed (main_loop)
static void tick_handler(struct tm *tick_time, TimeUnits units_changed){
  update_time();
  current_time = time(NULL);
  check_timers();
  update_timers();
  check_if_inactive();
}

// Key-press emulation from companion app - DEPRECATED
/*static void emulate_key_press(int command_key, int command_value){ 
  // Main window - Mode selection
  static int main_window_selection_old;
  main_window_selection_old = main_window_selection;
  if(command_key < 20){
    main_window_selection = command_key - 9;
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
    // Long up
    if(command_value == 3){
      up_long_click_handler_do();
    }
    // Long down
    if(command_value == 4){
      down_long_click_handler_do();
    }
    // Long select
    if(command_value == 5){
      select_long_click_handler_do();
    }
  } // Handle direct call to set start/pause no matter what the actual state is
  else if(command_key == 200){
    if(command_value == 1){
      main_window_selection = 1;
      Run = 0;
      up_single_click_handler_do();
    }
    if(command_value == 2){
      main_window_selection = 1;
      Run = 1;
      up_single_click_handler_do();
    }
  } else {
    // Settings window - wrapper function
    emulate_key_press_settings(command_key, command_value);
  }
  main_window_selection = main_window_selection_old;
}*/

// Handling callback for received data from phone app - DEPRECATED
/*static void inbox_received_callback(DictionaryIterator *iterator, void *context) {
  // Get the first pair
  static Tuple *t;
  t = dict_read_first(iterator);

  // Process all pairs present
  while (t != NULL) {*/
    /*
    Android shell syntax:
    am broadcast --user 0 -a com.getpebble.action.app.SEND -E UUID '333C6293-0FD9-4AFB-9E72-3EB0E74D3E6D' -E MSG_DATA '[{"key":int,"value":str,"length":int,"type":"string"}]'
    am broadcast --user 0 -a com.getpebble.action.app.SEND -E UUID '333C6293-0FD9-4AFB-9E72-3EB0E74D3E6D' -E MSG_DATA '[{"key":int,"value":int,"length":int,"type":"int"}]'
    */

    /*// Extract key and value
    static int command_key;
    command_key = (int) t->key;
    static int command_value;
    command_value = *t->value->data;
    emulate_key_press(command_key, command_value);
    // Get next pair, if any
    t = dict_read_next(iterator);
  }
}*/

// Handling callback for received data from phone app
static void inbox_received_callback(DictionaryIterator *iterator, void *context) {
  // Get the first pair
  Tuple *t;
  t = dict_read_first(iterator);

  // Process all pairs present
  while (t != NULL) {
    /*
    Android shell syntax:
    am broadcast --user 0 -a com.getpebble.action.app.SEND -E UUID '333C6293-0FD9-4AFB-9E72-3EB0E74D3E6D' -E MSG_DATA '[{"key":int,"value":str,"length":int,"type":"string"}]'
    am broadcast --user 0 -a com.getpebble.action.app.SEND -E UUID '333C6293-0FD9-4AFB-9E72-3EB0E74D3E6D' -E MSG_DATA '[{"key":int,"value":int,"length":int,"type":"int"}]'
    */

    // Extract key and value
    int command_key;
    command_key = (int) t->key;
    if(command_key < 10009){
      int command_value = *t->value->data;
      if(command_key < 10004){
        emulate_key_press_settings(command_key, command_value);
      } else {
        if(command_key == 10004){
          score_types[0] = command_value;
        }
        if(command_key == 10005){
          score_types[1] = command_value;
        }
        if(command_key == 10006){
          score_types[2] = command_value;
        }
        if(command_key == 10007){
          score_types[3] = command_value;
        }
        score_A[4] = score_A[0] * score_types[0] + score_A[1] * score_types[1] + score_A[2] * score_types[2] + score_A[3] * score_types[3];
        score_B[4] = score_B[0] * score_types[0] + score_B[1] * score_types[1] + score_B[2] * score_types[2] + score_B[3] * score_types[3];
      }
    } else {
      static char command_string[2];
      memcpy(command_string, t->value->cstring, t->length);
      if(command_key == 10009){
        subs_identifier[0] = command_string[0];
      }
      if(command_key == 10010){
        team_identifier[0] = command_string[0];
        team_identifier[1] = strchr(command_string, ',')[1];
        //static char temp_buffer[] = "Team A";
        //snprintf(temp_buffer, sizeof(temp_buffer), "Team %c", team_identifier[0]);
        //strcpy(affected[0], temp_buffer);
        //snprintf(temp_buffer, sizeof(temp_buffer), "Team %c", team_identifier[1]);
        //strcpy(affected[1], temp_buffer);
      }
      // re-compute number of subs with new parsing chars
      subs_a = 0;
      subs_b = 0;
      for(int i = 0; i < number_events; i++){
        if(events[i][1] >= 8){
          static char temp_buffer[12] = "            ";
          snprintf(temp_buffer, 13, "%.*s", events[i][2], notes + events[i][1] - 8);
          if (search_string(temp_buffer, 13, subs_identifier[0])){
            if (search_string(temp_buffer, 13, team_identifier[0])){
              subs_a += 1;
            }
            if (search_string(temp_buffer, 13, team_identifier[1])){
              subs_b += 1;
            }
          }          
        }
      }
    }
    //APP_LOG(APP_LOG_LEVEL_ERROR, "Parsing dictionary %d %d", command_key, command_value);
    // Get next pair, if any
    t = dict_read_next(iterator);
  }
  write_persist();
  //ReadSettings();
  current_time = time(NULL);
  remaining_time = match_duration - match_time;
  check_timers();
  update_timers();
  update_scores();
  // Force timer selection
  if(main_window_selection != 1){
    reset_GUI();
  }
}

static void inbox_dropped_callback(AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Message dropped!");
}

// Reload window back from other window
static void handle_window_appear(Window* window) {
  ReadSettings();
  //send_int(0, 0);
  //send_str(100, "init");
  // (re)-create UI and reset ui timer
  initialise_ui();
  last_button_press = time(NULL);
  // Register time and click services
  tick_timer_service_subscribe(SECOND_UNIT, tick_handler);
  window_set_click_config_provider(m_window, (ClickConfigProvider) config_provider);
  // Update timers and scores
  current_time = time(NULL);
  remaining_time = match_duration - match_time;
  check_timers();
  update_timers();
  update_scores();
  // Force timer selection
  if(main_window_selection != 1){
    reset_GUI();
  }
}

static void handle_window_load(Window* window) {
  // Upon loading also read settings
  //ReadSettings();
}

// Initialize app
void init()
{
  //APP_LOG(APP_LOG_LEVEL_DEBUG, "Start main, Free memory: %d", heap_bytes_free());
  // Register inbox callbacks - DEPRECATED
  app_message_register_inbox_received(inbox_received_callback);
  app_message_register_inbox_dropped(inbox_dropped_callback);

    // Register outbox callbacks
  app_message_register_outbox_failed(outbox_failed_callback);
  app_message_register_outbox_sent(outbox_sent_callback);

  // Open AppMessage
  //app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
  //const uint32_t inbox_size = 256;
  //const uint32_t outbox_size = 19 * MAX_EVENTS + 1;
  //app_message_open(inbox_size, outbox_size);
  app_message_open(312, 111); // < (5 * 7) + (5 * 13) + (7 + 3) + 1; batch of 5 events, maximal content 5 notes (max 13b each) + end event
  
  // Create, set, and push main Window element
  m_window = window_create();
  window_set_window_handlers(m_window, (WindowHandlers) {
    .load = handle_window_load,
    .appear = handle_window_appear,
    .disappear = handle_window_disappear,
    .unload = handle_window_unload,
  });
  window_stack_push(m_window, true); 
  //APP_LOG(APP_LOG_LEVEL_DEBUG, "Main started, Free memory: %d", heap_bytes_free());
}

// deinitialize app
void deinit(){
  write_persist();
  app_message_deregister_callbacks();
  window_destroy(m_window); 
}