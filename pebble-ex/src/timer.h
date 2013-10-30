//
//  timer.h
//  pebble-ex
//
//  Created by Paul Bates on 10/13/13.
//  Copyright (c) 2013 Sonos. All rights reserved.
//

#ifndef __PEBBLE_EX_TIMER_H__
#define __PEBBLE_EX_TIMER_H__

#include "pebble_os.h"

#define TIMER_HANDLER_COOKIE 0x7183 // Best way TIME could be represented with HEX
#define TIMER_APP_TIMER_HANDLER (PebbleAppTimerHandler)timer_handler

// Callback for timer when the timer fires.
typedef void (*TimerCallback)(AppContextRef app_ctx, void *ctx);


// Single timer representation, can be used multiple times but only scheduled once per fire.
// Attempts to schedule the same timer will reschedule the timer and will be fired at the timeout
// from the last schedule. After a timer has expired it may also be rescheduled again. If repeating
// behavior is desired, instead of rescheduling an expired timer set it to repeat.
//
// Optional you can specify a timer context, passed to the timer fire handler. You may use a Layer*
// or any other long lived data.
//
// In order to use Timers in your app you will need to register the TIMER_APP_TIMER_HANDLER with
// your application's event loop PebbleAppHandlers. Set timer_handler = TIMER_APP_TIMER_HANDLER.
typedef struct {
    uint32_t interval_ms;
    bool repeat : 1;
    bool scheduled : 1;
    AppTimerHandle handle;
    void *context;
    TimerCallback callback;
} Timer;


// Initializes a timer with a millisecond timeout
void timer_init(Timer *timer, TimerCallback callback, uint32_t timeout_ms);

// Adjusts the timeout dictating when a timer fires.
// Adjusting this on an active timer will have no affect unless it's a repeating timer, which will
// then the new timeout will be used for the next repeat timer.
void timer_set_timeout(Timer *timer, uint32_t timeout_ms);
// Sets repeating behavior of a timer.
void timer_set_repeat(Timer *timer, bool repeat);
// Sets a timer's context data, which may be a Layer * or some other data. The context will be
// passed to the timer's callback function.
void timer_set_context(Timer *timer, void *context);

// Schedules/reschedules an active or expired timer
void timer_schedule(Timer *timer, AppContextRef app_ctx);
// Cancels an active timer, causing it not to fire. The timer may be rescheduled later if desired.
void timer_cancel(Timer *timer, AppContextRef app_ctx);

// AppTimerHandler to be use when initalizing the pebble app event loop.
// Use TIMER_APP_TIMER_HANDLER for convenience
void timer_handler(AppContextRef app_ctx, AppTimerHandle handle, uint32_t cookie);

#endif // __PEBBLE_EX_TIMER_H__
