#include "DamageManager.h"
#include "Game.h"

DamageManager::DamageManager(void) : Item(""), timeDispach(450), pointDispach(10);
{
	
}

/*
DamageManager::~DamageManager(void)
{
}*/

//dans le cas où le personnage cible viens de se faire tuer
void DamageManager::addDamage(uu::u32 attacker, uu::u32 targetId){

	int i;
	for(i = 0;	i < damages.size();	i++)	//on vérifie d'abord que le personnage target n'est pas déjà dans la liste des nouveaux morts.
	{
		if(damages[i].getTargetId() == targetId)		{
			damages[i].addAttacker(attacker);
			return;
		}
	}
	if(i == damages.size())		//sinon, on crée la donnée pour dispaché les points dans
		damages.push_back(Damage(attacker, targetId, uu::Time::GetSynchTime()+timeDispach));

}

//dans le cas où le personnage cible est déjà mort, on
void DamageManager::addAttackerInDamage(uu::u32 attacker, uu::u32 targetId){
	for(Damage& dam: damages)
	{
		if(dam.getTargetId() == targetId)		{
			dam.addAttacker(attacker);
			return;
		}
	}
	//si il n'est pas dans la liste "damages", alors il est mort depuis trop longtemps et les points ont déjà été partagés.
	//cela peut aussi dire que l'on a attaqué à l'endroit où se tenait le cadavre il y a un plus de 10 minutes.
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
	
	float pointToDispatch = pointDispach / damage.getAttackersId().size();

	for(uu::u32& attackerId : damage.getAttackersId()){
		//interpréter l'info par soit même
		Game::GetInstance().transmetPoints(attackerId, pointToDispatch);

		//envoyer l'info à tous les autres.
		Game::GetInstance().DispatchLocalEntityScore(attackerId, pointToDispatch);
	}
}