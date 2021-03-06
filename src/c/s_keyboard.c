#include "s_keyboard.h"
 
// Max text limit in characters
// You may adjust this to allow longer messages
#define MAX_CHARS 13

#define TOP 0
#define MID 1
#define BOT 2

Window* window;

TextLayer* text_title;
TextLayer* text_layer;
TextLayer* text_input;

TextLayer* buttons1[3];
TextLayer* buttons2[3];
TextLayer* buttons3[3];
TextLayer** bbuttons[3];

//InverterLayer* inverter_side;
BitmapLayer* background_side;

bool menu = false;

// Here are the three cases, or sets
char caps[] =    	"ABCDEFGHIJKLM NOPQRSTUVWXYZ";
char letters[] = 	"abcdefghijklm nopqrstuvwxyz";
char numsym[] = 		"1234567890!?-'\"$()&*+#:@/,.";

// the below three strings just have to be unique, abc - xyz will be overwritten with the long strings above
char* btext1[] = {"abc", "def", "ghi"};
char* btext2[] = {"jkl", "m n", "opq"};
char* btext3[] = {"rst", "uvw", "xyz"};
char** btexts[] = {btext1, btext2, btext3};

// These are the actual sets that are displayed on each button, also need to be unique
char set1[4] = "  a";
char set2[4] = "  b";
char set3[4] = "  c";
char* setlist[] = {set1, set2, set3};

char* cases[] = {"CAP", "low", "#@1"};

//atic int cur_set = 1;
int cur_set = 0;

void drawSides();
void drawMenu();
void set_menu();
void drawNotepadText();

char* rotate_text[] = {caps, letters, numsym};
void next();

char* master = letters;

char text_buffer[MAX_CHARS];
int pos = 0;
int top, end, size;

const char* title;
void* extra;
TertiaryTextCallback callback;

const bool animated = true;

// This function changes the next case/symbol set.
void change_set(int s, bool lock)
{
    int count = 0;
    master = rotate_text[s];
    for (int i=0; i<3; i++)
    {
        for (int j=0; j<3; j++)
        {
            for (int k=0; k<3; k++)
            {
                btexts[i][j][k] = master[count];
                count++;
            }
        }
    }
    
    menu = false;
    if (lock) cur_set = s;
    
    drawSides();
}

void next()
{
    top = 0;
    end = 26;
    size = 27;
}

void clickButton(int b)
{
	if (menu)
	{
		change_set(b, false);
		return;
	}

	if (size > 3)
	{
		size /= 3;
		if (b == TOP)
			end -= 2*size;
		else if (b == MID)
		{
			top += size;
			end -= size;
		}
		else if (b == BOT)
			top += 2*size;
	}
	else
	{
    // Prevent overflowing
    if (pos == MAX_CHARS-1)
      pos--;
    
    // Type the character
		text_buffer[pos++] = master[top+b];
		drawNotepadText();
		change_set(cur_set, false);
		next();
	}

	drawSides();    
}

static void up_single_click_handler(ClickRecognizerRef recognizer, void* context)
{
    clickButton(TOP);
}

static void select_single_click_handler(ClickRecognizerRef recognizer, void* context)
{
    clickButton(MID);
}

static void down_single_click_handler(ClickRecognizerRef recognizer, void* context)
{
    clickButton(BOT);
}

bool common_long(int b)
{
    if (menu)
    {
        change_set(b, true);
        return true;
    }
    return false;
}

static void up_long_click_handler(ClickRecognizerRef recognizer, void* context)
{
    if (common_long(TOP)) return;
    
    set_menu();
}

static void select_long_click_handler(ClickRecognizerRef recognizer, void* context)
{   
  if (common_long(MID)) return;
  
  // Close this window
  window_stack_pop( animated );
    
  // Call user supplied callback with text_buffer
  callback( text_buffer, strlen( text_buffer ), extra ); 

}


static void down_long_click_handler(ClickRecognizerRef recognizer, void* context) {
    
    if (common_long(BOT)) return;
    
    // delete or cancel when back is held
    if (size==27 && pos>0 )
    {
        text_buffer[--pos] = ' ';
        drawNotepadText();
    }
    else
    {
        next();
        drawSides();
    }
    
}

void set_menu()
{
	menu = true;
	drawMenu();
}

void click_config_provider(void* context)
{
    window_single_click_subscribe(BUTTON_ID_UP, up_single_click_handler);
    window_long_click_subscribe(BUTTON_ID_UP, 1000, up_long_click_handler, NULL);
    
    window_single_click_subscribe(BUTTON_ID_SELECT, select_single_click_handler);
    window_long_click_subscribe(BUTTON_ID_SELECT, 1000, select_long_click_handler, NULL);
    
    window_single_click_subscribe(BUTTON_ID_DOWN, down_single_click_handler);
    window_long_click_subscribe(BUTTON_ID_DOWN, 1000, down_long_click_handler, NULL);
}

