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
            orxVECTOR vTemp;
            Player *poNewPlayer = ld51::GetInstance().CreateObject<Player>(GetModelName());
            poNewPlayer->SetHealth(GetHealth());
            poNewPlayer->bDead = bDead;
            poNewPlayer->SetPosition(GetPosition(vTemp));
            poNewPlayer->SetScale(GetScale(vTemp));
            poNewPlayer->SetAnim(orxObject_GetCurrentAnim(GetOrxObject()), orxTRUE, orxTRUE);
            SetLifeTime(orxFLOAT_0);
        }

        // Pop config section
        PopConfigSection();
    }
    Character::Update(_rstInfo);
    bNew = orxFALSE;
}
