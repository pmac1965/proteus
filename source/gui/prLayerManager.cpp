#include "prLayerManager.h"
#include "prLayer.h"


// Namespaces
namespace Proteus {
namespace Gui {


prLayerManager::prLayerManager() : mIndex(0)
{
}


prLayerManager::~prLayerManager()
{
}


bool prLayerManager::AddLayer(const char *name)
{
    new prLayer(name);

    return false;
}


}}// Namespaces
