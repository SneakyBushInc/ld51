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
            poNewPlayer->SetAnim(orxObject_GetCurrentAnim(GetOrxObject()), orxTRUE);
            SetLifeTime(orxFLOAT_0);
        }
        else
        {
            // Select our input set
            const orxSTRING zInputSet = orxInput_GetCurrentSet();
            orxInput_SelectSet(orxConfig_GetCurrentSection());

            // Revive?
            if(orxInput_HasBeenActivated("Revive"))
            {
                ld51 &roGame = ld51::GetInstance();

                orxVECTOR vPos;
                GetPosition(vPos, orxTRUE);
                Player *poBestTarget    = orxNULL;
                orxFLOAT fBestDistance  = orxFLOAT_MAX;
                orxFLOAT fHealDistance  = orxConfig_GetFloat("HealDistance");

                for(Player *poTarget = roGame.GetNextObject<Player>();
                    poTarget;
                    poTarget = roGame.GetNextObject<Player>(poTarget))
                {
                    if(poTarget->IsDead())
                    {
                        orxVECTOR vTargetPos;
                        poTarget->GetPosition(vTargetPos, orxTRUE);
                        orxFLOAT fDistance = orxVector_GetSquareDistance(&vPos, &vTargetPos);
                        if(fDistance < fHealDistance)
                        {
                            poTarget->Revive();
                        }
                    }
                }
            }

            // Restore previous input set
            orxInput_SelectSet(zInputSet);
        }

        // Pop config section
        PopConfigSection();
    }
    if(!bNew)
    {
        Character::Update(_rstInfo);
    }
    else
    {
        bNew = orxFALSE;
    }
}
