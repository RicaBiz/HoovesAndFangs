#pragma once
#include "strutture.h"
#include "lib.h"
#include "struttureModel.hpp"
vec2 randomPosition(int width, int height);
void usaShaderConUniforms(unsigned int shader_programId, glm::mat4& projection, mat4& model, float currentTime, vec2 resolution);
void findBB(Figura* fig);
void updateBB(Figura* fig);
vec4 findSideBB(Figura* fig, Direction ponyDirection);
bool checkCollision(vec4 obj1Min, vec4 obj1Max, vec4 obj2Min, vec4 obj2Max);
bool checkCollision(Figura obj1, Figura obj2);
vec3 positionModelToView(glm::vec2 position, glm::vec2 fieldDimensions);
void loadHermite(FiguraHermite* fig, const char* fileName, bool withColor = true);
void findHermiteBB(FiguraHermite* fig, int curvaBBindex);
void goToGameScreen();
void leaveGameScreen(Screen newScreen);
void changeScreen(Screen newScreen);
void loadScoreboard();
void saveScoreboard();
void resetScoreboard();