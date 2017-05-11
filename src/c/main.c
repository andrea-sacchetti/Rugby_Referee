#include <pebble.h>
#include "main.h"
#include "s_main_window.h"


// Main method
int main(void)
{
  init();
  app_event_loop();
  deinit();
}

/* 

INTERFACE TO COMPANION PHONE APP *** DEPRECATED: NOT USED BY ANYBODY AND MEMORY HUNGRY ***

Commands watch ==> phone:

  ijk, n ==> i = window, j = selection, k = button (0 = up, 1 = down, 2 = select, 3 = long-up, 4 = long-down, 5 = long-select), n = message
  
  Messages:
  
    i = 1, n = match time seconds
    ijk = 100, n = match time seconds (n < 0 ==> paused, n > 0 ==> running)
    ijk = 112, n = score type (0-3 = T/C/P/D A, 4-8 = T/C/P/D A)
    
    i = 2
      k = 0, 1
        j = 0, n = match duration seconds
        j = 1, n = card duration seconds
        j = 2, n = injury duration seconds
      k = 2
        n = 0, no message
  

Commands phone ==> watch:

  ij, k ==> i = window, j = selection - 1, k = button (0 = up, 1 = down, 2 = select, 3 = long-up, 4 = long-down, 5 = long-select)
  
  Special commands:
  
    101, n ==> set match duration to n
    102, n ==> set card duration to n
    103, n ==> set injury duration to n

    200, 1 ==> start clock
    200, 2 ==> stop watch

*/