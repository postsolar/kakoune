#ifndef ranked_match_hh_INCLUDED
#define ranked_match_hh_INCLUDED

#include "string.hh"
#include "meta.hh"

#include <cstdint>

namespace Kakoune
{

using UsedLetters = uint64_t;
UsedLetters used_letters(StringView str);

constexpr UsedLetters upper_mask = 0xFFFFFFC000000;

inline UsedLetters to_lower(UsedLetters letters)
{
    return ((letters & upper_mask) >> 26) | (letters & (~upper_mask));
}

struct RankedMatch
{
    RankedMatch(StringView candidate, StringView query, size_t input_order = 0);
    RankedMatch(StringView candidate, UsedLetters candidate_letters,
                StringView query, UsedLetters query_letters, size_t input_order = 0);

    const StringView& candidate() const { return m_candidate; }
    bool operator<(const RankedMatch& other) const;
    bool operator==(const RankedMatch& other) const { return m_candidate == other.m_candidate; }

    explicit operator bool() const { return m_matches; }

private:
    template<typename TestFunc>
    RankedMatch(StringView candidate, StringView query, TestFunc test, size_t input_order);

    enum class Flags : int
    {
        None = 0,
        // Order is important, the highest bit has precedence for comparison
        SingleWord       = 1 << 0,
        Contiguous       = 1 << 1,
        OnlyWordBoundary = 1 << 2,
        Prefix           = 1 << 3,
        BaseName         = 1 << 4,
        SmartFullMatch   = 1 << 5,
        FullMatch        = 1 << 6,
    };
    friend constexpr bool with_bit_ops(Meta::Type<Flags>) { return true; }

    StringView m_candidate{};
    bool m_matches = false;
    Flags m_flags = Flags::None;
    int m_word_boundary_match_count = 0;
    int m_max_index = 0;
    size_t m_input_order;
};

}

#endif // ranked_match_hh_INCLUDED
