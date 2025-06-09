#ifndef SCENE_H
#define SCENE_H

#include "clock.h"
#include "config.h"
#include "display.h"
#include "sensor.h"

#define SCENE_LIFETIME 25 // how long a scene should be displayed measured in cycles of the main loop

typedef enum { overview, co2graph, end } SCENE_TYPE;
extern SCENE_TYPE SCENE_current_scene;

extern void SCENE_display_current_scene();
extern void SCNENE_advance();

#endif
