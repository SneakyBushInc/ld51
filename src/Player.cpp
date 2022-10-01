/**
 * @file Player.cpp
 * @date 1-Oct-2022
 */

#include "Player.h"

void Player::Die()
{
}

void Player::OnCreate()
{
    // Init variables
    Character::OnCreate();
    orxConfig_SetBool("IsPlayer", orxTRUE);
}

void Player::OnDelete()
{
}

void Player::Update(const orxCLOCK_INFO &_rstInfo)
{
    if(!bDead)
    {
        // Push config section
        PushConfigSection();

        //! TODO: Player specifics

        // Pop config section
        PopConfigSection();
    }
}
