//
//  activity.c
//  pebble-ex
//
//  Created by Paul Bates on 10/7/13.
//  Copyright (c) 2013 Sonos. All rights reserved.
//

#include "activity.h"

#include "grect.h"


#define ACTIVITY_BLOCK_MIN_SIZE 8
#define ACTIVITY_BLOCK_MAX_SIZE 14
#define ACTIVITY_BLOCK_PADDING 2
#define ACTIVITY_PULSE_MS 100
#define ACTIVITY_RESET_MS 280
#define ACTIVITY_RECT GRect(0, 0, (ACTIVITY_BLOCK_MAX_SIZE * 3 + ACTIVITY_BLOCK_PADDING * 2), ACTIVITY_BLOCK_MAX_SIZE)

// Forward declaration
void activity_layer_step(ActivityLayer *activity_layer);
void activity_layer_update_callback(Layer *layer, GContext *ctx);
void activity_animation_timer_callback(AppContextRef app_ctx, void *ctx);


#ifdef __clang__
#pragma mark - Initialization
#endif

/*******************************************************************************
 Activity Layer
 ******************************************************************************/

void activity_layer_init(ActivityLayer *activity_layer)
{
    layer_init(&activity_layer->layer, ACTIVITY_RECT);
    layer_set_update_proc(&activity_layer->layer, activity_layer_update_callback);

    // Create timer needed for animation
    timer_init(&activity_layer->animation_timer, activity_animation_timer_callback, ACTIVITY_RESET_MS);
    timer_set_repeat(&activity_layer->animation_timer, true);
    timer_set_context(&activity_layer->animation_timer, activity_layer);

    // Set layer defaults
    activity_layer->color = GColorBlack;
    activity_layer->background_color = GColorClear;
    activity_layer_reset(activity_layer);
}


#ifdef __clang__
#pragma mark - Publc properties
#endif

void activity_layer_set_color(ActivityLayer *activity_layer, GColor color)
{
    activity_layer->color = color;
    layer_mark_dirty(&activity_layer->layer);
}

void activity_layer_set_background_color(ActivityLayer *activity_layer, GColor color)
{
    activity_layer->background_color = color;
    layer_mark_dirty(&activity_layer->layer);
}


#ifdef __clang__
#pragma mark - Public functions
#endif

void activity_layer_reset(ActivityLayer *activity_layer)
{
    activity_layer->reset_loop = false;
    activity_layer->block1_prog = 0;
    activity_layer->block2_prog = 0;
    activity_layer->block3_prog = 0;

    layer_mark_dirty(&activity_layer->layer);

    timer_set_timeout(&activity_layer->animation_timer, ACTIVITY_RESET_MS);
}

void activity_layer_start_animation(ActivityLayer *activity_layer, AppContextRef app_ctx)
{
    timer_schedule(&activity_layer->animation_timer, app_ctx);
}

void activity_layer_stop_animation(ActivityLayer *activity_layer, AppContextRef app_ctx)
{
    timer_cancel(&activity_layer->animation_timer, app_ctx);
}


#ifdef __clang__
#pragma mark - Private functions
#endif

void activity_layer_step(ActivityLayer *activity_layer)
{
    int max = (ACTIVITY_BLOCK_MAX_SIZE - ACTIVITY_BLOCK_MIN_SIZE) / 2;

    // When in a reset loop, animate out the 3rd block
    if (activity_layer->reset_loop) {
        if (activity_layer->block3_prog > 0) {
            activity_layer->block3_prog--;
            if (activity_layer->block2_prog > 0) {
                activity_layer->block2_prog--;
            }
            if (activity_layer->block1_prog > 0) {
                activity_layer->block1_prog--;
            }
            if (activity_layer->block3_prog == 0) {
                activity_layer->reset_loop = false;

                // Alter the animation timing during a reset to take a little extra time
                timer_set_timeout(&activity_layer->animation_timer, ACTIVITY_RESET_MS);
            }
        }

        layer_mark_dirty(&activity_layer->layer);
        return;
    }

    // Set the animation timing, because this can be changed in a reset loop
    timer_set_timeout(&activity_layer->animation_timer, ACTIVITY_PULSE_MS);

    if (activity_layer->block3_prog > 0) {
        if (activity_layer->block3_prog < max) {
            activity_layer->block3_prog++;

            // Enter a reset loop when
            if (activity_layer->block3_prog == max) {
                activity_layer->reset_loop = true;
            }
        }

        if (activity_layer->block2_prog > 0) {
            activity_layer->block2_prog--;
        }
        if (activity_layer->block1_prog > 0) {
            activity_layer->block1_prog--;
        }
    }

    else if (activity_layer->block2_prog > 0) {
        if (activity_layer->block2_prog < max) {
            activity_layer->block2_prog++;
        }
        else {
            activity_layer->block3_prog++;
        }

        if (activity_layer->block1_prog > 0) {
            activity_layer->block1_prog--;
        }
    }

    else {
        if (activity_layer->block1_prog < max) {
            activity_layer->block1_prog++;
            if (activity_layer->block3_prog > 0) {
                activity_layer->block3_prog--;
            }
        }
        else {
            activity_layer->block2_prog++;
        }
    }
    
    layer_mark_dirty(&activity_layer->layer);
}


#ifdef __clang__
#pragma mark - Callbacks
#endif

void activity_layer_update_callback(Layer *layer, GContext *ctx)
{
    ActivityLayer *activity_layer = (ActivityLayer *)layer;

    GRect draw_rect = layer_get_bounds(layer);

    // Draw background
    graphics_context_set_fill_color(ctx, activity_layer->background_color);
    graphics_fill_rect(ctx, draw_rect, 0, GCornerNone);

    graphics_context_set_fill_color(ctx, activity_layer->color);

    // Draw the activity blocks
    int max_delta = (ACTIVITY_BLOCK_MAX_SIZE - ACTIVITY_BLOCK_MIN_SIZE);
    GRect activity_rect = grect_center_rect(&ACTIVITY_RECT, &draw_rect);
    int x_offset = grect_get_min_x(&activity_rect) - (max_delta / 2);
    int y_offset = grect_get_min_y(&activity_rect) - (max_delta / 2);

    int size = ACTIVITY_BLOCK_MIN_SIZE + (activity_layer->block1_prog * 2);
    int pos_delta = max_delta - activity_layer->block1_prog;
    GRect block_rect = GRect(x_offset + pos_delta, y_offset + pos_delta, size, size);
    graphics_fill_rect(ctx, block_rect, 0, GCornerNone);

    x_offset += ACTIVITY_BLOCK_MAX_SIZE + ACTIVITY_BLOCK_PADDING;

    size = ACTIVITY_BLOCK_MIN_SIZE + (activity_layer->block2_prog * 2);
    pos_delta = max_delta - activity_layer->block2_prog;
    block_rect = GRect(x_offset + pos_delta, y_offset + pos_delta, size, size);
    graphics_fill_rect(ctx, block_rect, 0, GCornerNone);

    x_offset += ACTIVITY_BLOCK_MAX_SIZE + ACTIVITY_BLOCK_PADDING;

    size = ACTIVITY_BLOCK_MIN_SIZE + (activity_layer->block3_prog * 2);
    pos_delta = max_delta - activity_layer->block3_prog;
    block_rect = GRect(x_offset + pos_delta, y_offset + pos_delta, size, size);
    graphics_fill_rect(ctx, block_rect, 0, GCornerNone);
}

void activity_animation_timer_callback(AppContextRef app_ctx, void *ctx)
{
    ActivityLayer *layer = (ActivityLayer *)ctx;
    activity_layer_step(layer);
}
