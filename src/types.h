#ifndef TYPES_H
#define TYPES_H
#include <cstdint>
#include <cstddef>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

#ifdef VISUAL_STUDIO_2015_CAME_OUT
    constexpr u8 operator "" _u8(char value)
    {
        return static_cast<u8>(value);
    }

    constexpr const u8* operator "" _u8(const char *value, size_t n)
    {
        return reinterpret_cast<const u8 *>(value);
    }
#endif

#endif
