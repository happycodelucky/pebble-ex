//
//  grect.h
//  pebble-ex
//
//  Created by Paul Bates on 10/13/13.
//  Copyright (c) 2013 Sonos. All rights reserved.
//

#ifndef __PEBBLE_EX_GRECT_H__
#define __PEBBLE_EX_GRECT_H__

#include "pebble_os.h"

int grect_get_width(const GRect * const rect);
int grect_get_height(const GRect * const rect);
int grect_get_min_x(const GRect * const rect);
int grect_get_min_y(const GRect * const rect);
int grect_get_max_x(const GRect * const rect);
int grect_get_max_Y(const GRect * const rect);
int grect_get_mid_x(const GRect * const rect);
int grect_get_mid_y(const GRect * const rect);

GRect grect_center_rect(const GRect * const rect, const GRect * const container_rect);

#endif // __PEBBLEX_GRECT_H__
