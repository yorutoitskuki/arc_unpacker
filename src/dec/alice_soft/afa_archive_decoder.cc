#include "dec/alice_soft/afa_archive_decoder.h"
#include "algo/locale.h"
#include "algo/pack/zlib.h"
#include "algo/range.h"
#include "err.h"
#include "io/memory_stream.h"

using namespace au;
using namespace au::dec::alice_soft;

static const bstr magic1 = "AFAH"_b;
static const bstr magic2 = "AlicArch"_b;
static const bstr magic3 = "INFO"_b;

bool AfaArchiveDecoder::is_recognized_impl(io::File &input_file) const
{
    if (input_file.stream.read(magic1.size()) != magic1)
        return false;
    input_file.stream.skip(4);
    return input_file.stream.read(magic2.size()) == magic2;
}

std::unique_ptr<dec::ArchiveMeta> AfaArchiveDecoder::read_meta_impl(
    const Logger &logger, io::File &input_file) const
{
    input_file.stream.seek(magic1.size() + 4 + magic2.size() + 4 * 2);
    const auto file_data_start = input_file.stream.read_le<u32>();
    if (input_file.stream.read(magic3.size()) != magic3)
        throw err::CorruptDataError("Corrupt file table");

    const auto table_size_comp = input_file.stream.read_le<u32>();
    const auto table_size_orig = input_file.stream.read_le<u32>();
    const auto file_count = input_file.stream.read_le<u32>();

    io::MemoryStream table_stream(
        algo::pack::zlib_inflate(
            input_file.stream.read(table_size_comp)));

    auto meta = std::make_unique<ArchiveMeta>();
    for (const auto i : algo::range(file_count))
    {
        auto entry = std::make_unique<PlainArchiveEntry>();

        table_stream.skip(4);
        const auto name_size = table_stream.read_le<u32>();
        entry->path = algo::sjis_to_utf8(
            table_stream.read_to_zero(name_size)).str();

        table_stream.skip(4 * 3); // for some games, apparently this is 4 * 2
        entry->offset = table_stream.read_le<u32>() + file_data_start;
        entry->size = table_stream.read_le<u32>();
        meta->entries.push_back(std::move(entry));
    }
    return meta;
}

std::unique_ptr<io::File> AfaArchiveDecoder::read_file_impl(
    const Logger &logger,
    io::File &input_file,
    const dec::ArchiveMeta &m,
    const dec::ArchiveEntry &e) const
{
    const auto entry = static_cast<const PlainArchiveEntry*>(&e);
    const auto data = input_file.stream.seek(entry->offset).read(entry->size);
    auto output_file = std::make_unique<io::File>(entry->path, data);
    output_file->guess_extension();
    return output_file;
}

std::vector<std::string> AfaArchiveDecoder::get_linked_formats() const
{
    return {"alice-soft/aff", "alice-soft/ajp", "alice-soft/qnt"};
}

static auto _ = dec::register_decoder<AfaArchiveDecoder>("alice-soft/afa");
