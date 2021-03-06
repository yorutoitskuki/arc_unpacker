#include "dec/fvp/nvsg_image_decoder.h"
#include "test_support/catch.h"
#include "test_support/decoder_support.h"
#include "test_support/file_support.h"
#include "test_support/image_support.h"

using namespace au;
using namespace au::dec::fvp;

static const std::string dir = "tests/dec/fvp/files/nvsg/";

static void do_test(
    const std::string &input_path, const std::string &expected_path)
{
    const auto decoder = NvsgImageDecoder();
    const auto input_file = tests::file_from_path(dir + input_path);
    const auto expected_file = tests::file_from_path(dir + expected_path);
    const auto actual_image = tests::decode(decoder, *input_file);
    tests::compare_images(actual_image, *expected_file);
}

TEST_CASE("FVP NVSG images", "[dec]")
{
    SECTION("Format 0")
    {
        do_test("BG085_001", "BG085_001-out.png");
    }

    SECTION("Format 1")
    {
        do_test("bu_effect22_F_1", "bu_effect22_F_1-out.png");
    }

    SECTION("Format 2")
    {
        do_test("CHR_Shigure", "CHR_Shigure-out.png");
    }

    SECTION("Format 3")
    {
        do_test("diss48", "diss48-out.png");
    }

    SECTION("Format 4")
    {
        do_test("gaiji_heart", "gaiji_heart-out.png");
    }
}
