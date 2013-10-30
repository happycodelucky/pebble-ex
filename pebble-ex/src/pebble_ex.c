//
//  pebble_ex.c
//  pebble-ex
//
//  Created by Paul Bates on 10/13/13.
//  Copyright (c) 2013 Sonos. All rights reserved.
//

#include "pebble_ex.h"

void layer_center_in_parent(Layer *layer)
{
    if (!layer || !layer->parent)
        return;

    GRect frame = layer_get_frame(layer);
    GRect parent_frame = layer_get_frame(layer->parent);
    layer_set_frame(layer, grect_center_rect(&frame, &parent_frame));
}
