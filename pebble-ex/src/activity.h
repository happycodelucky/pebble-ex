//
//  activity.h
//  pebble-ex
//
//  Created by Paul Bates on 10/7/13.
//  Copyright (c) 2013 Sonos. All rights reserved.
//

#ifndef __PEBBLE_EX_ACTIVITY_H__
#define __PEBBLE_EX_ACTIVITY_H__

#include "pebble_os.h"
#include "timer.h"

// Activity indicator layer for displaying an animating indeterminate indicator.
// Starting and stopping the animation is explicit, using the activity_layer_start_animation and
// activity_layer_stop_animation functions.
typedef struct {
    Layer layer;
    GColor color : 2;
    GColor background_color : 2;

    Timer animation_timer;

    bool reset_loop : 1;
    unsigned int block1_prog : 3;
    unsigned int block2_prog : 3;
    unsigned int block3_prog : 3;
} ActivityLayer;

// Initialize an activity layer
void activity_layer_init(ActivityLayer *activity_layer);

// Sets the activity color
void activity_layer_set_color(ActivityLayer *activity_layer, GColor color);
// Sets the background color of the activity indicator
void activity_layer_set_background_color(ActivityLayer *activity_layer, GColor color);

// Resets the activity indicator to the very first frame of animation.
// Starting and stopping the activity animation does not reset when resuming an animation
void activity_layer_reset(ActivityLayer *activity_layer);

// Starts the animation for the activity layer.
void activity_layer_start_animation(ActivityLayer *activity_layer, AppContextRef app_ctx);
// Stops an running animation for the activity layer.
void activity_layer_stop_animation(ActivityLayer *activity_layer, AppContextRef app_ctx);

#endif // __PEBBLEX_ACTIVITY_H__
