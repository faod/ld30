#pragma once
#ifndef DEF_CONTACTL
#define DEF_CONTACTL

#include <Box2D/Box2D.h>

class ContactListener : public b2ContactListener
{
	virtual void BeginContact(b2Contact *contact);
	virtual void EndContact(b2Contact *contact);
};


#endif
