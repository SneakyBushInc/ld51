/**
 * @file Player.h
 * @date 1-Oct-2022
 */

#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Character.h"

/** Player Class
 */
class Player : public Character
{
public:

                void            Die();


protected:

                void            OnCreate();
                void            OnDelete();
                void            Update(const orxCLOCK_INFO &_Info);


private:
};

#endif // __PLAYER_H__
