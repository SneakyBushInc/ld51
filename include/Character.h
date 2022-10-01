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


protected:

                void            OnCreate();
                void            OnDelete();
                void            Update(const orxCLOCK_INFO &_Info);


private:

                orxFLOAT        fHealth;
};

#endif // __CHARACTER_H__
