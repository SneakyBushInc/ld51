/**
 * @file Object.h
 * @date 1-Oct-2022
 */

#ifndef __OBJECT_H__
#define __OBJECT_H__

#include "ld51.h"

/** Object Class
 */
class Object : public ScrollObject
{
public:

                orxFLOAT        GetDamage() const       {return fDamage;}
                orxFLOAT        GetKnockback() const    {return fKnockback;}
                orxBOOL         IsSingleHit() const     {return bSingleHit;}
                orxBOOL         IsCharacter() const     {return bCharacter;}


protected:

                void            OnCreate();
                void            OnDelete();
                void            Update(const orxCLOCK_INFO &_rstInfo);

                orxBOOL         bCharacter;


private:
                orxFLOAT        fDamage, fKnockback;
                orxBOOL         bSingleHit;
};

#endif // __OBJECT_H__
