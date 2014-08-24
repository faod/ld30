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
				contact->setEnabled(false);
			}	
		break;
		case FOOT:
			static_cast<Character*>(fixA->GetUserData())->on_land(); // Foot -> wall
		break;
		case WALL:
			if(fixB->GetFilterData().categoryBits == FOOT)
				static_cast<Character*>(fixB->GetUserData())->on_land(); //wall->foot
		break;
		case TRIGGER:

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
			static_cast<Character*>(fixA->GetUserData())->on_jump();
		break;
		case WALL:
			if(fixB->GetFilterData().categoryBits == FOOT)
				static_cast<Character*>(fixB->GetUserData())->on_jump();
		default:
			break;
	};
}
