#include "GSTEquipmentAttributeSet.h"

UGSTEquipmentAttributeSet::UGSTEquipmentAttributeSet()
{
	InitFireRate(1.0f);          // Default fire rate: 1 shot per second
	InitDamage(10.0f);           // Default base damage: 10
	InitCooldown(3.0f);          // Default cooldown: 3 seconds
	InitDamageResistance(0.0f);  // Default: No resistance (full damage taken)
	InitBurrowMaxTime(5.0f);     // Default: 5 seconds of burrow time
	InitHarpoonDamage(10.f);
	InitCannonDamage(25.0f);
	InitBallLightningDamage(30.f);
}
