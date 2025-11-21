#pragma once
#include "init_geometrie.h"
#include "Utilities.h"
#include "inizializzazioni.h"
#include "gestione_curve.h"
#define PI 3.14159265358979323

const char* ponyFileName = "pony.txt";
const char* ponyAtkFileName = "pony_attacking.txt";
const char* carrotFileName = "carrot.txt";
const char* seedFileName = "seed.txt";
const char* fangFileName = "fang.txt";
const char* fangAtkFileName = "fang_attacking.txt";

void INIT_PIANO(Figura* fig, vec4 color_top, vec4 color_bottom)
{
    fig->vertices.clear();

    fig->vertices.push_back(vec3(-1.0, -1.0, 0.0));
    fig->colors.push_back(color_top);
    fig->vertices.push_back(vec3(1.0, -1.0, 0.0));
    fig->colors.push_back(color_bottom);
    fig->vertices.push_back(vec3(1.0, 1.0, 0.0));
    fig->colors.push_back(color_top);
    fig->vertices.push_back(vec3(-1.0, 1.0, 0.0));
    fig->colors.push_back(color_bottom);

    //Calcolo AABB della figura
    findBB(fig);
    //Aggiungo al campo vertices le 4 coordinate del Bounding box in coordinate dell'oggetto
    fig->vertices.push_back(vec3(fig->min_BB_obj.x, fig->min_BB_obj.y, 0.0));
    fig->colors.push_back(vec4(1.0, 1.0, 1.0, 1.0));
    fig->vertices.push_back(vec3(fig->max_BB_obj.x, fig->min_BB_obj.y, 0.0));
    fig->colors.push_back(vec4(1.0, 1.0, 1.0, 1.0));
    fig->vertices.push_back(vec3(fig->max_BB_obj.x, fig->max_BB_obj.y, 0.0));
    fig->colors.push_back(vec4(1.0, 1.0, 1.0, 1.0));
    fig->vertices.push_back(vec3(fig->min_BB_obj.x, fig->max_BB_obj.y, 0.0));
    fig->colors.push_back(vec4(1.0, 0.0, 0.0, 1.0));
    fig->nv = fig->vertices.size();
    fig->render = GL_TRIANGLE_FAN;
}

void INIT_SQUARE(Figura* fig, vec4 color)
{
	fig->vertices.clear();
	fig->vertices.push_back(vec3(-0.5, -0.5, 0.0));
	fig->colors.push_back(color);
	fig->vertices.push_back(vec3(0.5, -0.5, 0.0));
	fig->colors.push_back(color);
	fig->vertices.push_back(vec3(0.5, 0.5, 0.0));
	fig->colors.push_back(color);
	fig->vertices.push_back(vec3(-0.5, 0.5, 0.0));
	fig->colors.push_back(color);

	//Calcolo AABB della figura
	findBB(fig);
	//Aggiungo al campo vertices le 4 coordinate del Bounding box in coordinate dell'oggetto
	fig->vertices.push_back(vec3(fig->min_BB_obj.x, fig->min_BB_obj.y, 0.0));
	fig->colors.push_back(vec4(1.0, 1.0, 1.0, 1.0));
	fig->vertices.push_back(vec3(fig->max_BB_obj.x, fig->min_BB_obj.y, 0.0));
	fig->colors.push_back(vec4(1.0, 1.0, 1.0, 1.0));
	fig->vertices.push_back(vec3(fig->max_BB_obj.x, fig->max_BB_obj.y, 0.0));
	fig->colors.push_back(vec4(1.0, 1.0, 1.0, 1.0));
	fig->vertices.push_back(vec3(fig->min_BB_obj.x, fig->max_BB_obj.y, 0.0));
	fig->colors.push_back(vec4(1.0, 0.0, 0.0, 1.0));

	fig->nv = fig->vertices.size();
	fig->render = GL_TRIANGLE_FAN;
}

