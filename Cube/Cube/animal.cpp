#include "animal.h"


Animal::Animal()
{
	m_dimension = Vector3<float>(1.5, 2.3, 2.4);
	m_AttackRange = 5.2;
	m_AttackSpeed = 1.1;
	m_AttackDamage = 10;
	m_VerticalRot = 0;
	m_Armor = 0.5;
	m_HorizontalRot = rand() % 180;
	m_isAlive = false;
	m_timeNextTarget = 3;

}

Animal::~Animal()
{
}

void Animal::Move(World &world)
{
	if (m_isAlive)
	{
		m_vitesse.x = 0.05;
		m_vitesse.z = 0.05;

		if (m_ClockTarget.getElapsedTime().asSeconds() < m_timeNextTarget)
		{
			Vector3<float> deplacementVector = Vector3<float>(sin(m_HorizontalRot / 180 * PI), 0.f, cos(m_HorizontalRot / 180 * PI));
			deplacementVector.Normalize();

			//Avance en x
			m_pos.x += deplacementVector.x * m_vitesse.x;
			if (CheckCollision(world))
			{
				m_pos.x -= deplacementVector.x * m_vitesse.x;
				Jump();
			}
			//En z
			m_pos.z += deplacementVector.z * m_vitesse.z;
			if (CheckCollision(world))
			{
				m_pos.z -= deplacementVector.z * m_vitesse.z;
				Jump();
			}
		}
		else
		{
			m_HorizontalRot += rand() % 200 - 100;
			m_timeNextTarget = rand() % 10;
			m_ClockTarget.restart();
		}



		//Chute
		m_pos.y -= m_vitesse.y;

		//Si collision
		if (CheckCollision(world))
		{

			//Si on a touche le sol 
			if (m_vitesse.y > 0)
				m_isInAir = false;

			//annule
			m_pos.y += m_vitesse.y;
			m_vitesse.y = 0;
		}
		else
			m_isInAir = true;

		//Acceleration
		m_vitesse.y += 0.013f;

	}
}

void Animal::Draw(Model3d &model) const
{
	if (m_isAlive)
	{
		if (m_ClockAnimationDmg.getElapsedTime().asSeconds() < 0.1)
			model.Render(m_pos.x, m_pos.y, m_pos.z, m_HorizontalRot, m_VerticalRot, 1.f, 0.5f, 0.5f);
		else
			model.Render(m_pos.x, m_pos.y, m_pos.z, m_HorizontalRot, m_VerticalRot, 1.f, 1.f, 1.f);

		if (false)
		{
			glPushMatrix();

			glTranslatef(m_pos.x, m_pos.y, m_pos.z);
			glRotatef(m_HorizontalRot, 0.f, 1.f, 0.f);

			glColor3f(1.f, 0.0f, 0.f);

			glBegin(GL_QUADS);

			glVertex3f(-m_dimension.x / 2, m_dimension.y, m_dimension.z / 2);
			glVertex3f(m_dimension.x / 2, m_dimension.y, m_dimension.z / 2);
			glVertex3f(m_dimension.x / 2, m_dimension.y, -m_dimension.z / 2);
			glVertex3f(-m_dimension.x / 2, m_dimension.y, -m_dimension.z / 2);

			glVertex3f(-m_dimension.x / 2, 0, m_dimension.z / 2);
			glVertex3f(-m_dimension.x / 2, 0, -m_dimension.z / 2);
			glVertex3f(m_dimension.x / 2, 0, -m_dimension.z / 2);
			glVertex3f(m_dimension.x / 2, 0, m_dimension.z / 2);

			glVertex3f(-m_dimension.x / 2, m_dimension.y, m_dimension.z / 2);
			glVertex3f(-m_dimension.x / 2, m_dimension.y, -m_dimension.z / 2);
			glVertex3f(m_dimension.x / 2, m_dimension.y, -m_dimension.z / 2);
			glVertex3f(m_dimension.x / 2, m_dimension.y, m_dimension.z / 2);

			glVertex3f(-m_dimension.x / 2, 0, m_dimension.z / 2);
			glVertex3f(m_dimension.x / 2, 0, m_dimension.z / 2);
			glVertex3f(m_dimension.x / 2, 0, -m_dimension.z / 2);
			glVertex3f(-m_dimension.x / 2, 0, -m_dimension.z / 2);

			glEnd();
			glPopMatrix();
		}
	}
}

void Animal::GetDamage(float damage)
{
	m_ClockAnimationDmg.restart();
	Jump();
	if (rand() % 100 > 66)
		Sound::Play(Sound::FLESH_IMPACT);
	Character::GetDamage(damage);
}