#include "dec/sysadv/pga_image_decoder.h"
#include "dec/png/png_image_decoder.h"

using namespace au;
using namespace au::dec::sysadv;

static const bstr magic = "PGAPGAH\x0A"_b;

bool PgaImageDecoder::is_recognized_impl(io::File &input_file) const
{
    return input_file.stream.read(magic.size()) == magic;
}

res::Image PgaImageDecoder::decode_impl(
    const Logger &logger, io::File &input_file) const
{
    const auto png_data = input_file.stream
        .seek(magic.size())
        .skip(3)
        .read_to_eof();

    io::File png_file;
    png_file.stream.write("\x89\x50\x4E\x47"_b);
    png_file.stream.write("\x0D\x0A\x1A\x0A"_b);
    png_file.stream.write("\x00\x00\x00\x0D"_b);
    png_file.stream.write("IHDR"_b);
    png_file.stream.write(png_data);

    const auto png_decoder = dec::png::PngImageDecoder();
    return png_decoder.decode(logger, png_file);
}

static auto _ = dec::register_decoder<PgaImageDecoder>("sysadv/pga");
