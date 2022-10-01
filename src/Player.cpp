/**
 * @file Player.cpp
 * @date 1-Oct-2022
 */

#include "Player.h"

void Player::Die()
{
    Character::Die();
}

void Player::OnCreate()
{
    // Init variables
    Character::OnCreate();
    bNew = orxTRUE;
    orxConfig_SetBool("IsPlayer", orxTRUE);
    orxConfig_SetString("Class", orxConfig_GetParent(orxConfig_GetCurrentSection()));
}
    
void Player::OnDelete()
{
    Character::OnDelete();
}

void Player::Update(const orxCLOCK_INFO &_rstInfo)
{
    if(!bDead)
    {
        // Push config section
        PushConfigSection();

        // Cycle?
        if(!bNew && orxInput_HasBeenActivated("Cycle"))
        {
            Player *poNewPlayer = ld51::GetInstance().CreateObject<Player>(GetModelName());
            poNewPlayer->SetHealth(GetHealth());
            poNewPlayer->bDead = bDead;
            SetLifeTime(orxFLOAT_0);
        }

        // Pop config section
        PopConfigSection();
    }
    Character::Update(_rstInfo);
    bNew = orxFALSE;
}
