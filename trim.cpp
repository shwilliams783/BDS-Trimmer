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
	
	while (1) { // throw away everything until get to "graph" declaration
	if(feof(input)) 
		perror("no graph to read in input file");

	fscanf(input, "%s", string); // read in string from file

	if(strncmp(string, "graph", 5) == 0) 
		break; // stop when get to "graph" declaration
	}
	
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

		fscanf(input, "%s", string); // read in string from file

		float weight = 1.0;
		if(strncmp(string, "weight", 6) == 0) {// check to see if weight specified
			fscanf(input, "%s", string); // read in string from file
			weight = atof(string);
			fscanf(input, "%s", string); // read in string from file
		}
		
		else
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
			
		std::list<node_t>::iterator edgeIt;
		for(edgeIt = nodes.begin(); edgeIt != nodes.end(); edgeIt++)
		{
			if((*edgeIt).originalID == source)
			{
				(*edgeIt).edges.push_back(target);
				std::cout << target << " added to source edge list." << std::endl;
			}
			if((*edgeIt).originalID == target)
			{
				(*edgeIt).edges.push_back(source);
				std::cout << source << " added to target edge list." << std::endl;
			}				
		}
	}  

	return 1;	
}