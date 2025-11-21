#pragma once
#include "Field.hpp"
#include <ctime>   // Needed for time()
//#include "Utilities.h"

unsigned const int fieldWidth = 1000, fieldHeight = 1000;
unsigned const int fermenterX = 100, fermenterY = 100;
unsigned int score = 0;
Pony pony;
vector<Fang> fangs;
Fermenter fermenter;
vector<Carrot> carrots;
bool grownCarrotToUpdate = false;

bool spawnFang = false, spawnCarrot = false;

unsigned int fangSpawnRate = 1;
float timeCount = 0;
unsigned int secondCount = 0;
float ponySpeed = 250.0f; // 250 units per second

const float attackCooldown = 0.7f; // times per second
const float immunityCooldown = 1.0f; // seconds
bool canTakeDamage = true;
const float carrotGrowTimer = 3.0f; // seconds
const float fangSpawnTimer = 1.5f; // seconds
const float fangDespawnTimer = 4.0f; // seconds

float fangSpeed = 100.0f; // 50 units per second


void resetField()
{
	score = 0;

	// Spawn a pony in the center of the field
	pony = Pony();
	pony.position = glm::vec2(fieldWidth / 2, fieldHeight / 2);
	pony.health = 4;
	pony.status = IDLE;
	pony.direction = RIGHT;
	pony.attackCooldown = attackCooldown;
	pony.damageCooldown = immunityCooldown;

	fangs.clear();
	carrots.clear();

	fangSpawnRate = 1;
	timeCount = 0;
	secondCount = 0;

	//fermenter = Fermenter();
	//fermenter.progress = 0;
	//fermenter.position = glm::vec2(fieldWidth / 2, fieldHeight);

	srand(time(0));
}

bool updateFangStatus(int index, CharacterStatus status)
{
	if (index >= 0 && index < fangs.size()) {
		// The Fang can only attack if it is currently moving
		if (status == ATTACKING && fangs[index].status == MOVING) {
			fangs[index].status = status;
			return true;
		}

		// The Fang dies
		if (status == DEAD) {
			if (fangs[index].status != DEAD && fangs[index].status != SPAWNING) {
				fangs[index].status = status;
				return true;
			}
		}
	}
	return false;
}

void removeFang(int index)
{
	if (index >= 0 && index < fangs.size())
		fangs.erase(fangs.begin() + index);
}

void removeCarrot(int index)
{
	if (index >= 0 && index < carrots.size())
		carrots.erase(carrots.begin() + index);
}

void harvestCarrot(int index)
{
	if (index >= 0 && index < carrots.size())
	{
		if (carrots[index].status == RIPE)
		{
			score += 10; // Increase score for harvesting a ripe carrot
			removeCarrot(index);
		}
	}
}

void updateFermenterProgress(float amount)
{
	fermenter.progress = amount;
}

bool getGrownCarrotToUpdate()
{
	return grownCarrotToUpdate;
}

void resetGrownCarrotToUpdate()
{
	grownCarrotToUpdate = false;
}

glm::vec2 getFieldDimensions()
{
	return glm::vec2(fieldWidth, fieldHeight);
}

unsigned int getScore()
{
	return score;
}

Pony getPony()
{
	return pony;
}

void changeHealth(int amount)
{
	if (amount < 0 && canTakeDamage)
		canTakeDamage = false;
	else if (amount < 0 && !canTakeDamage)
		return;
	if (pony.health > 0)
		pony.health += amount;
}

void updatePonyStatus(CharacterStatus status)
{
	pony.status = status;
}

vector<Fang> getFangs()
{
	return fangs;
}

vector<Carrot> getCarrots()
{
	return carrots;
}

Fermenter getFermenter()
{
	return fermenter;
}

void movePony(float* component, float amount, float deltaTime)
{
	*component += amount * deltaTime;
	pony.status = MOVING;
}

void movePonyX(float direction, float deltaTime)
{
	movePony(&pony.position.x, direction * ponySpeed, deltaTime);
	if (pony.position.x < 0)
		pony.position.x = 0;
	if (pony.position.x > fieldWidth)
		pony.position.x = fieldWidth;
	if (direction < 0)
		pony.direction = LEFT;
	else if (direction > 0)
		pony.direction = RIGHT;
}

void movePonyY(float direction, float deltaTime)
{
	movePony(&pony.position.y, direction * ponySpeed, deltaTime);
	if (pony.position.y < 0)
		pony.position.y = 0;
	if (pony.position.y > fieldHeight)
		pony.position.y = fieldHeight;
}

void doMovementAction(float deltaTime, Action action)
{
	switch (action)
	{
	case MOVE_LEFT:
		movePonyX(-1, deltaTime);
		break;
	case MOVE_RIGHT:
		movePonyX(1, deltaTime);
		break;
	case MOVE_UP:
		movePonyY(1, deltaTime);
		break;
	case MOVE_DOWN:
		movePonyY(-1, deltaTime);
		break;
	default:
		pony.status = IDLE;
		break;
	}
}

