#include "DistanceTable.h"
#include "Optimizer.h"
#include "PointSequence.h"
#include "Segment.h"
#include "fileio/PointSet.h"
#include "fileio/Tour.h"
#include "primitives.h"
#include "verify.h"

#include <iostream>
#include <vector>

int main(int argc, char** argv)
{
    // Read points.
    if (argc < 2)
    {
        std::cout << "Arguments: point_set_file_path optional_tour_file_path" << std::endl;
        return 0;
    }
    fileio::PointSet point_set(argv[1]);
    // Initial tour.
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
    // Initialize distance table.
    PointSequence point_sequence(tour);
    DistanceTable dt(point_set.x(), point_set.y());
    // Initialize segments.
    const auto& next = point_sequence.next();
    Segment::Container segments;
    for (auto id : tour)
    {
        auto length = dt.lookup_length(id, next[id]);
        segments.insert({id, next[id], length});
    }
    // Hill climbing optimization loop.
    Optimizer optimizer(dt);
    auto prev_length = verify::tour_length(segments, dt);
    std::cout << "Initial tour length: " << prev_length << std::endl;
    int iteration{1};
    primitives::length_t improvement{1};
    constexpr bool debug_mode{false};
    constexpr int print_period{1};
    std::cout << "Starting hill-climbing optimization:\n";
    const auto n = point_set.x().size();
    std::cout << "Expecting " << (n -1) * (n - 2) / 2 - 1 << " checks per best-improvement iteration.\n";
    while (improvement > 0)
    {
        optimizer.find_best(segments);
        point_sequence.new_tour(segments, optimizer.best().segments, optimizer.best().new_segments);
        if (debug_mode)
        {
            std::cout << optimizer << std::endl;
            if (verify::valid_cycle(segments))
            {
                std::cout << "Tour is still a valid cycle." << std::endl;
            }
            else
            {
                std::cout << "ERROR: tour has become invalid!" << std::endl;
                break;
            }
        }
        if (iteration % print_period == 0)
        {
            auto current_length = verify::tour_length(segments, dt);
            improvement = prev_length - current_length;
            std::cout << "Iteration " << iteration
                << " final tour length: " << current_length
                << " (improvement since last update: " << improvement << ")\n";
            prev_length = current_length;
        }
        ++iteration;
    }
    if (improvement == 0)
    {
        std::cout << "Local optimum reached." << std::endl;
    }
    else
    {
        std::cout << "Hill-climbing optimization prematurely stopped." << std::endl;
    }
    return 0;
}
