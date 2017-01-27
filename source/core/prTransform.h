#pragma once


#include "../math/prVector3.h"


// Namespaces
namespace Proteus {
namespace Core {


class prTransform
{
public:

    Proteus::Math::prVector3    position;
    Proteus::Math::prVector3    rotation;
    Proteus::Math::prVector3    scale;
};


}} // Namespaces