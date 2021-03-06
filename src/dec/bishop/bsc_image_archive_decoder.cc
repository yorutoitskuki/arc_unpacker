#include "dec/bishop/bsc_image_archive_decoder.h"

using namespace au;
using namespace au::dec::bishop;

static const auto magic = "BSS-Composition"_b;

bool BscImageArchiveDecoder::is_recognized_impl(io::File &input_file) const
{
    return input_file.stream.seek(0).read(magic.size()) == magic;
}

algo::NamingStrategy BscImageArchiveDecoder::naming_strategy() const
{
    return algo::NamingStrategy::Sibling;
}

std::unique_ptr<dec::ArchiveMeta> BscImageArchiveDecoder::read_meta_impl(
    const Logger &logger, io::File &input_file) const
{
    const auto file_count = input_file.stream.seek(17).read<u8>();
    auto meta = std::make_unique<dec::ArchiveMeta>();
    input_file.stream.seek(32);
    while (input_file.stream.left())
    {
        auto entry = std::make_unique<PlainArchiveEntry>();
        entry->offset = input_file.stream.pos();
        entry->size = input_file.stream.skip(0x36).read_le<u32>() + 0x40;
        input_file.stream.seek(entry->offset + entry->size);
        meta->entries.push_back(std::move(entry));
    }
    return meta;
}

std::unique_ptr<io::File> BscImageArchiveDecoder::read_file_impl(
    const Logger &logger,
    io::File &input_file,
    const dec::ArchiveMeta &m,
    const dec::ArchiveEntry &e) const
{
    const auto entry = static_cast<const PlainArchiveEntry*>(&e);
    const auto data = input_file.stream.seek(entry->offset).read(entry->size);
    return std::make_unique<io::File>(entry->path, data);
}

static auto _ = dec::register_decoder<BscImageArchiveDecoder>("bishop/bsc");
