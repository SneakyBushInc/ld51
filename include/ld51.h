/**
 * @file ld51.h
 * @date 30-Sep-2022
 */

#ifndef __ld51_H__
#define __ld51_H__

#define __NO_SCROLLED__
#include "Scroll/Scroll.h"

/** Game Class
 */
class ld51 : public Scroll<ld51>
{
public:

                orxVECTOR       vArenaTL, vArenaBR;
                orxFLOAT        fDeadZone;


private:

                orxSTATUS       Bootstrap() const;

                void            Update(const orxCLOCK_INFO &_rstInfo);

                orxSTATUS       Init();
                orxSTATUS       Run();
                void            Exit();
                void            BindObjects();

                orxU32          GetPlayerCount() const;


private:
};

#endif // __ld51_H__
