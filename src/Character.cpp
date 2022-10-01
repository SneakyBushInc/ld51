/**
 * @file Character.cpp
 * @date 1-Oct-2022
 */

#include "Character.h"

void Character::Die()
{
}

void Character::OnCreate()
{
    // Init variables
    Object::OnCreate();
    orxConfig_SetBool("IsCharacter", orxTRUE);

    // Enable our input set
    orxInput_EnableSet(orxConfig_GetCurrentSection(), orxTRUE);
}

void Character::OnDelete()
{
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
}
