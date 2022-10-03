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
        fIncomingDamage = orxFLOAT_0;
        PushConfigSection();
        SetHealth(fReviveHealth);
        PopConfigSection();
        AddTrack("ReviveTrack");
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
    bCharacter = orxTRUE;
    fHealth = fMaxHealth = orxConfig_GetFloat("Health");
    fReviveHealth = orxConfig_GetFloat("ReviveHealth");
    fRunSpeed = orxConfig_GetFloat("RunSpeed");
    fHealDistance = orxConfig_GetFloat("HealDistance");
    u64Loadout = orxConfig_GetU64("Loadout");
    bAction1Auto = orxConfig_GetBool("Action1Auto");
    bAction2Auto = orxConfig_GetBool("Action2Auto");
    fDamageTick = orxConfig_GetFloat("DamageTick");
    fDamageTime = orxObject_GetActiveTime(GetOrxObject()) + fDamageTick;
    fIncomingDamage = orxFLOAT_0;

    // Enable our input set
    orxInput_EnableSet(GetModelName(), orxTRUE);
}

void Character::OnDelete()
{
    Object::OnDelete();
}

orxBOOL Character::OnCollide(ScrollObject *_poCollider, orxBODY_PART *_pstPart, orxBODY_PART *_pstColliderPart, const orxVECTOR &_rvPosition, const orxVECTOR &_rvNormal)
{
    if(!IsDead())
    {
        orxOBJECT *pstOwner;
        if(((pstOwner = orxOBJECT(orxObject_GetOwner(_poCollider->GetOrxObject()))) != GetOrxObject())
        && (!pstOwner || (orxOBJECT(orxObject_GetOwner(pstOwner)) != GetOrxObject())))
        {
            Object *poObject = ScrollCast<Object *>(_poCollider);
            if(!poObject->IsCharacter()
            || !ScrollCast<Character *>(_poCollider)->IsDead())
            {
                if(poObject->IsSingleHit())
                {
                    SetHealth(GetHealth() - poObject->GetDamage());
                    if(poObject->GetKnockback() > orxFLOAT_0)
                    {
                        orxVECTOR vKnockback, vPos, vColliderPos;
                        orxVector_Add(&vPos,
                                      &vPos,
                                      orxVector_Mulf(&vKnockback,
                                                     orxVector_Normalize(&vKnockback,
                                                                         orxVector_Sub(&vKnockback,
                                                                                       &GetPosition(vPos, orxTRUE),
                                                                                       &_poCollider->GetPosition(vColliderPos, orxTRUE))),
                                                     poObject->GetKnockback()));
                         SetPosition(vPos);
                    }

                    if(bPlayer)
                    {
                        if(poObject->GetDamage() < orxFLOAT_0)
                        {
                            AddTrack("PlayerHealTrack");
                        }
                        else if(poObject->GetDamage() > orxFLOAT_0)
                        {
                            AddTrack("PlayerHitTrack");
                        }
                    }

                    _poCollider->SetLifeTime(orxFLOAT_0);
                }
                else
                {
                    fIncomingDamage += poObject->GetDamage();
                }
            }
        }
    }

    return Object::OnCollide(_poCollider, _pstPart, _pstColliderPart, _rvPosition, _rvNormal);
}

orxBOOL Character::OnSeparate(ScrollObject *_poCollider)
{
    if(!IsDead())
    {
        Object *poObject = ScrollCast<Object *>(_poCollider);
        if(!poObject->IsCharacter()
        || !ScrollCast<Character *>(_poCollider)->IsDead())
        {
            if(!poObject->IsSingleHit())
            {
                fIncomingDamage -= poObject->GetDamage();
            }
        }
    }
    return Object::OnSeparate(_poCollider);
}

void Character::Update(const orxCLOCK_INFO &_rstInfo)
{
    if(bPlayer && orxObject_GetActiveTime(GetOrxObject()) >= fDamageTime)
    {
        fDamageTime = orxObject_GetActiveTime(GetOrxObject()) + fDamageTick;
        if(!bDead && fIncomingDamage)
        {
            SetHealth(GetHealth() - fIncomingDamage);
            AddTrack("PlayerHitTrack");
        }
    }

    if(!bDead)
    {
        ld51 &roGame = ld51::GetInstance();

        // Select our input set
        const orxSTRING zInputSet = orxInput_GetCurrentSet();
        orxInput_SelectSet(GetModelName());

        // Update movement
        const orxSTRING zAnim = "Idle";
        orxVECTOR vMove = {orxInput_GetValue("MoveRight") - orxInput_GetValue("MoveLeft"), orxInput_GetValue("MoveDown") - orxInput_GetValue("MoveUp")};
        orxVector_FromCartesianToSpherical(&vMove, &vMove);
        if(vMove.fRho >= roGame.fDeadZone)
        {
            zAnim = "Run";
            vMove.fRho = fRunSpeed;
            if(orxMath_Abs(vMove.fTheta) > orxMATH_KF_PI_BY_2)
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
        orxVector_FromSphericalToCartesian(&vMove, &vMove);
        SetSpeed(vMove);

        // Containment
        orxVECTOR vPos;
        GetPosition(vPos, orxTRUE);
        orxVector_Clamp(&vPos, &vPos, &roGame.vArenaTL, &roGame.vArenaBR);
        SetPosition(vPos, orxTRUE);

        // Aim
        if(Object *poLoadout = roGame.GetObject<Object>(u64Loadout))
        {
            orxVECTOR vAim = {orxInput_GetValue("AimRight") - orxInput_GetValue("AimLeft"), orxInput_GetValue("AimDown") - orxInput_GetValue("AimUp")};
            orxVector_FromCartesianToSpherical(&vAim, &vAim);
            if(vAim.fRho >= roGame.fDeadZone)
            {
                orxVECTOR vScale;
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

            // Action1?
            if(!poLoadout->IsAnim("Action1", orxTRUE)
            && ((bAction1Auto && orxInput_IsActive("Action1"))
             || orxInput_HasBeenActivated("Action1")))
            {
                zAnim = "Action1";
            }
            // Action2?
            if(!poLoadout->IsAnim("Action2", orxTRUE)
            && ((bAction2Auto && orxInput_IsActive("Action2"))
             || orxInput_HasBeenActivated("Action2")))
            {
                zAnim = "Action2";
            }
        }

        // Update anim
        SetAnim(zAnim);

        // Restore previous input set
        orxInput_SelectSet(zInputSet);
    }
    else
    {
        SetSpeed(orxVECTOR_0);
    }

    Object::Update(_rstInfo);
}
