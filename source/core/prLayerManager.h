#pragma once


#include "prCoreSystem.h"


int LAYER_MAX_LAYERS    = 32;


// Namespaces
namespace Proteus {
namespace Core {


// Forward declarations
class prLayer;


class prLayerManager : public prCoreSystem
{
public:
    prLayerManager();
    ~prLayerManager();

    prLayer *GetLayerByIndex();

private:
    prLayer  **mpLayers;
};


}}// Namespaces
