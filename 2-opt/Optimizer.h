#pragma once

// Parallelization is easily achieved if each thread instantiates an Optimizer.

#include "SearchState.h"
#include "distance_functions.h"
#include "primitives.h"

#include <algorithm>
#include <ostream>
#include <vector>

class Optimizer
{
    using SegmentContainer = std::vector<Segment>;
public:
    Optimizer(const std::vector<primitives::space_t>& x, const std::vector<primitives::space_t>& y)
        : m_x(x), m_y(y) {}

    void find_best(const std::vector<Segment>& segments);
    void find_best(std::vector<Segment>::const_iterator it, const std::vector<Segment>::const_iterator end);
    const SearchState& best() const { return m_best; }

private:
    const std::vector<primitives::space_t>& m_x;
    const std::vector<primitives::space_t>& m_y;

    const size_t m_k{2}; // as in k-opt.
    SearchState m_best;
    SearchState m_current;

    void check_best();
};

inline std::ostream& operator<<(std::ostream& out, const Optimizer& optimizer)
{
    out << "Best swap found:\n";
    out << "\tPoint ID of each segment:\n";
    for (const auto& s : optimizer.best().segments)
    {
        out << "\t" << s.a << "\t" << s.b << "\n";
    }
    out << "\tOld length: " << optimizer.best().length << "\n";
    out << "\tImprovment: " << optimizer.best().improvement << "\n";
    return out;
}

