/**
 * @file Object.h
 * @date 30-Sep-2022
 */

#ifndef __OBJECT_H__
#define __OBJECT_H__

#include "ld51.h"

/** Object Class
 */
class Object : public ScrollObject
{
public:


protected:

                void            OnCreate();
                void            OnDelete();
                void            Update(const orxCLOCK_INFO &_rstInfo);


private:
};

#endif // __OBJECT_H__
