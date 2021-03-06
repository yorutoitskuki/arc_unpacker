#include "dec/wild_bug/wpx/retrieval.h"
#include "algo/range.h"
#include "err.h"

using namespace au;
using namespace au::dec::wild_bug::wpx;

static std::vector<u8> build_dict()
{
    std::vector<u8> dict(0x100 * 0x100);
    for (const auto i : algo::range(0x100))
    {
        u8 n = -1 - i;
        for (const auto j : algo::range(0x100))
            dict[0x100 * i + j] = n--;
    }
    return dict;
}

static std::vector<u8> build_table(io::BaseBitStream &bit_stream)
{
    std::vector<u8> sizes(0x100);
    for (const auto n : algo::range(0, 0x100, 2))
    {
        sizes[n + 1] = bit_stream.read(4);
        sizes[n] = bit_stream.read(4);
    }

    std::vector<u8> table(0x10000);
    for (const auto n : algo::range(0x100))
    {
        auto size = sizes[n];
        if (!size)
            continue;
        u16 index = bit_stream.read(size);
        index <<= 15 - size;
        index &= 0x7FFF;
        table[2 * index] = size;
        table[2 * index + 1] = n;
    }
    return table;
}

static int find_table_index(
    const std::vector<u8> &table, io::BaseBitStream &bit_stream)
{
    auto index = 0;
    u8 n = 0;
    auto mask = 0x4000;
    while (true)
    {
        n++;
        if (bit_stream.read(1))
            index |= mask;
        if (n == table[2 * index])
            break;
        if (!(mask >>= 1))
            throw err::CorruptDataError("Failed to locate table value");
    }
    return index;
}

RetrievalStrategy1::RetrievalStrategy1(
    io::BaseBitStream &bit_stream, s8 quant_size)
    : IRetrievalStrategy(bit_stream), quant_size(quant_size)
{
    dict = build_dict();
    table = build_table(bit_stream);
}

u8 RetrievalStrategy1::fetch_byte(DecoderContext &context, const u8 *output_ptr)
{
    auto start_pos = output_ptr[-quant_size] << 8;
    auto index = find_table_index(table, context.bit_stream);
    auto size = table[2 * index + 1];
    auto value = dict[start_pos + size];
    while (size)
    {
        dict[start_pos + size] = dict[start_pos + size - 1];
        size--;
    }
    dict[start_pos] = value;
    return value;
}

RetrievalStrategy2::RetrievalStrategy2(io::BaseBitStream &bit_stream)
    : IRetrievalStrategy(bit_stream)
{
    table = build_table(bit_stream);
}

u8 RetrievalStrategy2::fetch_byte(
    DecoderContext &context, const u8 *output_ptr)
{
    auto index = find_table_index(table, context.bit_stream);
    return table[2 * index + 1];
}

RetrievalStrategy3::RetrievalStrategy3(io::BaseBitStream &bit_stream)
    : IRetrievalStrategy(bit_stream)
{
}

u8 RetrievalStrategy3::fetch_byte(
    DecoderContext &context, const u8 *output_ptr)
{
    return context.input_stream.read<u8>();
}
