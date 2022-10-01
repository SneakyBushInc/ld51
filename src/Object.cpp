/**
 * @file Object.cpp
 * @date 1-Oct-2022
 */

#include "Object.h"

void Object::OnCreate()
{
    orxConfig_SetBool("IsObject", orxTRUE);
}

void Object::OnDelete()
{
}

void Object::Update(const orxCLOCK_INFO &_rstInfo)
{
}

orxBOOL Object::OnShader(orxSHADER_EVENT_PAYLOAD &_rstPayload)
{
    return orxTRUE;
}
