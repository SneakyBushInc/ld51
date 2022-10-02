/**
 * @file Character.cpp
 * @date 1-Oct-2022
 */

#include "Character.h"

void Character::Die()
{
    bDead = orxTRUE;
    SetHealth(orxFLOAT_0);
    SetSpeed(orxVECTOR_0);
    SetAnim("Death");
}

void Character::Revive()
{
    if(bDead)
    {
        bDead = orxFALSE;
        PushConfigSection();
        SetHealth(orxConfig_GetFloat("ReviveHealth"));
        PopConfigSection();
        SetAnim("Revive");
    }
}

void Character::SetHealth(orxFLOAT _fHealth)
{
    if(!bDead)
    {
        fHealth = orxCLAMP(_fHealth, orxFLOAT_0, fMaxHealth);
        if(fHealth == orxFLOAT_0)
        {
            Die();
        }
    }
}

void Character::OnCreate()
{
    // Init variables
    Object::OnCreate();
    orxConfig_SetBool("IsCharacter", orxTRUE);
    fHealth = fMaxHealth = orxConfig_GetFloat("Health");
    fDamage = orxFLOAT_0;

    // Enable our input set
    orxInput_EnableSet(orxConfig_GetCurrentSection(), orxTRUE);
}

void Character::OnDelete()
{
    Object::OnDelete();
}

orxBOOL Character::OnCollide(ScrollObject *_poCollider, orxBODY_PART *_pstPart, orxBODY_PART *_pstColliderPart, const orxVECTOR &_rvPosition, const orxVECTOR &_rvNormal)
{
    if(!IsDead())
    {
        _poCollider->PushConfigSection();
        if(!orxConfig_GetBool("IsCharacter")
        || !(ScrollCast<Character *>(_poCollider))->IsDead())
        {
            if(orxConfig_GetBool("SingleHit"))
            {
                SetHealth(GetHealth() - orxConfig_GetFloat("Damage"));
                orxFLOAT fKnockback = orxConfig_GetFloat("Knockback");
                if(fKnockback > orxFLOAT_0)
                {
                    orxVECTOR vKnockback, vPos, vColliderPos;
                    orxVector_Add(&vPos,
                                  &vPos,
                                  orxVector_Mulf(&vKnockback,
                                                 orxVector_Normalize(&vKnockback,
                                                                     orxVector_Sub(&vKnockback,
                                                                                   &GetPosition(vPos, orxTRUE),
                                                                                   &_poCollider->GetPosition(vColliderPos, orxTRUE))),
                                                 fKnockback));
                     SetPosition(vPos);
                }
                _poCollider->SetLifeTime(orxFLOAT_0);
            }
            else
            {
                fDamage += orxConfig_GetFloat("Damage");
            }
        }
        _poCollider->PopConfigSection();
    }

    return Object::OnCollide(_poCollider, _pstPart, _pstColliderPart, _rvPosition, _rvNormal);
}

orxBOOL Character::OnSeparate(ScrollObject *_poCollider)
{
    _poCollider->PushConfigSection();
    if(!orxConfig_GetBool("SingleHit"))
    {
        fDamage -= orxConfig_GetFloat("Damage");
    }
    _poCollider->PopConfigSection();

    return Object::OnSeparate(_poCollider);
}

void Character::Update(const orxCLOCK_INFO &_rstInfo)
{
    if(!bDead && fDamage)
    {
        SetHealth(GetHealth() - (_rstInfo.fDT * fDamage));
    }

    if(!bDead)
    {
        // Push config section
        PushConfigSection();

        // Select our input set
        const orxSTRING zInputSet = orxInput_GetCurrentSet();
        orxInput_SelectSet(orxConfig_GetCurrentSection());

        // Update movement
        const orxSTRING zAnim = "Idle";
        orxVECTOR vMove = {orxInput_GetValue("MoveRight") - orxInput_GetValue("MoveLeft"), orxInput_GetValue("MoveDown") - orxInput_GetValue("MoveUp")};
        if(!orxVector_IsNull(&vMove))
        {
            zAnim = "Run";
            orxVector_Mulf(&vMove, orxVector_Normalize(&vMove, &vMove), orxConfig_GetFloat("RunSpeed"));
            if(vMove.fX < orxFLOAT_0)
            {
                orxVECTOR vScale;
                GetScale(vScale);
                vScale.fX = -orxMath_Abs(vScale.fX);
                SetScale(vScale);
            }
            else if(vMove.fX > orxFLOAT_0)
            {
                orxVECTOR vScale;
                GetScale(vScale);
                vScale.fX = orxMath_Abs(vScale.fX);
                SetScale(vScale);
            }
        }
        SetSpeed(vMove);

        // Aim
        if(Object *poLoadout = ld51::GetInstance().GetObject<Object>(orxConfig_GetU64("Loadout")))
        {
            orxVECTOR vAim = {orxInput_GetValue("AimRight") - orxInput_GetValue("AimLeft"), orxInput_GetValue("AimDown") - orxInput_GetValue("AimUp")};
            if(!orxVector_IsNull(&vAim))
            {
                orxVECTOR vScale;
                orxVector_FromCartesianToSpherical(&vAim, &vAim);
                poLoadout->GetScale(vScale);
                if(orxMath_Abs(vAim.fTheta) <= orxMATH_KF_PI_BY_2)
                {
                    poLoadout->SetRotation(vAim.fTheta);
                    vScale.fX = orxMath_Abs(vScale.fX);
                }
                else
                {
                    poLoadout->SetRotation(orxMATH_KF_PI + vAim.fTheta);
                    vScale.fX = -orxMath_Abs(vScale.fX);
                }
                poLoadout->SetScale(vScale);
            }
        }

        // Action1?
        if(orxInput_HasBeenActivated("Action1"))
        {
            zAnim = "Action1";
        }
        // Action2?
        if(orxInput_HasBeenActivated("Action2"))
        {
            zAnim = "Action2";
        }

        // Update anim
        SetAnim(zAnim);

        // Restore previous input set
        orxInput_SelectSet(zInputSet);

        // Pop config section
        PopConfigSection();
    }
    else
    {
        SetSpeed(orxVECTOR_0);
    }

    Object::Update(_rstInfo);
}
