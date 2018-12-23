#include "math_utils.h"

#include <QtGlobal>

qint64 MathUtils::map(qint64 val, qint64 fromLow, qint64 fromHigh, qint64 toLow, qint64 toHigh) 
{
    return (val - fromLow) * (toHigh - toLow) / (fromHigh - fromLow) + toLow;
}

qint64 MathUtils::max(qint64 a, qint64 b) {
    return a < b ? b : a;
}