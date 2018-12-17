#include "Optimizer.h"

void Optimizer::find_best(const Segment::Container& segments)
{
    m_best = SearchState();
    m_calls = 0;
    for (auto it = segments.cbegin(); it != segments.cend(); ++it)
    {
        m_current = SearchState(*it);
        find_best(std::next(it), segments.cend());
    }
    std::cout << m_k << "-opt checks: " << m_calls << std::endl;
}

void Optimizer::find_best(Segment::Container::const_iterator it, const Segment::Container::const_iterator end)
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
    ++m_calls;
    auto ordered_segments = m_current.segments;
    std::sort(std::begin(ordered_segments)
        , std::end(ordered_segments)
        , [&m_sequence_ids = m_sequence_ids](const Segment& a, const Segment& b) { return m_sequence_ids[a.a] < m_sequence_ids[b.a]; });
    auto& new_segments = m_current.new_segments;
    if (new_segments.size() != m_k)
    {
        new_segments.resize(m_k);
    }
    switch (m_k)
    {
        case 2:
        {
            check_best_2opt(ordered_segments);
        } break;
        case 3:
        {
            opt::check_best_3opt(ordered_segments, m_current, m_best, m_dt);
        } break;
        case 4:
        {
            opt::check_best_4opt(ordered_segments, m_current, m_best, m_dt);
        } break;
        // TODO: implement for higher m_k.
        default:
        {
        } break;
    }
}

void Optimizer::check_best_2opt(const std::vector<Segment>& ordered_segments)
{
    const auto& s = ordered_segments;
    auto& ns = m_current.new_segments;
    ns.clear();
    ns.push_back({s[0].a, s[1].a, m_dt.lookup_length(s[0].a, s[1].a)});
    if (ns[0].length >= m_current.length)
    {
        return;
    }
    ns.push_back({s[0].b, s[1].b, m_dt.lookup_length(s[0].b, s[1].b)});
    auto new_length = ns[0].length + ns[1].length;
    if (new_length >= m_current.length)
    {
        return;
    }
    m_current.improvement = m_current.length - new_length;
    // Using >= or > below can make a difference in results!
    // It is possible to get multiple swap configurations with the same improvement.
    // All are qualitatively equivalent choices, but still can lead to divergence in later results.
    // The configuration chosen depends on the search path.
    // If >, the first configuration will be kept.
    // If >=, the last configuration will be chosen.
    if (m_current.improvement > m_best.improvement)
    {
        m_best = m_current;
    }
}

