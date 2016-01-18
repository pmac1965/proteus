/**
 * prIosFacebook.mm
 *
 *  Copyright 2014 Paul Michael McNab
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */


#include "../prConfig.h"


#if defined(PLATFORM_IOS)

#if 0
#import <FacebookSDK/FacebookSDK.h>
#endif
#include "../social/facebook/prFacebook.h"
#include "../social/facebook/prFacebook_Ios.h"
#include "../debug/prTrace.h"


#define DEBUG_FACEBOOK


namespace
{
    void CreateNewSession()
    {
#if 0
        FBSession* session = [[FBSession alloc] init];
        [FBSession setActiveSession: session];
#endif
    }
}


/// ----------------------------------------------------------------------------
/// Initialise facebook for ios
/// ----------------------------------------------------------------------------
#if 0
void prFB_Initialise(prFacebookCallbacks *pCallback)
{
    #if defined(DEBUG_FACEBOOK)
    prTrace(LogError, "Facebook::Initalise\n");
    #endif
    
    if (pCallback)
    {
        pCallback->Facebook_Initialise(true);
    }
}


/// ----------------------------------------------------------------------------
/// Open a session
/// ----------------------------------------------------------------------------
void prFB_OpenSession(prFacebookCallbacks *pCallback, bool &mIsLoggedIn)
{
    #if defined(DEBUG_FACEBOOK)
    prTrace(LogError, "(1): Facebook::OpenSession\n");
    #endif
    
    NSArray *permissions = [[NSArray alloc] initWithObjects:
    @"email", @"public_profile", @"user_friends",
    nil];
     
     // Attempt to open the session. If the session is not open, show the user the Facebook login UX
    [FBSession openActiveSessionWithReadPermissions:permissions allowLoginUI:false completionHandler:^(FBSession *session, FBSessionState status, NSError *error)
    {
        #if defined(DEBUG_FACEBOOK)
        prTrace(LogError, "(2): Facebook::OpenSession\n");
        #endif

        // Error?
        if (error)
        {
            NSLog(@"Open session error");
            NSLog(@"Error %@", [error description]);
        }
        
        // Did something go wrong during login? I.e. did the user cancel?
        if (status == FBSessionStateClosedLoginFailed || status == FBSessionStateClosed || status == FBSessionStateCreatedOpening)
        {
            mIsLoggedIn = false;

            #if defined(DEBUG_FACEBOOK)
            prTrace(LogError, "(4) Facebook::OpenSession\n");
            #endif

            if (pCallback)
            {
                pCallback->Facebook_OpenSession(mIsLoggedIn);
            }
        }
        else
        {
            mIsLoggedIn = true;
            
            #if defined(DEBUG_FACEBOOK)
            prTrace(LogError, "(3) Facebook::OpenSession\n");
            #endif
            
            if (pCallback)
            {
                pCallback->Facebook_OpenSession(mIsLoggedIn);
            }
        }
    }];
}


/// ----------------------------------------------------------------------------
/// Login to facebook
/// ----------------------------------------------------------------------------
void prFB_Login(prFacebookCallbacks *pCallback, bool &mIsLoggedIn)
{
    #if defined(DEBUG_FACEBOOK)
    prTrace(LogError, "(1) Facebook::Login()\n");
    #endif
    
    
    NSArray *permissions = [[NSArray alloc] initWithObjects:
    @"email", @"public_profile", @"user_friends",
    nil];
     
     // Attempt to open the session. If the session is not open, show the user the Facebook login UX
     [FBSession openActiveSessionWithReadPermissions:permissions allowLoginUI:true completionHandler:^(FBSession *session, FBSessionState status, NSError *error)
     {
        #if defined(DEBUG_FACEBOOK)
        prTrace(LogError, "(2) Facebook::Login()\n");
        #endif
         
        // Did something go wrong during login? I.e. did the user cancel?
        if (status == FBSessionStateClosedLoginFailed || status == FBSessionStateClosed || status == FBSessionStateCreatedOpening)
        {
            // If so, just send them round the loop again
            [[FBSession activeSession] closeAndClearTokenInformation];
            [FBSession setActiveSession:nil];
             
            CreateNewSession();
             
            #if defined(DEBUG_FACEBOOK)
            prTrace(LogError, "(3) Facebook::Login()\n");
            #endif
            
            mIsLoggedIn = false;
             
            if (pCallback)
            {
                pCallback->Facebook_Login(mIsLoggedIn);
            }
        }
        else
        {
            #if defined(DEBUG_FACEBOOK)
            prTrace(LogError, "(4) Facebook::Login()\n");
            #endif
            
            mIsLoggedIn = true;
             
            if (pCallback)
            {
                pCallback->Facebook_Login(mIsLoggedIn);
            }
        }
    }];
}




