#pragma once

using namespace std;

#include <glm/glm.hpp>
#include <vector>
#include "struttureModel.hpp"


/*!
 * @brief Returns the dimensions of the field.
 * @return A glm::vec2 containing the width and height of the field.
 */
glm::vec2 getFieldDimensions();

/*! 
 * @brief Returns the current score.
 */
unsigned int getScore();

/*!
 * @brief Returns the current Pony on the field.
 * @return The Pony.
 */
Pony getPony();

/*!
 * @brief Returns the Fangs on the field.
 * @return A vector of Fang.
 */
vector<Fang> getFangs();

/*!
 * @brief Returns the Carrots on the field.
 * @return A vector of Carrot.
 */
vector<Carrot> getCarrots();

/*!
 * @brief Returns the Fermenter on the field.
 * @return The Fermenter.
*/
Fermenter getFermenter();

/*!
* @brief The main update method for the field model.
* Updates the Fangs statuses and positions, spawns new Fangs and Carrots, updates the Fermenter progress, and processes the Pony's actions.
* @param deltaTime: The time elapsed since the last update in milliseconds.
* @param action: The action to be performed by the Pony character.
*/
void updateField(float deltaTime, Action action);

/*!
 * @brief Initializes the field to a brand new state.
 */
void resetField();

/*!
 * @brief Increases the pony's health by the specified amount.
 */
void changeHealth(int amount);

/**
* @brief Updates the status of the Pony.
 * @param status: The new status to set for the Pony.
*/
void updatePonyStatus(CharacterStatus status);

/*!
 * @brief Updates the status of a Fang of specified index.
 * @param index: The index of the Fang to be updated.
 * @param status: The new status to set for the Fang.
 * @return True if the update was successful, false otherwise.
 */
bool updateFangStatus(int index, CharacterStatus status);

/*!
 * @brief Removes a Fang from the field by its index.
 * @param index: The index of the Fang to be removed.
 */
void removeFang(int index);

/*!
 * @brief Removes a Carrot from the field by its index.
 * @param index: The index of the Carrot to be removed.
 */
void removeCarrot(int index);

/*!
 * @brief Harvests a Carrot from the field by its index. Removes it from the field and increases the score.
 * @param index: The index of the Carrot to be harvested.
 */
void harvestCarrot(int index);

/*!
* @brief Sets the progress of the Fermenter to the specified amount.
* @param amount: The new progress amount (0 to 100).
*/
void updateFermenterProgress(float amount);

/**
* @brief Checks if a carrot grew before the last carrot update.
* @return True if a carrot grew before the last carrot update.
*/
bool getGrownCarrotToUpdate();

/**
* @brief Resets the grownCarrotToUpdate to false.
*/
void resetGrownCarrotToUpdate();