void INIT_HEART(float cx, float cy, float raggiox, float raggioy, Figura* fig, vec4 color_top, vec4 color_bottom)
{


    int i;
    float t, xx, yy;
    float stepA = (2 * PI) / fig->nTriangles;
    fig->vertices.push_back(vec3(cx, cy, 0.0));

    fig->colors.push_back(color_top);

    for (i = 0; i <= fig->nTriangles; i++)
    {
        t = (float)i * stepA;
        xx = cx + raggiox * (16 * pow(sin(t), 3));
        yy = cy + raggioy * ((13 * cos(t) - 5 * cos(2 * t) - 2 * cos(3 * t) - cos(4 * t)));
        fig->vertices.push_back(vec3(xx, yy, 0.0));
        fig->colors.push_back(color_bottom);
    }

    //Calcolo AABB della figura
    findBB(fig);
    //Aggiungo al campo vertices le 4 coordinate del Bounding box in coordinate dell'oggetto
    fig->vertices.push_back(vec3(fig->min_BB_obj.x, fig->min_BB_obj.y, 0.0));
    fig->colors.push_back(vec4(1.0, 1.0, 1.0, 1.0));
    fig->vertices.push_back(vec3(fig->max_BB_obj.x, fig->min_BB_obj.y, 0.0));
    fig->colors.push_back(vec4(1.0, 1.0, 1.0, 1.0));
    fig->vertices.push_back(vec3(fig->max_BB_obj.x, fig->max_BB_obj.y, 0.0));
    fig->colors.push_back(vec4(1.0, 1.0, 1.0, 1.0));
    fig->vertices.push_back(vec3(fig->min_BB_obj.x, fig->max_BB_obj.y, 0.0));
    fig->colors.push_back(vec4(1.0, 0.0, 0.0, 1.0));
    fig->nv = fig->vertices.size();
    fig->render = GL_TRIANGLE_FAN;

}

//void INIT_PONY(FiguraHermite* fig, Figura* ponyBB) {
//    int i;
//    struct Dati {
//        int index_curva;
//        float x;
//        float y;
//        float z;
//        float Tens;
//        float Bias;
//        float Cont;
//    };
//
//    // Apertura input file
//    FILE* file = fopen("pony.txt", "r");
//    if (file == NULL) {
//        perror("Impossibile aprire il file");
//    }
//
//    // Resetta le curve esistenti (opzionale: elimina punti precedenti)
//    for (auto& c : *fig) {
//        c.CP.clear();
//        c.Tens.clear();
//        c.Bias.clear();
//        c.Cont.clear();
//        c.colCP.clear();
//        c.Derivata.clear();
//    }
//
//    Dati dati;
//    int total_line = 0;
//
//    while (fscanf(file, "%d %f %f %f %f %f %f", &dati.index_curva, &dati.x, &dati.y, &dati.z, &dati.Tens, &dati.Bias, &dati.Cont) == 7) {
//        // Alloca curve vuote se non esistono abbastanza elementi
//        cout << "CURVA " << dati.index_curva << endl;
//        while (fig->size() <= dati.index_curva) {
//            fig->push_back(Curva());
//            cout << "Nuova curva " << dati.index_curva << endl;
//        }
//        // Aggiunge i dati solo alla curva giusta
//        (*fig)[dati.index_curva].CP.push_back(vec3(dati.x, dati.y, dati.z));
//        (*fig)[dati.index_curva].Tens.push_back(dati.Tens);
//        (*fig)[dati.index_curva].Bias.push_back(dati.Bias);
//        (*fig)[dati.index_curva].Cont.push_back(dati.Cont);
//        (*fig)[dati.index_curva].colCP.push_back(vec4(0.0, 0.0, 0.0, 1.0));
//        (*fig)[dati.index_curva].Derivata.push_back(vec3(0.0, 0.0, 0.0));
//        total_line++;
//
//    }
//
//
//    fclose(file);
//    cout << "NUMERO DI CURVE " << fig->size() << endl;
//
//    for (int i = 0; i < fig->size(); i++)
//    {
//        (*fig)[i].ncp = (*fig)[i].CP.size();
//        (*fig)[i].nv = 200;
//        (*fig)[i].ntg = 100;
//        INIT_VAO_DYNAMIC(&(*fig)[i], true);
//        cout << "Curva " << i << "Numero di punti " << (*fig)[i].ncp << endl;
//
//        figuraCorrente = *fig;
//        t_corde(i);
//        // Calcola la curva di Hermite
//        CostruisciHermite(&(*fig)[i], i);
//
//        // Aggiorna il VAO della curva
//        UPDATE_VAO_DYNAMIC(&(*fig)[i], true);
//    }
//
//    
//}

void INIT_PONY(FiguraHermite* fig) {

    loadHermite(fig, ponyFileName);
    findHermiteBB(fig, LEGBSX);
}

void INIT_CARROT(FiguraHermite* fig, bool isRipe) {

    if (isRipe)
        loadHermite(fig, carrotFileName);
    else
        loadHermite(fig, seedFileName);
    findHermiteBB(fig, CARROTBODY);
}

void INIT_FANG(FiguraHermite* fig) {
    loadHermite(fig, fangFileName);
    findHermiteBB(fig, RIGHTFANG);
}

void INIT_ATTACK_CURVES(FiguraHermite* curveAtkPony, FiguraHermite* curveAtkFang) {
    loadHermite(curveAtkPony, ponyAtkFileName, false);
    loadHermite(curveAtkFang, fangAtkFileName, false);
}