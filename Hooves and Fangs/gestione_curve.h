#pragma once
#include "strutture.h"
void t_equidistanti(int index_curva);
void t_corde(int index_curva);
float dx(int i, float* t, Curva* curva);
float dy(int i, float* t, Curva* curva);
float DX(int i, float* t, int index_curva);
float DY(int i, float* t, int index_curva);
void InterpolazioneHermite(float* t, Curva* curva, int indx_curva, vec4 color_top, vec4 color_bot);
void CostruisciHermite( Curva* curva, int index_curva);