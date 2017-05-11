#define MAX_NOTES_LENGTH 200
#define MAX_EVENTS 50
#define NUM_MATCHLENGTH_PKEY 4
#define NUM_CARDLENGTH_PKEY 5
#define NUM_INJURYLENGTH_PKEY 6
#include <pebble.h>

void show_s_settings(void);
void hide_s_settings(void);
void emulate_key_press_settings(int command_key, int command_value);
//void send_int(int key, int value);
//void send_str(int key, const char* value);
//void send_block(int key[10], int value[10], int n_entries);
//void sync_durations(long m, long c, long i);
void outbox_sent_callback(DictionaryIterator *iter, void *context);
void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context);

long match_duration;
long card_duration;
long injury_duration;

int number_events;
int events[MAX_EVENTS][3];
//char event_types[7][10];
char team_identifier[2];

int number_notes;
char notes[MAX_NOTES_LENGTH];
int notes_size;

int subs_a;
int subs_b;

long next_expiring_timer;
Window *m_window;
int current_period;