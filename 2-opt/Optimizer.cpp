#include "Optimizer.h"

void Optimizer::find_best(const std::vector<Segment>& segments)
{
    for (auto it = segments.cbegin(); it != segments.cend(); ++it)
    {
        m_current = SearchState(*it);
        find_best(it + 1, segments.cend());
    }
}

void Optimizer::find_best(std::vector<Segment>::const_iterator it, const std::vector<Segment>::const_iterator end)
{
    while (it != end)
    {
        if (not m_current.valid(*it))
        {
            ++it;
            continue;
        }
        m_current.push_back(*it);
        if (m_current.size() == m_k)
        {
            check_best();
            ++it;
        }
        else
        {
            find_best(++it, end);
        }
        m_current.pop_back();
    }
}

void Optimizer::check_best()
{
    // TODO: implement for m_k > 2.
    // TODO: distance table.
    const auto& s = m_current.segments;
    primitives::length_t new_length = distance_functions::euc2d(m_x, m_y, s[0].a, s[1].a);
    if (new_length > m_current.length)
    {
        return;
    }
    new_length += distance_functions::euc2d(m_x, m_y, s[0].b, s[1].b);
    if (new_length > m_current.length)
    {
        return;
    }
    m_current.improvement = m_current.length - new_length;
    if (m_current.improvement > m_best.improvement)
    {
        m_best = m_current;
    }
}

