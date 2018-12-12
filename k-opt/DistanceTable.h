#pragma once

#include "primitives.h"

#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>

class DistanceTable
{
public:
    DistanceTable(const std::vector<primitives::space_t>& x, const std::vector<primitives::space_t>& y)
        : m_x(x), m_y(y)
    {
        std::cout << "Computing distance table...";
        auto point_count = m_x.size();
        m_table.resize(point_count - 1);
        for (primitives::point_id_t i{0}; i < m_table.size(); ++i)
        {
            size_t first_j = i + 1;
            m_table[i].resize(point_count - first_j);
            for (size_t j{first_j}; j < point_count; ++j)
            {
                m_table[i][j - first_j] = compute_euc2d(i, j);
            }
        }
        std::cout << " done.\n";
    }

    primitives::length_t lookup_length(primitives::point_id_t a, primitives::point_id_t b) const
    {
        return m_table[std::min(a, b)][std::max(a, b) - std::min(a, b) - 1];
    }
    primitives::length_t compute_length(primitives::point_id_t a, primitives::point_id_t b) const
    {
        return compute_euc2d(a, b);
    }

private:
    const std::vector<primitives::space_t>& m_x;
    const std::vector<primitives::space_t>& m_y;
    std::vector<std::vector<primitives::length_t>> m_table;

    primitives::length_t compute_euc2d(primitives::point_id_t a, primitives::point_id_t b) const
    {
        auto dx = m_x[a] - m_x[b];
        auto dy = m_y[a] - m_y[b];
        auto exact = std::sqrt(dx * dx + dy * dy);
        return exact + 0.5; // return type cast.
    }
};
