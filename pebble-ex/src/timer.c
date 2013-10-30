//
//  timer.c
//  pebble-ex
//
//  Created by Paul Bates on 10/13/13.
//  Copyright (c) 2013 Sonos. All rights reserved.
//

#include "timer.h"

// Maximum number of timers that can be scheduled by an app
#define TIMER_MAX_TIMERS 4

// Tracks scheduled timers
static Timer* timer_timers[TIMER_MAX_TIMERS];
static short timer_active_timers = 0;

/*******************************************************************************
 Timers
 ******************************************************************************/

#ifdef __clang__
#pragma mark - Initialization
#endif

void timer_init(Timer *timer, TimerCallback callback, uint32_t timeout_ms)
{
    if (!timer)
        return;
    
    timer->repeat = false;
    timer->scheduled = false;
    timer->callback = callback;
    timer->context = NULL;
    timer->handle = 0;

    timer_set_timeout(timer, timeout_ms);
}


#ifdef __clang__
#pragma mark - Publc properties
#endif

void timer_set_timeout(Timer *timer, uint32_t timeout_ms)
{
    if (!timer)
        return;

    // Setting a timer with a 0 timeout will cause Pebble OS to crash
    if (timeout_ms == 0) {
        APP_LOG(APP_LOG_LEVEL_ERROR, "Cannot set Timer timeout to 0");
        timeout_ms = 1;
    }

    timer->interval_ms = timeout_ms;
}

void timer_set_repeat(Timer *timer, bool repeat)
{
    if (!timer)
        return;

    timer->repeat = repeat;
}

void timer_set_context(Timer *timer, void *context)
{
    if (!timer)
        return;

    timer->context = context;
}


#ifdef __clang__
#pragma mark - Public functions
#endif

void timer_schedule(Timer *timer, AppContextRef app_ctx)
{
    if (!timer)
        return;

    // Check if the timer is already in the scheduled timers and cancel if so.
    if (timer->scheduled) {
        for (short i = 0; i < timer_active_timers; i++) {
            if (timer_timers[i] == timer) {
                timer_cancel(timer, app_ctx);
                break;
            }
        }
    }

    if (timer_active_timers + 1 >= TIMER_MAX_TIMERS) {
        APP_LOG(APP_LOG_LEVEL_ERROR, "Unable to schedule timer. Max allowed %d", TIMER_MAX_TIMERS);
        return;
    }

    // Schedule the timer
    timer->scheduled = true;
    timer_timers[timer_active_timers] = timer;
    timer_timers[timer_active_timers]->handle = app_timer_send_event(app_ctx, timer->interval_ms, TIMER_HANDLER_COOKIE);
    ++timer_active_timers;
}

void timer_cancel(Timer *timer, AppContextRef app_ctx)
{
    if (!timer)
        return;

    // Check if the timer is active
    for (short i = 0; i < timer_active_timers; i++) {
        if (timer_timers[i] == timer) {
            // Cancel the timer
            app_timer_cancel_event(app_ctx, timer_timers[i]->handle);
            timer_timers[i]->handle = 0;
            timer_timers[i]->scheduled = false;

            // Coalesce the remainig timers
            for (short j = i + 1; j < timer_active_timers; j++) {
                timer_timers[i - 1] = timer_timers[i];
            }
            --timer_active_timers;

            break;
        }
    }
}


#ifdef __clang__
#pragma mark - Callbacks
#endif

void timer_handler(AppContextRef app_ctx, AppTimerHandle handle, uint32_t cookie)
{
    if (cookie != TIMER_HANDLER_COOKIE)
        return;

    for (short i = 0; i < timer_active_timers; i++) {
        Timer *timer = timer_timers[i];
        if (timer->handle == handle) {
            // Call the timer callback first, and give the callback a chance to alter the current
            // state of the timer.
            timer->callback(app_ctx, timer->context);

            if (timer->repeat) {
                timer_timers[i]->handle = app_timer_send_event(app_ctx, timer->interval_ms, TIMER_HANDLER_COOKIE);
            }
            else {
                timer_cancel(timer, app_ctx);
            }
            
            break;
        }
    }
}
