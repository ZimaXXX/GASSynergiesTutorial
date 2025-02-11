#include "GSTEquipmentAttributeSet.h"

UGSTEquipmentAttributeSet::UGSTEquipmentAttributeSet()
{
	//General
	InitDamageResistance(0.0f);  // Default: No resistance (full damage taken)
	
	//Cannon
	InitCannonFireRate(1.0f);
	InitCannonDamage(10.0f);

	//Burrow
	InitBurrowMaxTime(5.0f);    
	InitBurrowCooldown(3.0f);

	//BallLightning
	InitBallLightningDamage(30.f);
	InitBallLightningCooldown(3.f);

	//Harpoon
	InitHarpoonDamage(10.f);
	InitHarpoonRange(1000.0f);
	InitHarpoonHardness(1.f);
	InitHarpoonCooldown(3.f);

	//Exploding Barrel
	InitExplodingBarrelCooldown(3.f);
	InitExplodingBarrelDamage(30.f);
}
