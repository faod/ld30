#include "ContactListener.hpp"

#include "Game.hpp"
#include "failure.hpp"

void ContactListener::BeginContact(b2Contact *contact)
{
	b2Fixture* fixA = contact->GetFixtureA();
	b2Fixture* fixB = contact->GetFixtureB();


	switch(fixA->GetFilterData().categoryBits)
	{
		case PLAYER:
			if(fixB->GetFilterData().categoryBits == TRIGGER)
			{
				
			}
			else if(fixB->GetFilterData().categoryBits == MONSTER)
			{
				
			}	
		break;
		case FOOT:
			if(fixB->GetFilterData().categoryBits == WALL)
				static_cast<Character*>(fixA->GetUserData())->on_land(); // Foot -> wall
			else if(fixB->GetFilterData().categoryBits == MONSTER)
				static_cast<Player*>(fixA->GetUserData())->onMonsterContact();//foot->monster
		break;
		case WALL:
			if(fixB->GetFilterData().categoryBits == FOOT)
				static_cast<Character*>(fixB->GetUserData())->on_land(); //wall->foot
		break;
		case TRIGGER:

		break;
		case MONSTER:
			if(fixB->GetFilterData().categoryBits == FOOT)
				static_cast<Player*>(fixB->GetUserData())->onMonsterContact(); //monster->foot
		break;
		default:
			break;
	};


}

void ContactListener::EndContact(b2Contact *contact)
{
	b2Fixture* fixA = contact->GetFixtureA();
	b2Fixture* fixB = contact->GetFixtureB();

	switch(fixA->GetFilterData().categoryBits)
	{
		case FOOT:
			if(fixB->GetFilterData().categoryBits == WALL)
				static_cast<Character*>(fixA->GetUserData())->on_jump(); //foot -> wall
			else if(fixB->GetFilterData().categoryBits == MONSTER)
				static_cast<Player*>(fixA->GetUserData())->onMonsterSeparation(); //foot -> monster
		break;
		case WALL:
			if(fixB->GetFilterData().categoryBits == FOOT)
				static_cast<Character*>(fixB->GetUserData())->on_jump(); //wall -> foot
		break;
		case MONSTER:
			if(fixB->GetFilterData().categoryBits == FOOT)
				static_cast<Player*>(fixB->GetUserData())->onMonsterSeparation(); //monster->foot
		break;
		default:
			break;
	};
}
