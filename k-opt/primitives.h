#pragma once

// Aliases and constants for primitive types.

#include <cstdint>
#include <limits>

namespace primitives {

using length_t = uint64_t; // as in Segment lengths.
using point_id_t = uint32_t;
using space_t = double; // as in x, y coordinates.

constexpr auto InvalidPoint = std::numeric_limits<point_id_t>::max();

} // namespace primitives

