#pragma once

#include <vector>
#include "types.h"

class Damage
{
private :
	std::vector<uu::u32> attackersId;
	uu::u32 targetId;
	
	time_t timeStamp;

public:
	//Damage(void);
	Damage(uu::u32 attacker, uu::u32 targetId, time_t timeStamp);
	//~Damage(void);

	void addAttacker(uu::u32 attackerId);

	uu::u32 getTargetId() const {		return targetId;	}
	std::vector<uu::u32> getAttackersId() const {		return attackersId;	}
	time_t getTimeStamp() const {		return timeStamp;	}

};

