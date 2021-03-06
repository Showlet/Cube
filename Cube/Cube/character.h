#ifndef CHARACTER_H__
#define CHARACTER_H__

#include "define.h"
#include "vector3.h"
#include "world.h"
#include "sound.h"
#include "experience.h"

class Character
{
public:
	Character();
	~Character();

	//Action
	void Spawn(World &world, int x, int z);
	virtual void Move(World &world);
	bool CheckCollision(World &world) const;
	bool CheckCollision(Character & character) const;
	void Draw() const;
	bool Attack(Character * character, float damage);
	virtual bool Attack(Character * character);
	virtual bool GetDamage(float damage, bool ignoreArmor, bool godMode, Character* killer, Sound::ListeSons son = Sound::ListeSons::HURT, bool playonce = false);
	void Jump();
	void DeathCheck();

	//Set
	void SetName(std::string name);

	//Get   
	const Vector3<float>& GetPosition() const;
	const Vector3<float>& GetDimension() const;
	float GetHP() const;
	float GetHorizontalRotation() const;
	float GetVerticalRotation() const;
	int GetAttackRange() const;
	float GetAttackSpeed() const;
	float GetArmor() const;
	float GetAttackDamage() const;
	const std::string& GetName() const;
	bool GetisAlive() const;
	bool GetisInAir() const;
	void CheckBlock(World &world);

	virtual Experience* GetXp(){ return nullptr; }
	virtual int XpGainOnDeath() { return 20; }

protected:

	//Positionnement
	Vector3<float> m_pos;
	Vector3<float> m_dimension;
	Vector3<float> m_vitesse;
	float m_HorizontalRot;
	float m_VerticalRot;
	World* m_world;

	//Stat
	std::string m_Name;
	float m_health;
	float m_maxHealth;
	int m_AttackRange;
	float m_AttackSpeed;
	float m_Armor;
	float m_AttackDamage;
	float m_deathSink;

	//�tat
	bool m_isInAir;
	bool m_isAlive;
	bool m_isDying;
	//Trampoline
	int m_nbsauttrampoline = 0;
	float multiplicateur = 1.01f;
	//Timer
	sf::Clock m_cooldownAttackTimer;
	sf::Clock m_deathTick;
private:
	



};

#endif // MONSTER_H__

