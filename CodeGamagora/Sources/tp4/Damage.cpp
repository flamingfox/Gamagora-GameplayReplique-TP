#include "Damage.h"

Damage::Damage(uu::u32 attackerId, uu::u32 targetId, time_t timeStamp):
	targetId(targetId),	timeStamp(timeStamp)
{
	attackersId.push_back(attackerId);

}

void Damage::addAttacker(uu::u32 attackerId)
{
	attackersId.push_back(attackerId);
}