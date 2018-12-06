C++ implementation of standard 2-opt hill-climbing for the traveling salesman problem (TSP).
This implementation is actually currently capable of 3-opt, and could be slightly simplified for 2-opt only.
This repo will be renamed standard-k-opt.

Input files are assumed to be in TSPLIB format.

Best-improvement 2-opt iterations require ((N - 1) * (N - 2) / 2 - 1) checks.

Compilation:
1. Make sure "CXX" in "2-opt/makefile" is set to the desired compiler.
2. Run "make" in "2-opt/".

Running:
1. Run "./2-opt.out" for usage details.

Style notes:
1. Namespaces follow directory structure. If an entire namespace is in a single header file, the header file name will be the namespace name.
2. Headers are grouped from most to least specific to this repo (e.g. repo header files will come before standard library headers).
