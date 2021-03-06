#pragma once

#include "io/file.h"
#include "res/image.h"

namespace au {
namespace dec {
namespace kirikiri {
namespace tlg {

    class Tlg5Decoder final
    {
    public:
        res::Image decode(io::File &file);
    };

} } } }
