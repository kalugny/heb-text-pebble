#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"
#include <string.h>

#define MY_UUID { 0xE6, 0x87, 0x3A, 0x7D, 0xDD, 0x3E, 0x4F, 0xD3, 0x80, 0xD4, 0xD6, 0xFB, 0xB6, 0xD7, 0xD4, 0x01 }
PBL_APP_INFO(MY_UUID,
             "Hebrew Text", "Yuval Kalugny",
             1, 0, /* App version */
             DEFAULT_MENU_ICON,
             APP_INFO_WATCH_FACE);


static const char * const MINUTES[] = {
    "הקדו",
    "יתשו\nתוקד",
    "שולשו\nתוקד",
    "עבראו\nתוקד",
    "השימחו",
    "ששו\nתוקד",
    "עבשו\nתוקד",
    "הנומשו\nתוקד",
    "עשתו\nתוקד",
    "הרשעו",
    "תחאו\nהרשע\nתוקד",
    "םייתשו\nהרשע\nתוקד",
    "שולשו\nהרשע\nתוקד",
    "עבראו\nהרשע\nתוקד",
    "עברו",
    "ששו\nהרשע\nתוקד",
    "עבשו\nהרשע\nתוקד",
    "הנומשו\nהרשע\nתוקד",
    "עשתו\nהרשע\nתוקד",
    "םירשעו",
    "םירשע\nתחאו",
    "םירשע\nםייתשו",
    "םירשע\nשולשו",
    "םירשע\nעבראו",
    "םירשע\nשמחו",
    "םירשע\nששו",
    "םירשע\nעבשו",
    "םירשע\nהנומשו",
    "םירשע\nעשתו",
    "יצחו",
    "םישולש\nתחאו",
    "םישולש\nםייתשו",
    "םישולש\nשולשו",
    "םישולש\nעבראו",
    "םישולש\nשמחו",
    "םישולש\nששו",
    "םישולש\nעבשו",
    "םישולש\nהנומשו",
    "םישולש\nעשתו",
    "םירשע",
    "םיעברא\nתחאו",
    "םיעברא\nםייתשו",
    "םיעברא\nשולשו",
    "םיעברא\nעבראו",
    "עבר",
    "םיעברא\nששו",
    "םיעברא\nעבשו",
    "םיעברא\nהנומשו",
    "םיעברא\nעשתו",
    "הרשע",
    "םישימח\nתחאו",
    "םישימח\nםייתשו",
    "םישימח\nשולשו",
    "םישימח\nעבראו",
    "השימח",
    "עברא\nתוקד",
    "שולש\nתוקד",
    "יתש\nתוקד",
    "הקד"
};

static const char * const HOURS[] = {
    "םייתש\nהרשע",
    "תחא",
    "םייתש",
    "שולש",
    "עברא",
    "שמח",
    "שש",
    "עבש",
    "הנומש",
    "עשת",
    "רשע",
    "תחא\nהרשע"
};

static const char * const HOURS_WITH_TO[] = {
    "םייתשל\nהרשע",
    "תחאל",
    "םייתשל",
    "שולשל",
    "עבראל",
    "שמחל",
    "ששל",
    "עבשל",
    "הנומשל",
    "עשתל",
    "רשעל",
    "תחאל\nהרשע"
};

Window window;
TextLayer timeLayer;

GFont font42;
GFont font36;
GFont font28;


void get_heb_desc_from_time(PblTm* t, char* timeText){

    int hour = t->tm_hour;
    int minutes = t->tm_min;
    int next_hour = hour + 1;
    
    if (hour > 11) {
        hour -= 12;
    }
    
    if (hour == 11){
        next_hour = 0;
    } else {
        next_hour = hour + 1;
    }
    
    if (minutes == 0){
        strcpy(timeText, HOURS[hour]);
        return;
    }
    
    if (minutes == 40 || minutes == 45 || minutes == 50 || minutes >= 55){
        strcat(timeText, MINUTES[minutes - 1]);
        strcat(timeText, "\n");
        strcat(timeText, HOURS_WITH_TO[next_hour]);
        return;
    }
    
    strcat(timeText, HOURS[hour]);
    strcat(timeText, "\n");
    strcat(timeText, MINUTES[minutes - 1]);
    
    return;

}

void handle_minute_tick(AppContextRef ctx, PebbleTickEvent *t) {
    
    (void)t;
    (void)ctx;
    
    static char timeText[100] = {0}; // Needs to be static because it's used by the system later.
    
    PblTm currentTime;
    
    get_time(&currentTime);

    strcpy(timeText, "");
    //strcat(timeText, HOURS[hour]);
    //strcat(timeText, " ");
    //strcat(timeText, MINUTES[minutes - 1]);
    
    get_heb_desc_from_time(&currentTime, timeText);
    
    int textLen = strlen(timeText);
    
    if (textLen < 20){
        text_layer_set_font(&timeLayer, font42);
    }
    else if (textLen < 40){
        text_layer_set_font(&timeLayer, font36);
    }
    else {
        text_layer_set_font(&timeLayer, font28);
    }
    
    text_layer_set_text(&timeLayer, timeText);
    
}

void handle_init(AppContextRef ctx) {
    (void)ctx;

    window_init(&window, "Hebrew Text");
    window_stack_push(&window, true /* Animated */);
    
    window_set_background_color(&window, GColorBlack);
    
    // If you neglect to call this, all `resource_get_handle()` requests
    // will return NULL.
    resource_init_current_app(&RESOURCES);
    
    font42 = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_SIMPLE_42));
    font36 = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_SIMPLE_36));
    font28 = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_SIMPLE_28));

    
    text_layer_init(&timeLayer, GRect(0, 0, window.layer.frame.size.w, window.layer.frame.size.h));
    text_layer_set_text_color(&timeLayer, GColorWhite);
    text_layer_set_text_alignment(&timeLayer, GTextAlignmentRight);
    text_layer_set_background_color(&timeLayer, GColorClear);
    
    handle_minute_tick(ctx, NULL);
    
    layer_add_child(&window.layer, &timeLayer.layer);
}

void pbl_main(void *params) {
  PebbleAppHandlers handlers = {
      .init_handler = &handle_init,
      
    .tick_info = {
      .tick_handler = &handle_minute_tick,
      .tick_units = MINUTE_UNIT
     }
  };
  app_event_loop(params, &handlers);
}
