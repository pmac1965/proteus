#pragma once


#include "../core/prTypes.h"
//#include "../core/prString.h"


// Namespaces
namespace Proteus {
namespace Gui {


class prLayerManager
{
public:
    prLayerManager();
    ~prLayerManager();

    bool AddLayer(const char *name);

private:
    Proteus::Core::u32  mIndex;
};


}}// Namespaces