void updateField(float deltaTime, Action movementAction)
{
	timeCount += deltaTime;

	// Update Pony Attack Cooldown
	if (pony.status == ATTACKING)
	{
		if (pony.attackCooldown > 0)
		{
			pony.attackCooldown -= deltaTime;
		}
		else
		{
			pony.status = IDLE;
			pony.attackCooldown = attackCooldown;
		}
	}

	if (!canTakeDamage) {
		if (pony.damageCooldown > 0) {
			pony.damageCooldown -= deltaTime;
		}
		else {
			canTakeDamage = true;
			pony.damageCooldown = immunityCooldown;
		}
	}

	// Update Fangs
	for (int i = 0; i < fangs.size(); i++)
	{
		// Update Fang Attack Cooldown
		if (fangs[i].status == ATTACKING)
		{
			if (fangs[i].attackCooldown > 0)
			{
				fangs[i].attackCooldown -= deltaTime;
			}
			else
			{
				fangs[i].status = MOVING;
				fangs[i].attackCooldown = attackCooldown;
			}
		}
		// Move Fangs towards their focus
		else if (fangs[i].status == MOVING)
		{
			glm::vec2 direction;
			if (fangs[i].focus == CARROT) {

				// Fangs focus on Pony if no Carrots are present
				if (carrots.size() == 0)
				{
					fangs[i].focus = PONY;
				}
				else
				{
					// Direction towards oldest Carrot
					direction = carrots[0].position - fangs[i].position;
				}
			}
			if (fangs[i].focus == PONY)
			{
				// Direction towards Pony
				direction = pony.position - fangs[i].position;
			}
			// Move the fang
			direction = normalize(direction);
			fangs[i].position.x += direction.x * fangSpeed * deltaTime;
			fangs[i].position.y += direction.y * fangSpeed * deltaTime;
		}
		// Update spawn timers - less efficient than counting seconds but makes a cooler progress bar
		else if (fangs[i].status == SPAWNING)
		{
			if (fangs[i].spawnTimer > 0)
			{
				fangs[i].spawnTimer -= deltaTime;
			}
			else
				fangs[i].status = MOVING;
		}
		else if (fangs[i].status == DEAD)
		{
			if (fangs[i].despawnTimer > 0)
			{
				fangs[i].despawnTimer -= deltaTime;
			}
			else
				removeFang(i);
		}
	}

	// Update Carrots
	for (int i = 0; i < carrots.size(); i++)
	{
		if (carrots[i].status == SEED)
		{
			if (carrots[i].growTimer > 0)
			{
				carrots[i].growTimer -= deltaTime;
			}
			else {
				carrots[i].status = GROWING;
				carrots[i].growTimer = carrotGrowTimer; // Reset grow timer for next stage
			}
		}
		else if (carrots[i].status == GROWING)
		{
			if (carrots[i].growTimer > 0)
			{
				carrots[i].growTimer -= deltaTime;
			}
			else {
				grownCarrotToUpdate = true;
				carrots[i].status = RIPE;
			}
		}
	}

	// Update Fermenter
	if (fermenter.progress < 100 && fermenter.progress > 0)
	{
		fermenter.progress += deltaTime / 100; // 100 ms to increase progress by 1
		if (fermenter.progress > 100)
			fermenter.progress = 100;
	}

	// Time-based events
	for (; timeCount >= 1; timeCount--)
	{
		secondCount += 1;
		spawnCarrot = true;
		spawnFang = true;

		// Spawn new Fangs every 10 seconds
		if (secondCount % 10 == 0 && spawnFang)
		{
			for (int i = 0; i < fangSpawnRate; i++)
			{
				Fang newFang;
				newFang.position = glm::vec2(rand() % fieldWidth, rand() % fieldHeight);
				newFang.status = SPAWNING;
				newFang.spawnTimer = fangSpawnTimer;
				newFang.despawnTimer = fangDespawnTimer;
				newFang.focus = rand() % 2 == 0 || carrots.size() == 0 ? PONY : CARROT;
				newFang.attackCooldown = attackCooldown;
				fangs.push_back(newFang);
			}
			spawnFang = false;
		}

		// Increase spawn rate every 60 seconds
		if (secondCount >= 60)
		{
			fangSpawnRate++;
			secondCount %= 60;
		}

		// Spawn Carrots every 5 seconds
		if (secondCount % 5 == 0 && spawnCarrot)
		{
			Carrot newCarrot;
			newCarrot.position = glm::vec2(rand() % fieldWidth, rand() % fieldHeight);
			newCarrot.status = SEED;
			newCarrot.growTimer = carrotGrowTimer;
			carrots.push_back(newCarrot);
			spawnCarrot = false;
		}
	}

	if (pony.status != ATTACKING)
		doMovementAction(deltaTime, movementAction);
}