/// ----------------------------------------------------------------------------
/// Fetches the users details
/// ----------------------------------------------------------------------------
void prFB_FetchUserDetails(prFacebookCallbacks *pCallback, prFacebook_Ios *pFacebook)
{
    #if defined(DEBUG_FACEBOOK)
    prTrace(LogError, "(1) prFB_FetchUserDetails\n");
    #endif
    
    // Start the facebook request
    [[FBRequest requestForMe]
     startWithCompletionHandler:
     ^(FBRequestConnection *connection, NSDictionary<FBGraphUser> *result, NSError *error)
     {
         // Did everything come back okay with no errors?
         if (!error && result)
         {
            #if defined(DEBUG_FACEBOOK)
            prTrace(LogError, "(2) prFB_FetchUserDetails\n");
            #endif
             
            // Extract out the player's Facebook ID and first name
            pFacebook->SetDetails((const char *)[result.first_name UTF8String], (u64)[result.id longLongValue]);
             
             if (pCallback)
             {
                 pCallback->Facebook_FetchUserDetails(true);
             }
         }
         else
         {
            #if defined(DEBUG_FACEBOOK)
            prTrace(LogError, "(3) prFB_FetchUserDetails\n");
            #endif
            
            if (pCallback)
            {
                pCallback->Facebook_FetchUserDetails(false);
            }
        }
    }];
}


/// ----------------------------------------------------------------------------
/// Brags to facebook
/// ----------------------------------------------------------------------------
void prFB_Brag(prFacebookCallbacks *pCallback, u64 fbid)
{
    #if defined(DEBUG_FACEBOOK)
    prTrace(LogError, "(1): Facebook::Brag\n");
    #endif
    
    // This function will invoke the Feed Dialog to post to a user's Timeline and News Feed
    // It will attemnt to use the Facebook Native Share dialog
    // If that's not supported we'll fall back to the web based dialog.
    
    // Prepare the native share dialog parameters
    FBShareDialogParams *shareParams = [[FBShareDialogParams alloc] init];
    shareParams.link        = nil;
    shareParams.name        = @"NAME";
    shareParams.caption     = @"CAPTION";
    shareParams.picture     = nil;
    shareParams.description = [NSString stringWithFormat:@"DESC"];
    
    if ([FBDialogs canPresentShareDialogWithParams:shareParams])
    {
        #if defined(DEBUG_FACEBOOK)
        prTrace(LogError, "(2): Facebook::Brag\n");
        #endif
        
        [FBDialogs presentShareDialogWithParams:shareParams
                                    clientState:nil
                                        handler:^(FBAppCall *call, NSDictionary *results, NSError *error)
                                        {
                                            //for (NSString *key in [results allKeys])
                                            //{
                                            //    NSLog(@"RESULT KEY: %@", [results objectForKey:key]);
                                            //}
                                            
                                            if (error)
                                            {
                                                prTrace(LogError, "Error publishing story\n");
                                                // Error publishing story
                                                if (pCallback)
                                                {
                                                    pCallback->Facebook_PostError();
                                                }
                                            }
                                            else if (results[@"completionGesture"] && [results[@"completionGesture"] isEqualToString:@"cancel"])
                                            {
                                                prTrace(LogError, "User cancelled publishing story\n");
                                                // User canceled story publishing
                                                if (pCallback)
                                                {
                                                    pCallback->Facebook_PostCancelled();
                                                }
                                            }
                                            else
                                            {
                                                prTrace(LogError, "Story published\n");
                                                // Story published
                                                if (pCallback)
                                                {
                                                    pCallback->Facebook_PostSent();
                                                }
                                            }
                                        }];
        
    }
    else
    {
        #if defined(DEBUG_FACEBOOK)
        prTrace(LogError, "(3): Facebook::Brag\n");
        #endif
        
        // Prepare the web dialog parameters
        NSDictionary *params = @{
                                 @"name" : shareParams.name,
                                 @"caption" : shareParams.caption,
                                 @"description" : shareParams.description
                                 };
        
        // Invoke the dialog
        [FBWebDialogs presentFeedDialogModallyWithSession:nil
                                               parameters:params
                                                  handler: ^(FBWebDialogResult result, NSURL *resultURL, NSError *error)
         {
             if (error)
             {
                 prTrace(LogError, "Error publishing story\n");
                 // Error publishing story
                 if (pCallback)
                 {
                     pCallback->Facebook_PostError();
                 }
             }
             else
             {
                 if (result == FBWebDialogResultDialogNotCompleted)
                 {
                     prTrace(LogError, "User cancelled publishing story\n");
                     // User canceled story publishing
                     if (pCallback)
                     {
                         pCallback->Facebook_PostCancelled();
                     }
                 }
                 else
                 {
                     prTrace(LogError, "Story published\n");
                     // Story published
                     if (pCallback)
                     {
                         pCallback->Facebook_PostSent();
                     }
                 }
             }
         }];
    }    
}
#endif


#endif//PLATFORM_IOS
