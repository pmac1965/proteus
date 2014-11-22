/**
 * prWeb_bada.cpp
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


#if defined(PLATFORM_BADA)


#include <FBase.h>
#include <FApp.h>
#include <stdio.h>
#include "prWeb.h"


// Using
using namespace Osp::App;
using namespace Osp::Base;
using namespace Osp::Base::Collection;


/// ---------------------------------------------------------------------------
/// Ctor
/// ---------------------------------------------------------------------------
prWeb::prWeb()
{
}


/// ---------------------------------------------------------------------------
/// Dtor
/// ---------------------------------------------------------------------------
prWeb::~prWeb()
{
}


/// ---------------------------------------------------------------------------
/// Opens a web page in a browser
/// ---------------------------------------------------------------------------
void prWeb::OpenURL(const char *address)
{
    if (address && *address)
    {
    	// Create address
        char buffer[512];
        snprintf(buffer, sizeof(buffer), "url:%s", address);

        ArrayList* pDataList = new ArrayList();
        if (pDataList)
        {
			pDataList->Construct();

			String* pData = new String(buffer);
			if (pData)
			{
				pDataList->Add(*pData);

				AppControl* pAc = AppManager::FindAppControlN(APPCONTROL_BROWSER, "");
				if (pAc)
				{
					pAc->Start(pDataList, null);
					delete pAc;
				}
			}

			pDataList->RemoveAll(true);
			delete pDataList;
        }
    }
}


#endif
