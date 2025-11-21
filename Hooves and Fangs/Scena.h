#pragma once
#include "strutture.h"
void costruisci_Scena(Figura* bkg, FiguraHermite* pony, vector<Figura>* hearts);

void aggiornaHearts(vector<Figura>* hearts);

void aggiornaFangs(vector<FiguraHermite>* fangs);

void aggiornaCarrots(vector<FiguraHermite>* carrots);