#pragma once

#include <vector>
#include "Damage.h"
#include "Item.h"
//#include "Game.h"
#include "Time.h"
//#include "Singleton.h"

class DamageManager	: public Item
{
private :
	std::vector<Damage> damages;

	float timeDispach;	//temps entre le moment où on reçoit l'information de la mort d'un Character et le moment où les points sont partagés.
	float pointDispach;	//points à partager au moment du partage pour une mort.

public:	
	DamageManager(void);
	//~DamageManager(void);

	void addDamage(uu::u32 attacker, uu::u32 targetId);
	void addAttackerInDamage(uu::u32 attacker, uu::u32 targetId);
	
	bool Update(time_t time_now);


private :
	void makeScoreDispatch(const Damage& damage) const;
};

