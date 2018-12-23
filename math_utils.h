#ifndef MATHUTILS_H
#define MATHUTILS_H

#include <QtGlobal>

class MathUtils {
public:
    static qint64 map(qint64 val, qint64 fromLow, qint64 fromHigh, qint64 toLow, qint64 toHigh);
    static qint64 max(qint64 a, qint64 b);
};

#endif //MATHUTILS_H