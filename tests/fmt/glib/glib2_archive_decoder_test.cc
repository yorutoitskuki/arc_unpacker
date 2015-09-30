#include "fmt/glib/glib2_archive_decoder.h"
#include "test_support/archive_support.h"
#include "test_support/catch.hh"
#include "test_support/file_support.h"

using namespace au;
using namespace au::fmt::glib;

static void do_test(const std::string &input_path)
{
    std::vector<std::shared_ptr<File>> expected_files
    {
        tests::stub_file("123.txt", "1234567890"_b),
        tests::stub_file("abc.txt", "abcdefghijklmnopqrstuvwxyz"_b),
    };

    Glib2ArchiveDecoder decoder;
    auto actual_files = tests::unpack_to_memory(input_path, decoder);
    tests::compare_files(expected_files, actual_files, true);
}

TEST_CASE("GLib GLib2 archives (Musume plugin)", "[fmt]")
{
    do_test("tests/fmt/glib/files/glib2/musume.g2");
}

TEST_CASE("GLib GLib2 archives (Mei plugin)", "[fmt]")
{
    do_test("tests/fmt/glib/files/glib2/mei.g2");
}

TEST_CASE("GLib GLib2 archives with nested directories", "[fmt]")
{
    std::vector<std::shared_ptr<File>> expected_files
    {
        tests::stub_file("test/nest/123.txt", "1234567890"_b),
        tests::stub_file("test/abc.txt", "abcdefghijklmnopqrstuvwxyz"_b),
    };

    Glib2ArchiveDecoder decoder;
    auto actual_files = tests::unpack_to_memory(
        "tests/fmt/glib/files/glib2/nest.g2", decoder);
    tests::compare_files(expected_files, actual_files, true);
}

TEST_CASE("GLib GLib2 archives with multiple decryption passes", "[fmt]")
{
    bstr big_content(0xA0000, 0xFF);
    auto big_file = tests::stub_file("big.txt", big_content);
    std::vector<std::shared_ptr<File>> expected_files { big_file };

    Glib2ArchiveDecoder decoder;
    auto arc_file = tests::zlib_file_from_path(
        "tests/fmt/glib/files/glib2/big-zlib.g2");
    auto actual_files = decoder.unpack(*arc_file, true);
    tests::compare_files(expected_files, actual_files, true);
}
