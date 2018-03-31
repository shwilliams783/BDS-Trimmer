#ifndef TRIM_H
#define TRIM_H

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include <string.h>
#include <list>
#include <time.h>

struct node_t{
	int originalID; //original vertex identity from source gml file
	int numEdges; //used to measure linearity or terminal point
	std::list<int> edges; //used to dynamically track which vertices are neighbors
};

#endif