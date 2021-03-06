#include "dec/kirikiri/xp3_archive_decoder.h"
#include "test_support/catch.h"
#include "test_support/decoder_support.h"
#include "test_support/file_support.h"

using namespace au;
using namespace au::dec::kirikiri;

static const std::string dir = "tests/dec/kirikiri/files/xp3/";

static void do_test(const std::string &input_path)
{
    const std::vector<std::shared_ptr<io::File>> expected_files
    {
        tests::stub_file("123.txt", "1234567890"_b),
        tests::stub_file("abc.xyz", "abcdefghijklmnopqrstuvwxyz"_b),
    };
    Xp3ArchiveDecoder decoder;
    decoder.plugin_manager.set("noop");
    const auto input_file = tests::file_from_path(dir + input_path);
    const auto actual_files = tests::unpack(decoder, *input_file);
    tests::compare_files(actual_files, expected_files, true);
}

TEST_CASE("KiriKiri XP3 archives", "[dec]")
{
    SECTION("Version 1")
    {
        do_test("xp3-v1.xp3");
    }

    SECTION("Version 2")
    {
        do_test("xp3-v2.xp3");
    }

    SECTION("Compressed file table")
    {
        do_test("xp3-compressed-table.xp3");
    }

    SECTION("Compressed file data")
    {
        do_test("xp3-compressed-files.xp3");
    }

    SECTION("Multiple SEGM chunks")
    {
        do_test("xp3-multiple-segm.xp3");
    }

    SECTION("Shuffled chunks")
    {
        do_test("xp3-shuffled.xp3");
    }

    SECTION("'time' chunk")
    {
        do_test("xp3-time.xp3");
    }
}
