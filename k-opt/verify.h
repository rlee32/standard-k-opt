#pragma once

#include "DistanceTable.h"
#include "Segment.h"
#include "primitives.h"

#include <algorithm>
#include <iostream>

namespace verify {

bool valid_cycle(const Segment::Container& segments)
{
    std::vector<primitives::point_id_t> next;
    next.resize(segments.size());
    std::vector<primitives::point_id_t> next_count;
    next_count.resize(segments.size());
    std::fill(next_count.begin(), next_count.end(), 0);
    for (const auto& s : segments)
    {
        next[s.a] = s.b;
        ++next_count[s.b];
    }
    bool next_error{false};
    for (primitives::point_id_t i{0}; i < next_count.size(); ++i)
    {
        if (next_count[i] != 1)
        {
            std::cout << "ERROR: edges go toward point " << i
                << " " << next_count[i] << " times (should only be 1)." << std::endl;
            next_error = true;
        }
    }
    if (next_error)
    {
        return false;
    }
    std::vector<int> visited;
    visited.resize(segments.size());
    std::fill(visited.begin(), visited.end(), 0);
    constexpr primitives::point_id_t start_point{0};
    primitives::point_id_t current{start_point};
    do
    {
        if (visited[current] > 0)
        {
            std::cout << __func__ << ": ERROR: city revisited." << std::endl;
            return false;
        }
        visited[current] = true;
        current = next[current];
    } while (current != start_point);
    for (primitives::point_id_t i{0}; i < visited.size(); ++i)
    {
        if (visited[i] < 1)
        {
            std::cout << __func__ << ": ERROR: city " << i << " not visited." << std::endl;
            {
                for (const auto& s : segments)
                {
                    if (s.a == i or s.b == i)
                    {
                        std::cout << "segment with " << i << ": " << s.a << " " << s.b << std::endl;
                    }
                }
            }
            return false;
        }
    }
    return true;
}

primitives::length_t tour_length(const Segment::Container& segments, const DistanceTable& dt)
{
    primitives::length_t length{0};
    for (const auto& s : segments)
    {
        length += dt.lookup_length(s.a, s.b);
    }
    return length;
}

void probe(const Segment::Container& segments, primitives::point_id_t point)
{
    std::cout << "Segments with point " << point << ":" << std::endl;
    for (const auto& s : segments)
    {
        if (s.a == point or s.b == point)
        {
            std::cout << s << std::endl;
        }
    }
}

} // namespace verify
