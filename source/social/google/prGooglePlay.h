#pragma once


#include "../../prConfig.h"


/*#if (defined(PLATFORM_ANDROID) || defined(PLATFORM_IOS))
    #include "gpg/achievement.h"
    #include "gpg/achievement_manager.h"
    #include "gpg/builder.h"
    #include "gpg/debug.h"
    #include "gpg/default_callbacks.h"
    #include "gpg/game_services.h"
    #include "gpg/leaderboard.h"
    #include "gpg/leaderboard_manager.h"
    #include "gpg/platform_configuration.h"
    #include "gpg/player_manager.h"
    #include "gpg/score_page.h"
    #include "gpg/types.h"
#else

#endif*/



class prGooglePlay
{
public:
    //prGooglePlay();
    //~prGooglePlay();


    static void Init();

private:
//#if (defined(PLATFORM_ANDROID) || defined(PLATFORM_IOS))

    //static bool is_auth_in_progress_;
    //static std::unique_ptr<gpg::GameServices> game_services_;



//#endif
};
