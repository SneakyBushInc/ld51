/**
 * @file ld51.cpp
 * @date 30-Sep-2022
 */

#define __SCROLL_IMPL__
#include "ld51.h"
#undef __SCROLL_IMPL__

#include "Object.h"
#include "Character.h"
#include "Monster.h"
#include "Player.h"

#define orxBUNDLE_IMPL
#include "orxBundle.h"
#undef orxBUNDLE_IMPL

#define orxMOD_IMPL
#include "orxMod.h"
#undef orxMOD_IMPL

#ifdef __orxMSVC__

/* Requesting high performance dedicated GPU on hybrid laptops */
__declspec(dllexport) unsigned long NvOptimusEnablement        = 1;
__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;

#endif // __orxMSVC__

static orxBOOL sbRestart = orxTRUE;

static orxSTATUS orxFASTCALL EventHandler(const orxEVENT *_pstEvent)
{
    static orxS32 s32PauseCount = 0;
    orxSTATUS eResult = orxSTATUS_SUCCESS;

    switch(_pstEvent->eID)
    {
        case orxSYSTEM_EVENT_FOCUS_LOST:
        case orxSYSTEM_EVENT_BACKGROUND:
        {
            s32PauseCount++;
            break;
        }

        case orxSYSTEM_EVENT_FOCUS_GAINED:
        case orxSYSTEM_EVENT_FOREGROUND:
        {
            s32PauseCount = orxMAX(s32PauseCount - 1, 0);
            break;
        }
    }

    ld51::GetInstance().PauseGame(s32PauseCount > 0);

    return eResult;
}

/** Update function, it has been registered to be called every tick of the core clock
 */
void ld51::Update(const orxCLOCK_INFO &_rstInfo)
{
    // Should quit?
    if(orxInput_IsActive("Quit"))
    {
        // Should restart?
        sbRestart = orxInput_HasBeenActivated("Reset");

        // Send close event
        orxEvent_SendShort(orxEVENT_TYPE_SYSTEM, orxSYSTEM_EVENT_CLOSE);
    }
    // Screenshot?
    else if(orxInput_HasBeenActivated("Screenshot"))
    {
        orxScreenshot_Capture();
    }
    // Reset?
    else if(orxInput_HasBeenActivated("Reset"))
    {
        for(ScrollObject *poObject = GetNextObject();
            poObject;
            poObject = GetNextObject())
        {
            DeleteObject(poObject);
        }
        CreateObject("Select");
    }
    else
    {
        // Has scene and not gameover?
        Object *poScene;
        if((poScene = GetObject<Object>(orxConfig_GetU64("Scene")))
        && (!GetObject<Object>(orxConfig_GetU64("GameOver"))))
        {
            // Game over?
            if(GetPlayerCount() == 0)
            {
                // Creates game over
                CreateObject("GameOver");
            }
        }
    }
}

/** Init function, it is called when all orx's modules have been initialized
 */
orxSTATUS ld51::Init()
{
    // Is processing a new bundle?
    if(orxBundle_IsProcessing())
    {
        // Done!
        return orxSTATUS_SUCCESS;
    }

    // Initialize MOD support
    orxMod_Init();

    // Register event handler
    orxEvent_AddHandler(orxEVENT_TYPE_SYSTEM, EventHandler);

    // Push game section
    orxConfig_PushSection("Game");

    // Get arena corners
    orxConfig_GetListVector("ArenaCorners", 0, &vArenaTL);
    orxConfig_GetListVector("ArenaCorners", 1, &vArenaBR);

    // Get dead zone
    fDeadZone = orxConfig_GetFloat("DeadZone");

    // Disable main viewport
    orxViewport_Enable(GetMainViewport(), orxFALSE);

    // Go to title
    CreateObject("Title");

    // Disable warnings
    orxDEBUG_ENABLE_LEVEL(orxDEBUG_LEVEL_OBJECT, orxFALSE);

    // Done!
    return orxSTATUS_SUCCESS;
}

/** Run function, it should not contain any game logic
 */
orxSTATUS ld51::Run()
{
    // Return orxSTATUS_FAILURE to instruct orx to quit
    return orxSTATUS_SUCCESS;
}

/** Exit function, it is called before exiting from orx
 */
void ld51::Exit()
{
    // Exit from MOD support
    orxMod_Exit();

    // Exit from bundle support
    orxBundle_Exit();

    // Let orx clean all our mess automatically. :)
}

/** BindObjects function, ScrollObject-derived classes are bound to config sections here
 */
void ld51::BindObjects()
{
    ScrollBindObject<Object>("Object");
    ScrollBindObject<Character>("Character");
    ScrollBindObject<Monster>("Monster");
    ScrollBindObject<Player>("Player");
}

orxU32 ld51::GetPlayerCount() const
{
    orxU32 u32Result = 0;
    for(Player *poPlayer = GetNextObject<Player>();
        poPlayer;
        poPlayer = GetNextObject<Player>(poPlayer))
    {
        if(!poPlayer->IsDead())
        {
            u32Result++;
        }
    }

    return u32Result;
}

/** Bootstrap function, it is called before config is initialized, allowing for early resource storage definitions
 */
orxSTATUS ld51::Bootstrap() const
{
    // Initialize bundle resource type
    orxBundle_Init();

    // Add config storage to find the initial config file
    orxResource_AddStorage(orxCONFIG_KZ_RESOURCE_GROUP, orxBUNDLE_KZ_RESOURCE_STORAGE, orxFALSE);
    orxResource_AddStorage(orxCONFIG_KZ_RESOURCE_GROUP, "../data/config", orxFALSE);

    // Return orxSTATUS_FAILURE to prevent orx from loading the default config file
    return orxSTATUS_SUCCESS;
}

/** Main function
 */
int main(int argc, char **argv)
{
    // Should restart?
    while(sbRestart)
    {
        // Clear restart
        sbRestart = orxFALSE;

        // Execute our game
        ld51::GetInstance().Execute(argc, argv);
    }

    // Done!
    return EXIT_SUCCESS;
}
