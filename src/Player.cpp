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
    bPlayer = orxTRUE;
    bNew    = orxTRUE;
}

void Player::OnDelete()
{
    Character::OnDelete();
}

void Player::Update(const orxCLOCK_INFO &_rstInfo)
{
    if(!bAttract)
    {
        if(!IsDead())
        {
            // Cycle?
            if(!bNew && orxInput_HasBeenActivated("Cycle"))
            {
                orxVECTOR vTemp;
                Player *poNewPlayer = ld51::GetInstance().CreateObject<Player>(GetModelName());
                poNewPlayer->SetHealth(GetHealth());
                poNewPlayer->SetPosition(GetPosition(vTemp));
                poNewPlayer->SetScale(GetScale(vTemp));
                poNewPlayer->SetAnim(orxObject_GetCurrentAnim(GetOrxObject()), orxTRUE);
                SetLifeTime(orxFLOAT_0);
            }
            else
            {
                // Select our input set
                const orxSTRING zInputSet = orxInput_GetCurrentSet();
                orxInput_SelectSet(GetModelName());

                // Revive?
                if(orxInput_HasBeenActivated("Revive"))
                {
                    ld51 &roGame = ld51::GetInstance();

                    orxVECTOR vPos;
                    GetPosition(vPos, orxTRUE);
                    Player *poBestTarget    = orxNULL;
                    orxFLOAT fBestDistance  = orxFLOAT_MAX;

                    for(Player *poTarget = roGame.GetNextObject<Player>();
                        poTarget;
                        poTarget = roGame.GetNextObject<Player>(poTarget))
                    {
                        if(poTarget->IsDead())
                        {
                            orxVECTOR vTargetPos;
                            poTarget->GetPosition(vTargetPos, orxTRUE);
                            orxFLOAT fDistance = orxVector_GetSquareDistance(&vPos, &vTargetPos);
                            if(fDistance < GetHealDistance())
                            {
                                poTarget->Revive();
                            }
                        }
                    }
                }

                // Restore previous input set
                orxInput_SelectSet(zInputSet);
            }
        }
        if(!bNew)
        {
            Character::Update(_rstInfo);
        }
        else
        {
            bNew = orxFALSE;
        }
        PushConfigSection();
        orxConfig_SetU32("CurrentHealth", orxF2S(GetHealth()));
        PopConfigSection();
    }
}
