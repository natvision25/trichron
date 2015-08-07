#include <pebble.h>

/* _____ ____  ___ ____ _   _ ____   ___  _   _ 
  |_   _|  _ \|_ _/ ___| | | |  _ \ / _ \| \ | |
    | | | |_) || | |   | |_| | |_) | | | |  \| |
    | | |  _ < | | |___|  _  |  _ <| |_| | |\  |
    |_| |_| \_\___\____|_| |_|_| \_\\___/|_| \_|
                                  ~~By Natvision
 Anyone can take, use, modify, and distribute 
  this watchface for free, as long as they don't
  take the credit. Enjoy :) 
  
  Also i'm 14 so don't expect sheer excellence*/
  
  //EDIT: 8/05/15 - Fixed 12 am and pm displaying as 0
  
Window *window;
TextLayer *text_layer;
Layer *canvas;
InverterLayer *inverter;
unsigned long timer = 0;

int unix; //variable to hold the unix time
int lasthour;
int lastminute;
int lastsecond;
int hour; //variable for the hours
int minute; //something to hold the minutes
int second; //cant forget the seconds :)

void update_time(); //Pre-define update_time so other functions don't freak out

void draw(Layer *this_layer, GContext *ctx){
  GPoint center = GPoint(72, 84); //This is obviously the center point
  GPoint hourpoint = GPoint(72, 84 - (hour*5)); //Calculate the hour point
  GPoint minutepoint = GPoint(72 + minute, 84 + minute); //Calculate the minute point
  GPoint secondpoint = GPoint(72 - second, 84 + second); //Calculate the second point

  //make the drawing colors black
  graphics_context_set_stroke_color(ctx, GColorBlack); 
  graphics_context_set_fill_color(ctx, GColorBlack);
  
  //draw the triangle
  graphics_draw_line(ctx, hourpoint, minutepoint);
  graphics_draw_line(ctx, minutepoint, secondpoint);
  graphics_draw_line(ctx, secondpoint, hourpoint);
  graphics_draw_line(ctx, center, minutepoint);
  graphics_draw_line(ctx, center, secondpoint);
  graphics_draw_line(ctx, center, hourpoint);
  
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time(); //update the time and the triangle
}

void update_time(){
  time_t temp = time(NULL);  //store the unix time in temp
  unix = temp; //unix is now temp
  //save the last units for animation
  lasthour = hour;
  lastminute = minute;
  lastsecond = second;
  
  //calculate the current time
  hour = unix/3600%12;
  minute = unix%3600/60;
  second = unix%60;
  
  
  static char buffer[64]; //here's the buffer for the time text

    if(hour == 0 && minute < 10)snprintf(buffer, 64, "12:0%d", minute); //combine the next two if statements
    else  if(hour == 0)snprintf(buffer, 64, "12:%d", minute); //display 12 instead of 0 when it's noon or midnight, so 12:16 won't be 0:16
    else  if(minute < 10)snprintf(buffer, 64, "%d:0%d", hour, minute); //add a zero if the minute is less than 10. 4:6 would be wierd.
    else snprintf(buffer, 64, "%d:%d", hour, minute); //just print the plain time if none of those happen.
  
  
  
    text_layer_set_text(text_layer, buffer); //update the text
  layer_mark_dirty(canvas); //update the triangle
}

void handle_init(void) {
	// Create a window and text layer
	window = window_create();
	text_layer = text_layer_create(GRect(0, 48, 144, 48));
  canvas = layer_create(GRect(0,0,144,168));
  
	
	// Set the text, font, and text alignment
	text_layer_set_text(text_layer, "lolfail"); //just in case it don't work :P
  GFont spire = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_SPIRE_48));
	text_layer_set_font(text_layer, spire);
	text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
	
	
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(text_layer)); // Add the text layer to the window
  layer_add_child(window_get_root_layer(window), canvas); //do the same for the canvas
  
	window_stack_push(window, true); //push the window
  tick_timer_service_subscribe(SECOND_UNIT, tick_handler); // make sure it shows the right time all the time
  layer_set_update_proc(canvas, draw); //set the function to draw into the canvas layer
	update_time(); //make sure the time is shown from the start
}

void handle_deinit(void) {
	// Destroy the layers
  layer_destroy(canvas);
	text_layer_destroy(text_layer);
	
	// Destroy the window
	window_destroy(window);
}

int main(void) {
	handle_init();
	app_event_loop();
	handle_deinit();
}
