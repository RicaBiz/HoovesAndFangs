#pragma once
#include "strutture.h"
#include "inizializzazioni.h"
extern FiguraHermite figuraCorrente;
extern float* t;
int pval = 200;  //Numero dei valori del parametro in cui visualizzare la curva


void t_equidistanti(int index_curva) {
	int i;
	float step_t;
	t = new float[figuraCorrente.curve[index_curva].CP.size()];
	step_t = 1.0 / (figuraCorrente.curve[index_curva].CP.size() - 1);
	for (i = 0; i < figuraCorrente.curve[index_curva].CP.size(); i++)
		t[i] = (float)i * step_t;
	t[figuraCorrente.curve[index_curva].CP.size()-1] = 1.0;

}
void t_corde(int index_curva)
{
	int i;
	float step_t;
	t = new float[figuraCorrente.curve[index_curva].CP.size()];
	t[0] = 0;
	for (i = 1; i < figuraCorrente.curve[index_curva].CP.size(); i++)
		t[i] = t[i - 1] + distance(figuraCorrente.curve[index_curva].CP[i - 1], figuraCorrente.curve[index_curva].CP[i]);

	for (i = 0; i < figuraCorrente.curve[index_curva].CP.size(); i++)
		t[i] = t[i] / t[figuraCorrente.curve[index_curva].CP.size() - 1];

}

/// /////////////////////////////////// Disegna geometria //////////////////////////////////////
//Per Curve di hermite
#define PHI0(t)  (2.0*t*t*t-3.0*t*t+1)
#define PHI1(t)  (t*t*t-2.0*t*t+t)
#define PSI0(t)  (-2.0*t*t*t+3.0*t*t)
#define PSI1(t)  (t*t*t-t*t)


float dx(int i, float* t, Curva* curva)
{
	if (i == 0)
		return  0.5 * (1.0 - curva->Tens[i]) * (1.0 - curva->Bias[i]) * (1.0 - curva->Cont[i]) * (curva->CP[i + 1].x - curva->CP[i].x) / (t[i + 1] - t[i]);
	if (i == curva->CP.size() - 1)
		return  0.5 * (1.0 - curva->Tens[i]) * (1.0 - curva->Bias[i]) * (1.0 - curva->Cont[i]) * (curva->CP[i].x - curva->CP[i - 1].x) / (t[i] - t[i - 1]);

	if (i % 2 == 0)
		return  0.5 * (1.0 - curva->Tens[i]) * (1.0 + curva->Bias[i]) * (1.0 + curva->Cont[i]) * (curva->CP.at(i).x - curva->CP.at(i - 1).x) / (t[i] - t[i - 1]) + 0.5 * (1 - curva->Tens[i]) * (1 - curva->Bias[i]) * (1 - curva->Cont[i]) * (curva->CP.at(i + 1).x - curva->CP.at(i).x) / (t[i + 1] - t[i]);
	else
		return  0.5 * (1.0 - curva->Tens[i]) * (1.0 + curva->Bias[i]) * (1.0 - curva->Cont[i]) * (curva->CP.at(i).x - curva->CP.at(i - 1).x) / (t[i] - t[i - 1]) + 0.5 * (1 - curva->Tens[i]) * (1 - curva->Bias[i]) * (1 + curva->Cont[i]) * (curva->CP.at(i + 1).x - curva->CP.at(i).x) / (t[i + 1] - t[i]);
}
float dy(int i, float* t,  Curva* curva)
{
	if (i == 0)
		return 0.5 * (1.0 - curva->Tens[i]) * (1.0 - curva->Bias[i]) * (1.0 - curva->Cont[i]) * (curva->CP.at(i + 1).y - curva->CP.at(i).y) / (t[i + 1] - t[i]);
	if (i == curva->CP.size() - 1)
		return  0.5 * (1.0 - curva->Tens[i]) * (1.0 - curva->Bias[i]) * (1.0 - curva->Cont[i]) * (curva->CP.at(i).y - curva->CP.at(i - 1).y) / (t[i] - t[i - 1]);

	if (i % 2 == 0)
		return  0.5 * (1.0 - curva->Tens[i]) * (1.0 + curva->Bias[i]) * (1.0 + curva->Cont[i]) * (curva->CP.at(i).y - curva->CP.at(i - 1).y) / (t[i] - t[i - 1]) + 0.5 * (1 - curva->Tens[i]) * (1 - curva->Bias[i]) * (1 - curva->Cont[i]) * (curva->CP.at(i + 1).y - curva->CP.at(i).y) / (t[i + 1] - t[i]);
	else
		return  0.5 * (1.0 - curva->Tens[i]) * (1.0 + curva->Bias[i]) * (1.0 - curva->Cont[i]) * (curva->CP.at(i).y - curva->CP.at(i - 1).y) / (t[i] - t[i - 1]) + 0.5 * (1 - curva->Tens[i]) * (1 - curva->Bias[i]) * (1 + curva->Cont[i]) * (curva->CP.at(i + 1).y - curva->CP.at(i).y) / (t[i + 1] - t[i]);
}

