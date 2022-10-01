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
                void            Revive();
                void            SetHealth(orxFLOAT _fHealth);


protected:

                void            OnCreate();
                void            OnDelete();
                void            Update(const orxCLOCK_INFO &_Info);


                orxBOOL         bDead;


private:

                orxFLOAT        fHealth, fMaxHealth;
};

#endif // __CHARACTER_H__
