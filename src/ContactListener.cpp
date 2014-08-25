#include "ContactListener.hpp"

#include "Game.hpp"
#include "failure.hpp"


static void onFootWall(b2Fixture* foot, b2Fixture* /*wall*/)
{
	static_cast<Character*>(foot->GetUserData())->on_land();
}
static void outFootWall(b2Fixture* foot, b2Fixture* /*wall*/)
{
	static_cast<Character*>(foot->GetUserData())->on_jump();
}

static void onFootMonster(b2Fixture* foot, b2Fixture* /*monster*/)
{
	static_cast<Player*>(foot->GetUserData())->onMonsterContact();
}
static void outFootMonster(b2Fixture* foot, b2Fixture* /*monster*/)
{
	static_cast<Player*>(foot->GetUserData())->onMonsterSeparation();
}
static void onPlayerTrigger(b2Fixture* player, b2Fixture *trigger)
{
	const char* str = static_cast<char*>(trigger->GetUserData());

	if(std::string("kill") == str) //kill trigger
	{
		static_cast<Character*>(player->GetUserData())->kill();
	}
	else if(std::string("secret") == str) //heal player
	{
		static_cast<Character*>(player->GetUserData())->damage(-100);
	}	
	else if(std::string("finish") == str) 
	{
		static_cast<Player*>(player->GetUserData())->finish();
	}
}
static void onSwordHit(b2Fixture* sword, b2Fixture *character)
{
	Character* att = static_cast<Character*>(sword->GetUserData());
	Character* def = static_cast<Character*>(character->GetUserData());
	att->attack(*def);
}

void ContactListener::BeginContact(b2Contact *contact)
{
	b2Fixture* fixA = contact->GetFixtureA();
	b2Fixture* fixB = contact->GetFixtureB();


	switch(fixA->GetFilterData().categoryBits)
	{
		case PLAYER:
			if(fixB->GetFilterData().categoryBits == TRIGGER)
				onPlayerTrigger(fixA, fixB);
			else if(fixB->GetFilterData().categoryBits == SWORD)
				onSwordHit(fixB, fixA);
		break;
		case FOOT:
			if(fixB->GetFilterData().categoryBits == WALL)
				onFootWall(fixA, fixB);
			else if(fixB->GetFilterData().categoryBits == MONSTER)
				onFootMonster(fixA, fixB);
		break;
		case WALL:
			if(fixB->GetFilterData().categoryBits == FOOT)
				onFootWall(fixB, fixA);	
		break;
		case TRIGGER:
			if(fixB->GetFilterData().categoryBits == PLAYER)
				onPlayerTrigger(fixB, fixA);
			else if(fixB->GetFilterData().categoryBits == MONSTER)
				onPlayerTrigger(fixB, fixA);
		break;
		case MONSTER:
			if(fixB->GetFilterData().categoryBits == FOOT)
				onFootMonster(fixB, fixA);
			else if(fixB->GetFilterData().categoryBits == SWORD)
				onSwordHit(fixB, fixA);
			else if(fixB->GetFilterData().categoryBits == TRIGGER)
				onPlayerTrigger(fixA, fixB);
		break;
		case SWORD:
			onSwordHit(fixA, fixB);
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
				outFootWall(fixA, fixB);
			else if(fixB->GetFilterData().categoryBits == MONSTER)
				outFootMonster(fixA, fixB);
		break;
		case WALL:
			if(fixB->GetFilterData().categoryBits == FOOT)
				outFootWall(fixB, fixA);
		break;
		case MONSTER:
			if(fixB->GetFilterData().categoryBits == FOOT)
				outFootMonster(fixB, fixA);
		break;
		default:
			break;
	};
}
