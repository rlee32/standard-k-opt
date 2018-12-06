#pragma once

// Parallelization is easily achieved if each thread instantiates an Optimizer.

#include "DistanceTable.h"
#include "SearchState.h"
#include "primitives.h"

#include <algorithm>
#include <iterator>
#include <ostream>
#include <vector>

class Optimizer
{
public:
    Optimizer(const DistanceTable& dt) : m_dt(dt) {}

    void find_best(const Segment::Container& segments);

    const SearchState& best() const { return m_best; }

private:
    const DistanceTable& m_dt;

    const size_t m_k{2}; // as in k-opt.
    SearchState m_best;
    SearchState m_current;
    int m_calls{0};

    void find_best(Segment::Container::const_iterator it, const Segment::Container::const_iterator end);
    void check_best();
};

inline std::ostream& operator<<(std::ostream& out, const Optimizer& optimizer)
{
    if (optimizer.best().empty())
    {
        return out << "No improving swap found.";
    }
    out << "Best swap found:\n";
    out << "\tPoint ID of each segment:\n";
    for (const auto& s : optimizer.best().segments)
    {
        out << "\t" << s.a << "\t" << s.b << "\n";
    }
    out << "\tOld length: " << optimizer.best().length << "\n";
    return out << "\tImprovement: " << optimizer.best().improvement;
}

