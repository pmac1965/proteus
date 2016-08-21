#pragma once


#include "../core/prTypes.h"
#include "../core/prString.h"


// Namespaces
namespace Proteus {
namespace Gui {


class prLayer
{
private:
    friend class prLayerManager;

    explicit prLayer(const char *name);


public:
    void SetName(const char *name);


private:
    prString            mName;
    Proteus::Core::u32  mBitFlag;

};


}}// Namespaces
