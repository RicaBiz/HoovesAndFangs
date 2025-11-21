#pragma once

using namespace std;

#include <glm/glm.hpp>
#include <vector>

enum Direction
{
	LEFT = 0,
	RIGHT
};

enum CharacterStatus
{
	IDLE = 0,
	MOVING,
	ATTACKING,
	SPAWNING,
	DEAD
};

enum CropStatus {
	SEED = 0,
	GROWING,
	RIPE
};

enum Action {
	MOVE_LEFT = 0,
	MOVE_RIGHT,
	MOVE_UP,
	MOVE_DOWN,
	HARVEST,
	PICK_UP,
	NONE
};

enum Focus {
	PONY = 0,
	CARROT
};

typedef struct {
	glm::vec2 position;
} Element;

struct CharacterElement : Element {
	CharacterStatus status;
	float attackCooldown;
};

const glm::vec2 ponyDimensions = glm::vec2(50, 50);
struct Pony : CharacterElement {
	Direction direction;
	unsigned int health;
	float damageCooldown;
};

const glm::vec2 fangDimensions = glm::vec2(30, 30);
struct Fang : CharacterElement {
	Focus focus;
	float spawnTimer;
	float despawnTimer;
};

const glm::vec2 carrotDimensions = glm::vec2(15, 15);
struct Carrot : Element {
	CropStatus status;
	float growTimer;
};

const glm::vec2 fermenterDimensions = glm::vec2(60, 80);
struct Fermenter : Element {
	float progress; // 0 to 100
};