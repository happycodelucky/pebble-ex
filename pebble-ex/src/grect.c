//
//  grect.h
//  pebble-ex
//
//  Created by Paul Bates on 10/13/13.
//  Copyright (c) 2013 Sonos. All rights reserved.
//

#include "grect.h"

#ifdef __clang__
#pragma mark - GRect methods
#endif


int grect_get_width(const GRect * const rect)
{
    return rect->size.w;
}

int grect_get_height(const GRect * const rect)
{
    return rect->size.h;
}

int grect_get_min_x(const GRect * const rect)
{
    return rect->origin.x;
}

int grect_get_min_y(const GRect * const rect)
{
    return rect->origin.y;
}

int grect_get_max_x(const GRect * const rect)
{
    return rect->origin.x + rect->size.w;
}

int grect_get_max_y(const GRect * const rect)
{
    return rect->origin.y + rect->size.h;
}

int grect_get_mid_x(const GRect * const rect)
{
    return rect->origin.x + (rect->size.w / 2);
}

int grect_get_mid_y(const GRect * const rect)
{
    return rect->origin.y + (rect->size.h / 2);
}

GRect grect_center_rect(const GRect * const rect, const GRect * const container_rect)
{
    return GRect((grect_get_width(container_rect) - grect_get_width(rect)) / 2,
                 (grect_get_height(container_rect) - grect_get_height(rect)) / 2,
                 grect_get_width(rect),
                 grect_get_height(rect));
}
