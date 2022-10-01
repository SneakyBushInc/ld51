/**
 * @file Monster.h
 * @date 1-Oct-2022
 */

#ifndef __MONSTER_H__
#define __MONSTER_H__

#include "Character.h"

/** Monster Class
 */
class Monster : public Character
{
public:

                void            Die();


protected:

                void            OnCreate();
                void            OnDelete();
                void            Update(const orxCLOCK_INFO &_Info);


private:
};

#endif // __MONSTER_H__