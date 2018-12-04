#include "Optimizer.h"
#include "PointSequence.h"
#include "distance_functions.h"
#include "fileio/PointSet.h"
#include "fileio/Tour.h"
#include "primitives.h"

#include <iostream>
#include <vector>

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        std::cout << "Arguments: point_set_file_path optional_tour_file_path" << std::endl;
        return 0;
    }
    fileio::PointSet point_set(argv[1]);
    Optimizer optimizer(point_set.x(), point_set.y());
    // Initialize tour.
    std::vector<primitives::point_id_t> tour;
    if (argc > 2)
    {
        fileio::Tour initial_tour(argv[2]);
        tour = initial_tour.point_ids();
    }
    else
    {
        for (primitives::point_id_t i{0}; i < point_set.count(); ++i)
        {
            tour.push_back(i);
        }
    }
    const auto length = point_set.cycle_length(tour);
    std::cout << "Initial tour length: " << length << std::endl;
    PointSequence point_sequence(tour);
    const auto& next = point_sequence.next();
    std::vector<Segment> segments;
    for (auto id : tour)
    {
        auto length = distance_functions::euc2d(point_set.x(), point_set.y(), id, next[id]);
        segments.push_back({id, next[id], length});
    }
    optimizer.find_best(segments);
    std::cout << optimizer << std::endl;
    return 0;
}
