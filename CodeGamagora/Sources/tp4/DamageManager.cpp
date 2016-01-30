#include "DamageManager.h"
#include "Game.h"

DamageManager::DamageManager(void) : Item(""), timeDispach(0.45), pointDispach(10)
{
	
}

/*
DamageManager::~DamageManager(void)
{
}*/

//dans le cas où le personnage cible viens de se faire tuer
void DamageManager::addDamage(uu::u32 attacker, uu::u32 targetId){

	unsigned int i;
	for(i = 0;	i < damages.size();	i++)	//on vérifie d'abord que le personnage target n'est pas déjà dans la liste des nouveaux morts.
	{
		if(damages[i].getTargetId() == targetId)		{
			damages[i].addAttacker(attacker);
			return;
		}
	}
	//sinon, on crée la donnée pour dispaché les points dans
	damages.push_back(Damage(attacker, targetId, uu::Time::GetSynchTime()+450));	//timeDispach));	//on a l'impression que ça met 450 SECONDES quand on met la variable timeDispach

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
	for(int i = damages.size()-1;	i >= 0;	i--)	//on parcours la liste des personnes mortes depuis pas longtemps (<450ms)
	{
		if(damages[i].getTimeStamp()<=time_now){	//si on a attend le timestamp, on partage les points entre les personnes qui l'on tué.
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
		//envoyer l'info à tous les autres.
		Game::GetInstance().DispatchLocalEntityScore(attackerId, pointToDispatch);

		//interpréter l'info par soit même
		Game::GetInstance().transmetPoints(attackerId, pointToDispatch);
	}
}