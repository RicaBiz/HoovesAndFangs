#pragma once
#include <glm/glm.hpp>
#include "Field.hpp"

using namespace glm;

struct FangInfo {
	glm::vec3 position;
	CharacterStatus status;
};

struct CarrotInfo {
	glm::vec3 position;
	CropStatus status;
};

/**
* @brief Initializes the field model to a new state.
*/
void initializeFieldModel();

/*!
* @brief Returns the current score of the Model.
*
* @return The current score integer.
*/
int getFieldScore();

/*!
* @brief Returns the current health of the Pony.
*
* @return The current health integer.
*/
int getHealth();

/**
* Returns the current position of the Pony.
*
* @return The current position as a glm::vec3.
*/
glm::vec3 getPonyPosition();

/**
* @brief Returns the current status of the Pony.
*
* @return The current CharacterStatus.
*/
CharacterStatus getPonyStatus();

/**
* @brief Returns position and status information about all Fangs on the field.
*
* @return A vector of FangInfo structures.
*/
vector<FangInfo> getFangsInfo();

/**
* @brief Returns all Carrots on the field.
* @return A vector of Carrot.
*/
vector<CarrotInfo> getCarrotsInfo();

/**
* @brief Returns the Fermenter on the field.
* @return The Fermenter.
*/
Fermenter getFieldFermenter();

/**
* @brief Removes a Fang from the field at the specified index.
* @param index The index of the Fang to remove.
*/
void removeFieldFang(int index);

/**
* @brief Updates the status of a Fang at the specified index.
* @param index The index of the Fang to update.
* @param status The new status to set for the Fang.
* @return True if the update was successful, false otherwise.
*/
bool updateFieldFangStatus(int index, CharacterStatus status);

/**
* @brief Removes a Carrot from the field at the specified index.
* @param index The index of the Carrot to remove.
*/
void removeFieldCarrot(int index);

/**
* @brief Harvests a Carrot from the field at the specified index.
* @param index The index of the Carrot to harvest.
*/
void harvestFieldCarrot(int index);

/**
* @brief Takes damage for the Pony, reducing its health by 1.
*/
void ponyTakeDamage();

/**
* @brief Increases the Pony's health by 1.
*/
void ponyIncreaseHealth();

/**
* @brief Makes the Pony perform an attack action.
*/
void ponyAttack();

/**
* @brief Starts the Fermenter by setting its progress to 1.
*/
void startFermenter();

/**
* @brief Empties the Fermenter by setting its progress to 0.
*/
void emptyFermenter();

/**
* @brief The main update method for the field model.
*/
void updateFieldModel(float deltaTime, Action action);

/**
* @brief Checks if carrots have grown after the last carrot update.
* @return True if at least one carrot grew after the last carrot update.
*/
bool isCarrotToUpdate();

/**
* @brief Resets the grownCarrotToUpdate to false.
*/
void resetCarrotToUpdate();