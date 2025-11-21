#pragma once
#include "strutture.h"

void INIT_PIANO(Figura* fig, vec4 color_top, vec4 color_bottom);

void INIT_SQUARE(Figura* fig, vec4 color);

void INIT_PONY(FiguraHermite* fig);

void INIT_CARROT(FiguraHermite* fig, bool isRipe);

void INIT_FANG(FiguraHermite* fig);

void INIT_ATTACK_CURVES(FiguraHermite* curveAtkPony, FiguraHermite* curveAtkFang);

void INIT_HEART(float cx, float cy, float raggiox, float raggioy, Figura* fig, vec4 color_top, vec4 color_bottom);