float DX(int i, float* t, int index_curva)
{
	//Nei vertici di controllo per i quali non sono stati modificati i parametri Tens, Bias, Cont il valore della derivata della componente x della curva è quello originale, altrimenti è quello che è stato modificato nella funzione 
	//keyboardfunc  in seguito alla modifica dei valori Tens, Bias e Cont.
	 
	if (figuraCorrente.curve[index_curva].Derivata.at(i).x == 0)
	   return dx(i, t, &figuraCorrente.curve[index_curva]);
	
	if (figuraCorrente.curve[index_curva].Derivata.at(i).x != 0)
		return figuraCorrente.curve[index_curva].Derivata.at(i).x;
	
	 
	
}

float DY(int i, float* t, int index_curva)
{
	// Nei vertici di controllo per i quali non sono stati modificati i parametri Tens, Bias, Cont il valore della derivata della componente y della curva è quello originale, altrimenti è quello che è stato modificato nella funzione
		//keyboardfunc  in seguito alla modifica dei valori Tens, Bias e Cont.
	 
	if (figuraCorrente.curve[index_curva].Derivata.at(i).y == 0)
		return dy(i, t, &figuraCorrente.curve[index_curva]);
	 
	if (figuraCorrente.curve[index_curva].Derivata.at(i).y != 0)
		return figuraCorrente.curve[index_curva].Derivata.at(i).y;

}

void InterpolazioneHermite(float* t, Curva* curva, int index_curva,vec4 color_top, vec4 color_bot)
{
	float p_t = 0, p_b = 0, p_c = 0, x, y;
	float passotg = 1.0 / (float)(pval - 1);
	int j;

	 
	float tg = 0, tgmapp, ampiezza;
	int i = 0;
	int is = 0; //indice dell'estremo sinistro dell'intervallo [t(i),t(i+1)] a cui il punto tg
	//appartiene
		
	for (tg = 0; tg <= 1; tg += passotg)
	{
		//Localizzo l'intervallo a cui tg appartiente
		if (tg > t[is + 1]) is++;
		 
		ampiezza = (t[is + 1] - t[is]);

		//mappo tg nell'intervallo [0,1]

		tgmapp = (tg - t[is]) / ampiezza;
		 
		x = curva->CP[is].x * PHI0(tgmapp) + DX(is, t, index_curva) * PHI1(tgmapp) * ampiezza + curva->CP[is + 1].x * PSI0(tgmapp) + DX(is + 1, t, index_curva) * PSI1(tgmapp) * ampiezza;
		y = curva->CP[is].y * PHI0(tgmapp) + DY(is, t, index_curva) * PHI1(tgmapp) * ampiezza + curva->CP[is + 1].y * PSI0(tgmapp) + DY(is + 1, t, index_curva) * PSI1(tgmapp) * ampiezza;
		 
		curva->vertices.push_back(vec3(x, y, 0.0));
		 
	    curva->colors.push_back(color_bot); 
		
		curva->nv = curva->vertices.size();
		 
	}
	 
}

void CostruisciHermite( Curva* curva, int index_curva)
{
	curva->vertices.clear();
	curva->colors.clear();
	curva->tg.clear();
	//curva->col_tg.clear();
	vec4 color_top = vec4(1.0, 0.0, 0.0, 1.0);
	vec4 color_bot = vec4(1.0, 0.8, 1.0, 1.0);
	InterpolazioneHermite(t, curva, index_curva, color_top, color_bot);
	
	//Calcolo di una misura normalizzata della distanza di ciascun vertice
	//  dal  centroide (il punto medio geometrico) della curva. 
	vec3 centroid(0.0f);
	for (auto& v : curva->vertices)
		centroid += v;
	centroid /= curva->vertices.size();
	
	curva->distNormalized.clear();

	// Calcolo della distanza massima dal centro
	float maxDist = 0.0f;
	for (auto& v : curva->vertices) {
		float dist = distance(v, centroid);
		maxDist = std::max(maxDist, dist);
	}

	// Calcolo valori normalizzati per ogni vertice

	for (auto& v : curva->vertices) {
		float dist = glm::distance(v, centroid);
		curva->distNormalized.push_back(dist / maxDist);
	}
	UPDATE_VAO_DYNAMIC(curva,true);
}