void drawMenu()
{
    for (int i=0; i<3; i++)
    {
        text_layer_set_text(bbuttons[i][i!=2], " ");
        text_layer_set_text(bbuttons[i][2], " ");
        
        text_layer_set_text(bbuttons[i][i==2], cases[i]);
        text_layer_set_font(bbuttons[i][0], fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
    }
}


// This method draws the characters on the right side near the buttons
void drawSides()
{
    if (size==27) // first click (full size)
    {
        // update all 9 labels to their proper values
        for (int h=0; h<3; h++)
        {
            for (int i=0; i<3; i++)
            {
                text_layer_set_text(bbuttons[h][i], btexts[h][i]);
                text_layer_set_background_color(bbuttons[h][i], GColorClear);
                text_layer_set_font(bbuttons[h][i], fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));
            }
            
        }
    }
    else if (size==9)   // second click
    {
        for (int i=0; i<3; i++)
        {
            text_layer_set_text(bbuttons[i][i!=2], " ");
            text_layer_set_text(bbuttons[i][2], " ");
            
            text_layer_set_text(bbuttons[i][i==2], btexts[top/9][i]);
            text_layer_set_font(bbuttons[i][i==2], fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
        }
        
    }
		else if (size == 3)
    {
        for (int i=0; i<3; i++)
        {
            setlist[i][2] = master[top+i];
            text_layer_set_text(bbuttons[i][i==2], setlist[i]);        
        }
    }
}

void clearText()
{
    for (int i=0; i<MAX_CHARS; i++)
        text_buffer[i] = '\0';
    pos = 0;
}

void initSidesAndText()
{
    // Make sure the text is cleared on the first launch
    clearText();
  
		// Retrieve the window layer and its bounds
    Layer *window_layer = window_get_root_layer(window); 
		GRect bounds = layer_get_bounds(window_layer);
	
		// Create a text layer for the text that is typed
    text_layer = text_layer_create((GRect) { .origin = { 0, 72 }, .size = { bounds.size.w, 20 } });
    text_layer_set_font(text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
    layer_add_child(window_layer, text_layer_get_layer(text_layer));
    
		// Create a text layer for the title
		text_title = text_layer_create( GRect( 5, 5, 100, 30 ) );
		text_layer_set_font( text_title, fonts_get_system_font( FONT_KEY_GOTHIC_14_BOLD ) );
		text_layer_set_text( text_title, title );
		layer_add_child( window_layer, text_layer_get_layer( text_title ) );
		
    text_input = text_layer_create((GRect) { .origin = { 10, 40 }, .size = { 100, 150 } });

    text_layer_set_background_color(text_input, GColorClear);
    text_layer_set_font(text_input, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
    layer_add_child(window_layer, text_layer_get_layer(text_input));

	background_side = bitmap_layer_create( GRect( 110, 0, 34, 169 ) );
	bitmap_layer_set_background_color(background_side, GColorBlack);
	layer_add_child( window_layer, bitmap_layer_get_layer( background_side ) );
    
    for (int i = 0; i<3; i++)
    {
        buttons1[i] = text_layer_create((GRect) { .origin = { 115, 12*i }, .size = { 100, 100 } });
        buttons2[i] = text_layer_create((GRect) { .origin = { 115, 12*i+50 }, .size = { 100, 100 } });
        buttons3[i] = text_layer_create((GRect) { .origin = { 115, 12*i+100 }, .size = { 100, 100 } });
    }

    for( int i=0; i<3; i++ )
        for( int j=0; j<3; j++ ){
      text_layer_set_text_color(bbuttons[i][j], GColorWhite);
      text_layer_set_background_color(bbuttons[i][j], GColorClear);
      layer_add_child( window_layer, text_layer_get_layer( bbuttons[i][j] ) );
    }
    
		// Side inverter
		//inverter_side = inverter_layer_create( GRect( 110, 0, 34, 169 ) );
		//layer_add_child( window_layer, inverter_layer_get_layer( inverter_side ) );
}

void drawNotepadText()
{
    text_layer_set_text(text_input, text_buffer);
}

void window_unload(Window *window)
{
  text_layer_destroy(text_layer);
	text_layer_destroy(text_input);
	text_layer_destroy(text_title);
	
	//inverter_layer_destroy( inverter_side );
	bitmap_layer_destroy( background_side );

    for( int i=0; i<3; i++ )
        for( int j=0; j<3; j++ )
			text_layer_destroy( bbuttons[ i ][ j ] );
  
  window_destroy(window);
  //APP_LOG(APP_LOG_LEVEL_DEBUG, "Free memory: %d", heap_bytes_free());
}

void window_load(Window* window)
{
    initSidesAndText();
    //drawSides();
    drawNotepadText();
}

void tertiary_text_prompt( const char* _title, TertiaryTextCallback _callback, void* _extra )
{
  //APP_LOG(APP_LOG_LEVEL_DEBUG, "Start keyboard, Free memory: %d", heap_bytes_free());
	// Store the arguments for later
	title = _title;
	extra = _extra;
	callback = _callback;

	// Setup the button arrays
    bbuttons[0] = buttons1;
    bbuttons[1] = buttons2;
    bbuttons[2] = buttons3;

		// Create and configure the window
    window = window_create();
    window_set_click_config_provider(window, click_config_provider);

    window_set_window_handlers(window, (WindowHandlers) {
        .load = window_load,
        .unload = window_unload,
    });

		// Initiate the character position and update the text layers
    next();

	// Push the window onto the stack
    window_stack_push(window, animated);
    
		// Default to lowercase letters
    //change_set(1, true);
		// Default to uppercase letters
    change_set(0, true);
  //APP_LOG(APP_LOG_LEVEL_DEBUG, "Keyboard started, Free memory: %d", heap_bytes_free());
}