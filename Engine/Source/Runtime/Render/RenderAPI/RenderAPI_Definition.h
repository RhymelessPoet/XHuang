#pragma once
#include "Math/Vector2.h"

namespace XHuang
{

using Extent2D = Vector2<UInt32>;
using Extent2DFloat = Vector2<float>;

enum class EQueueType
{
    UnDefine = 0,
    Graphics,
    Compute,
    Present,
    Max
};

}