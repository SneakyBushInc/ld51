/**
 * @file Character.h
 * @date 1-Oct-2022
 */

#ifndef __CHARACTER_H__
#define __CHARACTER_H__

#include "Object.h"

/** Character Class
 */
class Character : public Object
{
public:

                void            Die();
                orxBOOL         IsDead() const                  {return bDead;}
                void            Revive();
                void            SetHealth(orxFLOAT _fHealth);
                orxFLOAT        GetHealth() const               {return fHealth;}
                orxFLOAT        GetMaxHealth() const            {return fMaxHealth;}
                orxFLOAT        GetHealDistance() const         {return fHealDistance;}


protected:

                void            OnCreate();
                void            OnDelete();
                orxBOOL         OnCollide(ScrollObject *_poCollider, orxBODY_PART *_pstPart, orxBODY_PART *_pstColliderPart, const orxVECTOR &_rvPosition, const orxVECTOR &_rvNormal);
                orxBOOL         OnSeparate(ScrollObject *_poCollider);
                void            Update(const orxCLOCK_INFO &_Info);

                orxBOOL         bPlayer, bAttract;


private:

                orxU64          u64Loadout;
                orxFLOAT        fHealth, fMaxHealth, fReviveHealth;
                orxFLOAT        fIncomingDamage, fDamageTick, fDamageTime;
                orxFLOAT        fRunSpeed, fHealDistance;
                orxBOOL         bDead, bAction1Auto, bAction2Auto;
};

#endif // __CHARACTER_H__
