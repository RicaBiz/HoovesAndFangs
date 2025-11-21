#pragma once
#include <glad/glad.h>
#include "ShaderMaker.h"
#include "strutture.h"
#include "inizializzazioni.h"
#include "init_geometrie.h"
#include "Scena.h"
#include "Controller.hpp"

extern int width, height;
extern unsigned int programId;
extern FiguraHermite attacking_pony;
extern FiguraHermite attacking_fang;

void costruisci_Cuori(vector<Figura>* hearts, int nHearts)
{
	hearts->clear();
	for (int i = 0; i < nHearts; i++) {
		Figura heart;
		heart.render = GL_TRIANGLE_FAN;
		heart.nTriangles = 60;
		heart.position = vec3(-0.9f + i * 0.1f, 0.9f, 0.0f);
		heart.scale = vec3(0.0075, 0.01, 0.0);
		heart.shader_programId = programId;
		INIT_HEART(0.0, 0.0, 0.4, 0.4, &heart, vec4(1.0, 1.0, 1.0, 1.0), vec4(1.0, 0.0, 0.0, 1.0));
		heart.Model = mat4(1.0);
		//Genera il VAO della struttura  butterrfly
		INIT_VAO(&heart);
		hearts->push_back(heart);
	}
}

void costruisci_Scena(Figura* bkg, FiguraHermite* pony, vector<Figura>* hearts)
{

	//Costruisce il piano del background
	bkg->nTriangles = 2;
	bkg->render = GL_TRIANGLE_FAN;
	bkg->shader_programId = programId;
	INIT_PIANO(bkg, vec4(0.0, 0.0, 1.0, 1.0), vec4(1.0, 1.0, 1.0, 1.0));
	INIT_VAO(bkg);
	bkg->Model = mat4(1.0);
	bkg->Model = scale(bkg->Model, vec3((float)(width), (float)(height), 1.0));


	//Costruisce il ponyBB
	pony->hitbox.scale = vec3(0.15, 0.2, 1.0);

	INIT_ATTACK_CURVES(&attacking_pony, &attacking_fang);
	for (auto& curva : attacking_pony.curve) {
		curva.scale = vec3(0.15, 0.2, 1.0);
	}
	for (auto& curva : attacking_fang.curve) {
		curva.scale = vec3(0.12, 0.16, 1.0);
	}
	// Pony con FiguraHermite
	INIT_PONY(pony);
	for (auto& curva : pony->curve) {
		curva.scale = vec3(0.15, 0.2, 1.0);
	}

	////Costruisce il fermenter
	//fermenter->nTriangles = 2;
	//fermenter->render = GL_TRIANGLE_FAN;
	//fermenter->shader_programId = programId;
	//fermenter->position = vec3(0.5f, 0.9f, 0.0f);
	//fermenter->scale = vec3(0.1, 0.1, 1.0);
	//INIT_SQUARE(fermenter, vec4(0.8, 0.3, 0.0, 1.0));
	//fermenter->Model = mat4(1.0);
	////Genera il VAO della struttura fermenter
	//INIT_VAO(fermenter);

	costruisci_Cuori(hearts, getHealth());

}

//Aggiorna il numero di hearts se e' cambiato.
void aggiornaHearts(vector<Figura>* hearts)
{
	int nHearts = getHealth();
	if (nHearts != hearts->size()) 
		costruisci_Cuori(hearts, nHearts);
}

//Aggiorna il numero di fangs se e' cambiato.
void aggiornaFangs(vector<FiguraHermite>* fangs)
{
	vector<FangInfo> fangsInfo = getFangsInfo();
	if (fangs->size() != fangsInfo.size()) {
		fangs->clear();
		for (auto& fang : fangsInfo) {
			FiguraHermite fangFig;
			INIT_FANG(&fangFig);
			for (auto& curva : fangFig.curve) {
				curva.position = vec3(fang.position.x, fang.position.y, 0.0f);
				curva.scale = vec3(0.12, 0.16, 0.0);
			}
			fangFig.hitbox.position = vec3(fang.position.x, fang.position.y, 0.0f);
			fangFig.hitbox.scale = vec3(0.12, 0.16, 0.0);
			fangs->push_back(fangFig);
		}
	}
}

//Aggiorna le carrots sia nel numero che nello stato di crescita.
void aggiornaCarrots(vector<FiguraHermite>* carrots)
{
	vector<CarrotInfo> fieldCarrots = getCarrotsInfo();
	if (isCarrotToUpdate() || carrots->size() != fieldCarrots.size()) {
		resetCarrotToUpdate();
		carrots->clear();
		for (auto& carrot : fieldCarrots) {
			FiguraHermite carrotFig;
			INIT_CARROT(&carrotFig, carrot.status == RIPE);
			for (auto& curva : carrotFig.curve) {
				curva.position = vec3(carrot.position.x, carrot.position.y, 0.0f);
				curva.scale = vec3(0.09, 0.12, 0.0);
			}
			carrotFig.hitbox.position = vec3(carrot.position.x, carrot.position.y, 0.0f);
			carrotFig.hitbox.scale = vec3(0.06, 0.08, 0.0);
			carrots->push_back(carrotFig);
		}
	}
}