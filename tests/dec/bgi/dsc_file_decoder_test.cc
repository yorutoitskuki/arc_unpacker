#include "dec/bgi/dsc_file_decoder.h"
#include "test_support/catch.h"
#include "test_support/decoder_support.h"
#include "test_support/file_support.h"
#include "test_support/image_support.h"

using namespace au;
using namespace au::dec::bgi;

static const std::string dir = "tests/dec/bgi/files/dsc/";

static void do_test_image(
    const std::string &input_path, const std::string &expected_path)
{
    const auto decoder = DscFileDecoder();
    const auto input_file = tests::file_from_path(dir + input_path);
    const auto expected_file = tests::file_from_path(dir + expected_path);
    const auto actual_file = tests::decode(decoder, *input_file);
    tests::compare_images(*actual_file, *expected_file, false);
}

static void do_test_file(
    const std::string &input_path, const std::string &expected_path)
{
    const auto decoder = DscFileDecoder();
    const auto input_file = tests::file_from_path(dir + input_path);
    const auto expected_file = tests::file_from_path(dir + expected_path);
    const auto actual_file = tests::decode(decoder, *input_file);
    tests::compare_files(*actual_file, *expected_file, false);
}

TEST_CASE("BGI DSC files / images", "[dec]")
{
    SECTION("Raw files")
    {
        do_test_file("setupforgallery", "setupforgallery-out.dat");
    }

    SECTION("8-bit images")
    {
        do_test_image("SGTitle010000", "SGTitle010000-out.png");
    }

    SECTION("24-bit images")
    {
        do_test_image("SGMsgWnd010300", "SGMsgWnd010300-out.png");
    }

    SECTION("32-bit images")
    {
        do_test_image("SGTitle000000", "SGTitle000000-out.png");
    }
}
