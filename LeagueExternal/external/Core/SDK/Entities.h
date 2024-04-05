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



class LocalPlayer : public Entity
{
public:
	LocalPlayer() : Entity() {};
	LocalPlayer(vector2 origin) : Entity(origin) {};

public:
	championStats stats;
	bool bQAbility;
	bool bWAbility;
	bool bEAbility;
	bool bRAbility;
	vector2 vScreenAARange{ 0,0 };
	vector2 vScreenQRange{ 0,0 };
	vector2 vScreenWRange{ 0,0 };
	vector2 vScreenERange{ 0,0 };
	vector2 vScreenRRange{ 0,0 };
public:
	bool IsInside(Entity e, vector2 vRange, float offset, float threshold = 1.0f) {
		float r = (e.origin.x - origin.x) * (e.origin.x - origin.x) / (vRange.x * vRange.x)
			+ (e.origin.y - (origin.y + offset)) * (e.origin.y - (origin.y+ offset)) / (vRange.y * vRange.y);
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