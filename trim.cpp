#include "trim.h"

int main(int argc, char ** argv)
{
	if (argc != 3)
	perror("Usage:\n  trim input.gml output.gml"); 

	FILE *input;
	FILE *output;
	char string[50];
	int numNodes = 0;
	int numEdges = 0;
	node_t* newNode = new node_t;
	std::list<node_t> nodes;
	std::list<node_t>::iterator nodeIt = nodes.begin();
  
	if (((input = fopen(argv[1], "r")) == NULL) || ((output = fopen(argv[2], "w"))
	== NULL))
    perror("File could not be opened.\n");
	
	// Skip to graph declaration
	
	while (1) { // throw away everything until get to "graph" declaration
	if(feof(input)) 
		perror("no graph to read in input file");

	fscanf(input, "%s", string); // read in string from file

	if(strncmp(string, "graph", 5) == 0) 
		break; // stop when get to "graph" declaration
	}
	
	// Read and store each vertex as a node in nodes; store original ID as well
	
	while (1) { // throw away everything until get to "edge" declaration
		if(feof(input))
			perror("no edges to read in input file");

		fscanf(input, "%s", string); // read in string from file

		if(strncmp(string, "id", 2) == 0) { 
			fscanf(input, "%s", string); // read in id value
			newNode->originalID = atoi(string); //assign id value to new node
			numNodes++; // one more node found
			//std::cout<<"originalId = "<<newNode->originalID<<" numNodes = "<<numNodes<<std::endl;
			nodes.push_back(*newNode); // add newNode to nodes list
			nodeIt++; // increment node list iterator
			//std::cout<<"nodeIt.originalID = "<<(*nodeIt).originalID<<" nodes.size = "<<nodes.size()<<std::endl;
		}
			

		if(strncmp(string, "edge", 4) == 0) 
			break; // stop when read in first "edge"
	}
	
	// Read edges and store neighboring vertices in edges list
	
	while (!feof(input)) { // read edges until reach end of file
		fscanf(input, "%s", string); // read in string from file

		if(!strncmp(string, "[", 1) == 0) // check for correct character
			perror("No '[' after edge declaration");

		fscanf(input, "%s", string); // read in string from file
		if(!strncmp(string, "source", 6) == 0) // check for correct character
			perror("No 'source' declaration");

		fscanf(input, "%s", string); // read in start node from file
		int source = atoi(string);

		fscanf(input, "%s", string); // read in string from file
		if(!strncmp(string, "target", 6) == 0) // check for correct character
			perror("No 'target' declaration");

		fscanf(input, "%s", string); // read in target node from file
		int target = atoi(string);
		
		std::list<node_t>::iterator edgeIt;
		for(edgeIt = nodes.begin(); edgeIt != nodes.end(); edgeIt++)
		{
			if((*edgeIt).originalID == source)
			{
				(*edgeIt).edges.push_back(target);
				//std::cout << target << " added to source edge list." << std::endl;
			}
			if((*edgeIt).originalID == target)
			{
				(*edgeIt).edges.push_back(source);
				//std::cout << source << " added to target edge list." << std::endl;
			}				
		}

		fscanf(input, "%s", string); // read in string from file

		float weight = 1.0;
		if(strncmp(string, "weight", 6) == 0) {// check to see if weight specified
			fscanf(input, "%s", string); // read in string from file
			weight = atof(string);
			fscanf(input, "%s", string); // read in string from file
		}
		
		numEdges++; // count number of edges

		if(numEdges % 10000000 == 0) // message every 10 million edges
			std::cout << numEdges / 1000000 << " million edges read" << std::endl;

		if(!strncmp(string, "]", 1) == 0) // check for correct character
			perror("No ']' after edge declaration");
			
		if (feof(input))
			break; // break is at end of file

		fscanf(input, "%s", string); // read in string from file

		if(strncmp(string, "]", 1) == 0) // check for end of graph
			break;

		if(!strncmp(string, "edge", 4) == 0) // check for correct character
			perror("No 'edge' declaration");
		
	} 

	fclose(input);
	
	/* Node and edge display
	for(nodeIt = nodes.begin(); nodeIt != nodes.end(); nodeIt++)
	{
		std::cout << "nodeIt.originalID = " << (*nodeIt).originalID << ", edges: ";
		std::list<int>::iterator edgeIt;
		for(edgeIt = (*nodeIt).edges.begin(); edgeIt != (*nodeIt).edges.end(); edgeIt++)
		{
			std::cout << (*edgeIt) << ", ";
		}
		std::cout << std::endl;
	}*/

	// Search for nodes containing only one edge
	int prevNode;
	int neighbor;
	
	std::list<node_t>::iterator terminalIt;
	std::list<node_t>::iterator targetIt;
	for(terminalIt = nodes.begin(); terminalIt != nodes.end(); terminalIt++)
	{
		if((*terminalIt).edges.size() == 1) // this is a terminal vertex
		{
			//std::cout << "Terminal vertex found at ID = " << (*terminalIt).originalID << std::endl;
			prevNode = (*terminalIt).originalID; // store the first ID of the terminal vertex
			neighbor = (*terminalIt).edges.front(); // the first neighbor will be the ID of the only vertex in the edges list of the terminal vertex
			
			// the edge to be removed will be between neighbor and prevNode once the "linear cluster" reaches a proper cluster or another terminal vertex
			targetIt = nodes.begin(); // initialize the target iterator
			advance(targetIt, neighbor-1); // advance target iterator to the neighbor node in the nodes list
			
			//std::cout << "BEFORE WHILE targetIt.originalID " << (*targetIt).originalID << " and edges.size() = " << (*targetIt).edges.size() << std::endl;
			
			// check the number of edges in the neighboring node until a node
			// with exactly 1 edge, or 3+ edges is found:
			// case 1: do nothing; the cluster is completely linear.
			// case 3+: remove the edge beween the 3+ edges node and the prevNode 
			while((*targetIt).edges.size() == 2)
			{
				if ((*targetIt).edges.front() != prevNode)
				{
					prevNode = neighbor;
					neighbor = (*targetIt).edges.front();
				}
				else
				{
					prevNode = neighbor;
					neighbor = (*targetIt).edges.back();
				}
				targetIt = nodes.begin();
				advance(targetIt, neighbor-1);
				//std::cout << "targetIt.originalID " << (*targetIt).originalID << " and edges.size() = " << (*targetIt).edges.size() << std::endl;
			}
			// at this point, there is either a cluster or a linear terminal
			//at the neighbor vertex
			
			if((*targetIt).edges.size() >= 3) // neighbor is a cluster
			{
				std::list<node_t>::iterator edgeIt;
				for(edgeIt = nodes.begin(); edgeIt != nodes.end(); edgeIt++)
				{
					if((*edgeIt).originalID == neighbor)
					{
						std::list<int>::iterator cutIt;
						for(cutIt = (*edgeIt).edges.begin(); cutIt != (*edgeIt).edges.end(); cutIt++)
						{
							if((*cutIt) == prevNode)
							{
								cutIt = (*edgeIt).edges.erase(cutIt);
							}
						}
						numEdges--;
						std::cout << "cut edge from " << neighbor << " to " << prevNode << std::endl;
					}	
					else if((*edgeIt).originalID == prevNode)
					{
						std::list<int>::iterator cutIt;
						for(cutIt = (*edgeIt).edges.begin(); cutIt != (*edgeIt).edges.end(); cutIt++)
						{
							if((*cutIt) == neighbor)
							{
								cutIt = (*edgeIt).edges.erase(cutIt);
							}
						}
						//std::cout << "cut edge from " << prevNode << " to " << neighbor << std::endl;
					}	
				}
				
			}
			//std::cout << "end linear trim" << std::endl;
			
			/* Node and edge display
			for(nodeIt = nodes.begin(); nodeIt != nodes.end(); nodeIt++)
			{
				std::cout << "nodeIt.originalID = " << (*nodeIt).originalID << ", edges: ";
				std::list<int>::iterator edgeIt;
				for(edgeIt = (*nodeIt).edges.begin(); edgeIt != (*nodeIt).edges.end(); edgeIt++)
				{
					std::cout << (*edgeIt) << ", ";
				}
				std::cout << std::endl;
			}*/
		}
		//std::cout << "end terminal search iteration" << std::endl;
	}
	
	// save remaining files in a new .gml file
	fprintf(output,"Graph with %d nodes and %d edges.\ngraph\n[", numNodes, numEdges);
	
	std::list<node_t>::iterator saveIt;
	for(saveIt = nodes.begin(); saveIt != nodes.end(); saveIt++)
	{
		fprintf(output, "\n  node\n  [\n    id %d\n  ]", (*saveIt).originalID);
	}
	
	for(saveIt = nodes.begin(); saveIt != nodes.end(); saveIt++)
	{
		std::list<int>::iterator edgeIt;
		for(edgeIt = (*saveIt).edges.begin(); edgeIt != (*saveIt).edges.end(); edgeIt++)
		{
			if((*saveIt).originalID < (*edgeIt))
				fprintf(output, "\n  edge\n  [\n    source %d\n    target %d\n  ]", (*saveIt).originalID, (*edgeIt));
		}
	}
	
	fprintf(output, "\n]\n");
	
	
	return 1;	
}