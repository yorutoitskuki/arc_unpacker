#ifndef FORMATS_KIRIKIRI_XP3_FILTERS_FILTER_H
#define FORMATS_KIRIKIRI_XP3_FILTERS_FILTER_H
#include "file.h"

namespace Formats
{
    namespace Kirikiri
    {
        namespace Xp3Filters
        {
            class Filter
            {
            public:
                virtual void decode(File &file, u32 key) const = 0;
            };
        }
    }
}

#endif
