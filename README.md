# LinearTrimmer
This project adds two programs to supplement breadth-first search algorithm for creating clusters.

# makefile
A makefile has been included for the convenience of compilation

# trim.cpp

Trim.cpp "trims" terminal linear projections off of spherical clusters. Trimming these projections can result in linear clusters, doubletons, and singletons being dissociated from spherical clusters.

Usage:

<code>
./trim testFile.gml testFileTrim.gml
</code>

# bfsNet.cpp

Not included in this repository, as it was not created by me. Dr. Sharlee Climer wrote this program (and its associated sources and headers).

The provided makefile will also compile the needed files to run bfs, which sould be run on trimmed .gml files after using trim.

# output.cpp

output.cpp takes a given .gml data file and the comp*.nn files generated after running bfs and creates a .out file containing the number of clusters as well as a complete list of which vertices associate with each cluster. Singletons and doubleton vertices will display (k+1) to indicate few/no associations with other vertices.

Usage:
<code>
./output testFileTrim.out testFileTrim.gml comp*.nn
</code>
