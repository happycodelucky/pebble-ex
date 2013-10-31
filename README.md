# pebble-ex #

Extensions to PebbleSDK to provide missing support and extended user interface elements.

## Timers ##
Timer based eventing is available in the [PebbleSDK](https://github.com/pebble/pebblekit) but there is only a single timer callback function and your app needs to utilized timer cookies to know which scheduled timer fired. It would be much better if apps were able to schedule timers with a timeout period and have a handler method for each scheduled timer. Timer ([timer.h](/pebble-ex/src/timer.h)) provides this facility, to a limited extent, making working with custom timers far easier.

Timer supports two additional features, one being automatic rescheduling using repeating. A timer context is optionally supported  and passed to the timer hanlder callback function. Contexts allow for all sorts of enhancements, some of which are custom animation timing support. In fact **ActivityLayer** utilizes **Timer** to perform it's animations.

### Configuring Timers ###
Timers are utilize the .timer_handler function pointer in **PebbleAppHandlers**. In order for Timers to work your watch app must set timer_handler to **TIMER_APP_TIMER_HANDLER**.

	#include "pebble_ex.h"
	
	void pbl_main(AppTaskContextRef ctx) {
		PebbleAppHandlers handlers = {
			.init_handler = &handle_init,
			...
			.timer_handler = TIMER_APP_TIMER_HANDLER,
		};
	
		app_event_loop(ctx, &handlers);
	}

If you must, you can always implement your own **PebbleAppTimerHandler** and call timer_handler() when you do not recognize the timer handler cookie. 

### Using Timers ###

	static Layer custom_layer;
	static Timer repeating_timer;
	static bool stop_flipping = no;
	
	// Forward declaration
	void repeating_timer_callback(AppContextRef app_ctx, void *ctx);
	
	// Main appplication initialization
	void handle_init(AppContextRef ctx) {
		...
		// Initialized a Timer using a timer hander callback function.
		// The 10 represents 10 milliseconds until the timer fires
		timer_init(&repeating_timer, repeating_timer_callback, 10);
		
		// Designate the timer to reschedule itself automatically, creating
		// a repeating timer.
		timer_set_repeat(&repeating_timer, true);
		
		// Finally, schedule the timer
		timer_schedule(&repeating_timer, &custom_layer);
		...
	}
	
	// Timer handler as reference in handle_init()
	void repeating_timer_callback(AppContextRef app_ctx, void *ctx) {
		if (stop_flipping) {
			// Cancel the timer, preventing any more repeats scheduling.
			// This is only required to be called when the timer has not
			// fired or when the timer is a repeat timer.
			timer_cancel(&repeating_timer, app_ctx);
		}
		else {
			static bool flipped = false;
			
			// Use the layer context parameter, set when initializing the timer
			Layer *layer = (Layer *)ctx;
			layer_set_hidden(layer, flipped);
			
			flipped = !flipped;
		}
	}
