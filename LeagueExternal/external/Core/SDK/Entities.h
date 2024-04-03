#include "structs.h"
#include "activeplayer.h"

class Entity
{
public:
	Entity() {};
	Entity(vector2 origin) { this->origin = origin; };
	Entity(int id, vector2 origin) { this->id = id; this->origin = origin; };

public:
	int id = 0;
	vector2 origin{ 0,0 };

private:
	vector2 root{ 0,0 };
	vector2 root_offset{ 0,0 };
};

struct AbilityRange {
	vector2 vScreenRange{ 0,0 };
	vector2 vScreenOffset{ 0,0 };
	float Modifer = 0.0f;
};

class LocalPlayer : public Entity
{
public:
	LocalPlayer() : Entity() {};
	LocalPlayer(vector2 origin) : Entity(origin) {};

public:
	championStats stats;
	vector2 vScreenAARange{ 0,0 };
	bool bQAbility;
	vector2 vQRange{ 0,0 };
	bool bWAbility;
	vector2 vWRange{ 0,0 };
	bool bEAbility;
	vector2 vERange{ 0,0 };
	bool bRAbility;
	vector2 vRRange{ 0,0 };
public:
	bool IsInside(Entity e, vector2 vRange, float threshold = 1.0f) {
		float r = (e.origin.x - origin.x) * (e.origin.x - origin.x) / (vRange.x * vRange.x)
			+ (e.origin.y - (origin.y + 95)) * (e.origin.y - (origin.y + 95)) / (vRange.y * vRange.y);
		if (r <= threshold)
			return true;
		return false;
	}
};


class Enemy : public Entity
{
public:
	Enemy() : Entity() {};
	Enemy(int id, vector2 origin) : Entity(id, origin) {};

public:
	vector2 direction{ 0,0 };
	float magnitude = 0.0f;
	float speed = 0.0f;

public:
	vector2 Predict(float fPredictTime) {
		if (magnitude == 0.0f || magnitude > 15.0f)
			return origin;
		return origin + ((direction / magnitude) * speed * fPredictTime);
	}
};

class Minion : public Entity
{
public:
	Minion() : Entity() {};
	Minion(vector2 origin, float health) : Entity(origin) {
		this->health = health;
	};

public:
	float health;
};