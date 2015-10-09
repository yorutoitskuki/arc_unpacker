#include "fmt/bgi/arc_archive_decoder.h"
#include "fmt/bgi/audio_decoder.h"
#include "fmt/bgi/cbg_image_decoder.h"
#include "fmt/bgi/dsc_file_decoder.h"
#include "util/range.h"

using namespace au;
using namespace au::fmt::bgi;

static const bstr magic = "PackFile\x20\x20\x20\x20"_b;

namespace
{
    struct ArchiveEntryImpl final : fmt::ArchiveEntry
    {
        size_t offset;
        size_t size;
    };
}

struct ArcArchiveDecoder::Priv final
{
    CbgImageDecoder cbg_image_decoder;
    DscFileDecoder dsc_file_decoder;
    AudioDecoder audio_decoder;
};

ArcArchiveDecoder::ArcArchiveDecoder() : p(new Priv)
{
    add_decoder(&p->cbg_image_decoder);
    add_decoder(&p->dsc_file_decoder);
    add_decoder(&p->audio_decoder);
}

ArcArchiveDecoder::~ArcArchiveDecoder()
{
}

bool ArcArchiveDecoder::is_recognized_impl(File &arc_file) const
{
    return arc_file.io.read(magic.size()) == magic;
}

std::unique_ptr<fmt::ArchiveMeta>
    ArcArchiveDecoder::read_meta_impl(File &arc_file) const
{
    arc_file.io.seek(magic.size());
    auto file_count = arc_file.io.read_u32_le();
    auto file_data_start = arc_file.io.tell() + file_count * 32;
    auto meta = std::make_unique<ArchiveMeta>();
    for (auto i : util::range(file_count))
    {
        auto entry = std::make_unique<ArchiveEntryImpl>();
        entry->name = arc_file.io.read_to_zero(16).str();
        entry->offset = arc_file.io.read_u32_le() + file_data_start;
        entry->size = arc_file.io.read_u32_le();
        arc_file.io.skip(8);
        meta->entries.push_back(std::move(entry));
    }
    return meta;
}

std::unique_ptr<File> ArcArchiveDecoder::read_file_impl(
    File &arc_file, const ArchiveMeta &m, const ArchiveEntry &e) const
{
    auto entry = static_cast<const ArchiveEntryImpl*>(&e);
    arc_file.io.seek(entry->offset);
    auto data = arc_file.io.read(entry->size);
    auto output_file = std::make_unique<File>(entry->name, data);
    output_file->guess_extension();
    return output_file;
}

static auto dummy = fmt::register_fmt<ArcArchiveDecoder>("bgi/arc");
