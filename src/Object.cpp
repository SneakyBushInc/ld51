/**
 * @file Object.cpp
 * @date 1-Oct-2022
 */

#include "Object.h"

void Object::OnCreate()
{
    fDamage     = orxConfig_GetFloat("Damage");
    fKnockback  = orxConfig_GetFloat("Knockback");
    bSingleHit  = orxConfig_GetBool("SingleHit");
}

void Object::OnDelete()
{
}

void Object::Update(const orxCLOCK_INFO &_rstInfo)
{
}
