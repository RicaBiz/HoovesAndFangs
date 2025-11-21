#pragma once
#include "Controller.hpp"
#include "Utilities.h"

void initializeFieldModel()
{
	resetField();
}

int getFieldScore()
{
	return getScore();
}

int getHealth()
{
	return getPony().health;
}

glm::vec3 getPonyPosition()
{
	glm::vec2 ponyPosition = getPony().position;
	glm::vec2 fieldDimensions = getFieldDimensions();
	return positionModelToView(ponyPosition, fieldDimensions);
}

CharacterStatus getPonyStatus()
{
	return getPony().status;
}

vector<FangInfo> getFangsInfo()
{
	vector<FangInfo> fangInfos;
	glm::vec2 fieldDimensions = getFieldDimensions();
	for (Fang& fang : getFangs())
	{
		FangInfo info;
		info.position = positionModelToView(fang.position, fieldDimensions);
		info.status = fang.status;
		fangInfos.push_back(info);
	}
	return fangInfos;
}

vector<CarrotInfo> getCarrotsInfo()
{
	vector<CarrotInfo> carrotInfos;
	glm::vec2 fieldDimensions = getFieldDimensions();
	for (Carrot& carrot : getCarrots())
	{
		CarrotInfo info;
		info.position = positionModelToView(carrot.position, fieldDimensions);
		info.status = carrot.status;
		carrotInfos.push_back(info);
	}
	return carrotInfos;
}

Fermenter getFieldFermenter()
{
	return getFermenter();
}

void removeFieldFang(int index)
{
	removeFang(index);
}

bool updateFieldFangStatus(int index, CharacterStatus status)
{
	return updateFangStatus(index, status);
}

void removeFieldCarrot(int index)
{
	removeCarrot(index);
}

void harvestFieldCarrot(int index)
{
	harvestCarrot(index);
}

void ponyTakeDamage()
{
	changeHealth(-1);
}

void ponyIncreaseHealth()
{
	changeHealth(1);
}

void ponyAttack()
{
	updatePonyStatus(ATTACKING);
}

void startFermenter()
{
	updateFermenterProgress(1);
}

void emptyFermenter()
{
	updateFermenterProgress(0);
}

void updateFieldModel(float deltaTime, Action action)
{
	updateField(deltaTime, action);
}

bool isCarrotToUpdate()
{
	return getGrownCarrotToUpdate();
}

void resetCarrotToUpdate()
{
	resetGrownCarrotToUpdate();
}
