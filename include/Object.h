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

    virtual     void            Die()               {bDead = orxTRUE;}

                orxBOOL         bDead;


protected:

                void            OnCreate();
                void            OnDelete();
                void            Update(const orxCLOCK_INFO &_rstInfo);

                orxBOOL         OnShader(orxSHADER_EVENT_PAYLOAD &_rstPayload);


private:
};

#endif // __OBJECT_H__
