/**
 * @file Character.cpp
 * @date 1-Oct-2022
 */

#include "Character.h"

void Character::Die()
{
    bDead = orxTRUE;
    SetHealth(orxFLOAT_0);
    SetAnim("Death");
}

void Character::Revive()
{
    bDead = orxFALSE;
    PushConfigSection();
    SetHealth(orxConfig_GetFloat("ReviveHealth"));
    PopConfigSection();
    SetAnim("Revive");
}

void Character::SetHealth(orxFLOAT _fHealth)
{
    fHealth = orxCLAMP(_fHealth, orxFLOAT_0, fMaxHealth);
}

void Character::OnCreate()
{
    // Init variables
    Object::OnCreate();
    orxConfig_SetBool("IsCharacter", orxTRUE);
    fHealth = fMaxHealth = orxConfig_GetFloat("Health");

    // Enable our input set
    orxInput_EnableSet(orxConfig_GetCurrentSection(), orxTRUE);
}

void Character::OnDelete()
{
    Object::OnDelete();
}

void Character::Update(const orxCLOCK_INFO &_rstInfo)
{
    if(!bDead)
    {
        // Push config section
        PushConfigSection();

        // Select our input set
        const orxSTRING zInputSet = orxInput_GetCurrentSet();
        orxInput_SelectSet(orxConfig_GetCurrentSection());

        // Update movement
        const orxSTRING zAnim = orxNULL;
        orxVECTOR vMove = {orxInput_GetValue("MoveRight") - orxInput_GetValue("MoveLeft"), orxInput_GetValue("MoveDown") - orxInput_GetValue("MoveUp")};
        if(!orxVector_IsNull(&vMove))
        {
            zAnim = "Walk";
            orxVector_Mulf(&vMove, &vMove, orxConfig_GetFloat("WalkSpeed"));
        }
        SetSpeed(vMove);

        // Update anim
        SetAnim(zAnim, orxFALSE, orxTRUE);

        // Restore previous input set
        orxInput_SelectSet(zInputSet);

        // Pop config section
        PopConfigSection();
    }

    Object::Update(_rstInfo);
}
