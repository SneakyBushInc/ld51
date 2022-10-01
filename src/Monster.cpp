/**
 * @file Monster.cpp
 * @date 1-Oct-2022
 */

#include "Monster.h"
#include "Player.h"

void Monster::Die()
{
    Character::Die();
}

void Monster::OnCreate()
{
    // Init variables
    Character::OnCreate();
    u64Target           = orxU64_UNDEFINED;
    fTargetHysteresis   = orxConfig_GetFloat("TargetHysteresis");
    orxConfig_SetBool("IsMonster", orxTRUE);

    // Disable our input set
    orxInput_EnableSet(orxConfig_GetCurrentSection(), orxFALSE);
}

void Monster::OnDelete()
{
    Character::OnDelete();
}

void Monster::Update(const orxCLOCK_INFO &_rstInfo)
{
    if(!bDead)
    {
        // Push config section
        PushConfigSection();

        // Select target
        SelectTarget();

        //! TODO: AI

        // Pop config section
        PopConfigSection();
    }
    Character::Update(_rstInfo);
}

void Monster::SelectTarget()
{
    ld51 &roGame = ld51::GetInstance();

    orxVECTOR vPos, vTargetPos;
    GetPosition(vPos, orxTRUE);
    ScrollObject *poBestTarget = roGame.GetObject(u64Target);
    orxFLOAT fBestDistance = poBestTarget ? orxVector_GetSquareDistance(&poBestTarget->GetPosition(vTargetPos, orxTRUE), &vPos) : orxFLOAT_MAX;

    for(Player *poTarget = roGame.GetNextObject<Player>();
        poTarget;
        poTarget = roGame.GetNextObject<Player>(poTarget))
    {
        if(!poTarget->IsDead())
        {
            poTarget->GetPosition(vTargetPos, orxTRUE);
            orxFLOAT fDistance = orxVector_GetSquareDistance(&vPos, &vTargetPos);
            if(fDistance < fBestDistance - fTargetHysteresis)
            {
                poBestTarget    = poTarget;
                fBestDistance   = fDistance;
            }
        }
    }

    u64Target = poBestTarget ? poBestTarget->GetGUID() : orxU64_UNDEFINED;
}
