#include "DamageManager.h"
#include "Game.h"

DamageManager::DamageManager(void) : Item("")
{
	
}

/*
DamageManager::~DamageManager(void)
{
}*/

//dans le cas où le personnage cible viens de se faire tuer
void DamageManager::addDamage(uu::u32 attacker, uu::u32 targetId){

	damages.push_back(Damage(attacker, targetId, uu::Time::GetSynchTime()+450));

}

//dans le cas où le personnage cible est déjà mort, 
void DamageManager::addAttackerInDamage(uu::u32 attacker, uu::u32 targetId){
	for(Damage& dam: damages)
	{
		if(dam.getTargetId() == targetId)		{
			dam.addAttacker(attacker);
			return;
		}
	}
}

bool DamageManager::Update(time_t time_now)
{
	for(int i = damages.size()-1;	i >= 0;	i--)
	{
		if(damages[i].getTimeStamp()<=time_now){
			makeScoreDispatch(damages[i]);
			//suppresion damage puisque décision prise
			damages.erase(damages.begin()+i);
		}
	}
	return true;
}

void DamageManager::makeScoreDispatch(const Damage& damage) const{
	
	float pointToDispatch = 10.f / damage.getAttackersId().size();

	for(uu::u32& attackerId : damage.getAttackersId()){
		//interpréter l'info par soit même
		Game::GetInstance().transmetPoints(attackerId, pointToDispatch);

		//envoyer l'info à tous les autres.
		Game::GetInstance().DispatchLocalEntityScore(attackerId, pointToDispatch);
	}
}