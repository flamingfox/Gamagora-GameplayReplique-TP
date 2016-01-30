#include "DamageManager.h"
#include "Game.h"

DamageManager::DamageManager(void) : Item(""), timeDispach(0.45), pointDispach(10)
{
	
}

/*
DamageManager::~DamageManager(void)
{
}*/

//dans le cas o� le personnage cible viens de se faire tuer
void DamageManager::addDamage(uu::u32 attacker, uu::u32 targetId){

	unsigned int i;
	for(i = 0;	i < damages.size();	i++)	//on v�rifie d'abord que le personnage target n'est pas d�j� dans la liste des nouveaux morts.
	{
		if(damages[i].getTargetId() == targetId)		{
			damages[i].addAttacker(attacker);
			return;
		}
	}
	//sinon, on cr�e la donn�e pour dispach� les points dans
	damages.push_back(Damage(attacker, targetId, uu::Time::GetSynchTime()+450));	//timeDispach));	//on a l'impression que �a met 450 SECONDES quand on met la variable timeDispach

}

//dans le cas o� le personnage cible est d�j� mort, on
void DamageManager::addAttackerInDamage(uu::u32 attacker, uu::u32 targetId){
	for(Damage& dam: damages)
	{
		if(dam.getTargetId() == targetId)		{
			dam.addAttacker(attacker);
			return;
		}
	}
	//si il n'est pas dans la liste "damages", alors il est mort depuis trop longtemps et les points ont d�j� �t� partag�s.
	//cela peut aussi dire que l'on a attaqu� � l'endroit o� se tenait le cadavre il y a un plus de 10 minutes.
}

bool DamageManager::Update(time_t time_now)
{
	for(int i = damages.size()-1;	i >= 0;	i--)	//on parcours la liste des personnes mortes depuis pas longtemps (<450ms)
	{
		if(damages[i].getTimeStamp()<=time_now){	//si on a attend le timestamp, on partage les points entre les personnes qui l'on tu�.
			makeScoreDispatch(damages[i]);
			//suppresion damage puisque d�cision prise
			damages.erase(damages.begin()+i);
		}
	}
	return true;
}

void DamageManager::makeScoreDispatch(const Damage& damage) const{
	
	float pointToDispatch = pointDispach / damage.getAttackersId().size();

	for(uu::u32& attackerId : damage.getAttackersId()){
		//envoyer l'info � tous les autres.
		Game::GetInstance().DispatchLocalEntityScore(attackerId, pointToDispatch);

		//interpr�ter l'info par soit m�me
		Game::GetInstance().transmetPoints(attackerId, pointToDispatch);
	}
}