/**
 * @file Monster.cpp
 * @date 1-Oct-2022
 */

#include "Monster.h"

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

        // Select our input set
        const orxSTRING zInputSet = orxInput_GetCurrentSet();
        orxInput_SelectSet(orxConfig_GetCurrentSection());

        // Select target
        orxVECTOR vMove = {};
        if(Player *poTarget = SelectTarget())
        {
            // Move
            orxVECTOR vPos, vTargetPos;
            orxVector_Sub(&vMove, &poTarget->GetPosition(vTargetPos, orxTRUE), &GetPosition(vPos, orxTRUE));
            if(orxVector_GetSquareSize(&vMove) >= orxFLOAT_1)
            {
                orxVector_Normalize(&vMove, &vMove);
            }
            else
            {
                orxVector_Copy(&vMove, &orxVECTOR_0);
            }
        }
        orxInput_SetValue("MoveLeft", orxMath_Abs(orxMIN(vMove.fX, orxFLOAT_0)));
        orxInput_SetValue("MoveRight", orxMAX(vMove.fX, orxFLOAT_0));
        orxInput_SetValue("MoveUp", orxMath_Abs(orxMIN(vMove.fY, orxFLOAT_0)));
        orxInput_SetValue("MoveDown", orxMAX(vMove.fY, orxFLOAT_0));

        // Restore previous input set
        orxInput_SelectSet(zInputSet);

        // Pop config section
        PopConfigSection();
    }
    Character::Update(_rstInfo);
}

Player *Monster::SelectTarget()
{
    ld51 &roGame = ld51::GetInstance();

    orxVECTOR vPos, vTargetPos;
    GetPosition(vPos, orxTRUE);
    Player *poBestTarget = roGame.GetObject<Player>(u64Target);
    orxFLOAT fBestDistance = (poBestTarget && !poBestTarget->IsDead()) ? orxVector_GetSquareDistance(&poBestTarget->GetPosition(vTargetPos, orxTRUE), &vPos) : orxFLOAT_MAX;

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

    return poBestTarget;
}
