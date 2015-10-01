#pragma once

#include "fmt/image_decoder.h"

namespace au {
namespace fmt {
namespace minato_soft {

    class FilImageDecoder final : public ImageDecoder
    {
    protected:
        bool is_recognized_internal(File &) const override;
        pix::Grid decode_internal(File &) const override;
    };

} } }