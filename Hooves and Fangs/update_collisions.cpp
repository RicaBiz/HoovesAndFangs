#pragma once
#include "update_collisions.h"

extern vector<FiguraHermite> fangs;
extern vector<FiguraHermite> carrots;
//extern vector<Figura> carrots;
extern FiguraHermite pony;

void checkCollisions(void)
{
	updateBB(&pony.hitbox);
	for (int i = 0; i < fangs.size(); i++) {
		updateBB(&fangs[i].hitbox);
		// Controlla la collisione tra il pony e ogni fang
		if (checkCollision(pony.hitbox, fangs[i].hitbox)) {
			// Se un fang colpisce il pony, il fang attacca e riduce la salute del pony
			if (updateFieldFangStatus(i, ATTACKING))
				ponyTakeDamage();
		}
		
		// Controlla la collisione tra ogni fang e ogni carota
		for (int j = 0; j < carrots.size(); j++) {
			updateBB(&carrots[j].hitbox);
			if (checkCollision(carrots[j].hitbox, fangs[i].hitbox)) {
				// Se un fang colpisce una carota, il fang attacca e la carota viene rimossa
				if (updateFieldFangStatus(i, ATTACKING))
					removeFieldCarrot(j);
			}
		}

		// Se il pony sta attaccando, controlla la collisione tra il pony e ogni fang
		if (getPony().status == ATTACKING) {
			vec4 ponySideBB = findSideBB(&pony.hitbox, getPony().direction);
			vec4 fangBBMin = fangs[i].hitbox.min_BB_w;
			vec4 fangBBMax = fangs[i].hitbox.max_BB_w;
			// Se il pony colpisce un fang, il fang muore
			if (checkCollision(fangBBMin, fangBBMax, vec4(ponySideBB.x, ponySideBB.y, 0.0, 1.0), vec4(ponySideBB.z, ponySideBB.w, 0.0, 1.0))) {
				updateFieldFangStatus(i, DEAD);
			}
		}
	}

	// Controlla la collisione tra il pony e ogni carota
	for (int j = 0; j < carrots.size(); j++) {
		updateBB(&carrots[j].hitbox);
		if (checkCollision(pony.hitbox, carrots[j].hitbox)) {
			// Se il pony colpisce una carota, la carota viene raccolta
			harvestFieldCarrot(j);
			break; // Evita di contare pi collisioni in un singolo aggiornamento
		}
	}


}
