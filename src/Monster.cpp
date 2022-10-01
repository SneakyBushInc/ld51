/**
 * @file Monster.cpp
 * @date 1-Oct-2022
 */

#include "Monster.h"

void Monster::Die()
{
}

void Monster::OnCreate()
{
    // Init variables
    Character::OnCreate();
    orxConfig_SetBool("IsMonster", orxTRUE);

    // Disable our input set
    orxInput_EnableSet(orxConfig_GetCurrentSection(), orxFALSE);
}

void Monster::OnDelete()
{
}

void Monster::Update(const orxCLOCK_INFO &_rstInfo)
{
    if(!bDead)
    {
        // Push config section
        PushConfigSection();

        //! TODO: AI

        // Pop config section
        PopConfigSection();
    }
}
