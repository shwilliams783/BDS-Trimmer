#include <fstream>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include <string.h>
#include <list>
#include <time.h>

int main(int argc, char ** argv)
{
	if (argc < 4)
	perror("Usage:\n  output filename.out dataTrim.gml comp*.nn "); 

	FILE *input;
	std::fstream output;
	output.open(argv[1], std::ios::out);
	char buffer[50];
	int numVertices;
	int nextVertex;
	
	
	//std::cout << "argc = " << argc << std::endl;
	if ((input = fopen(argv[2], "r")) == NULL)
		perror("File could not be opened.\n");
	
	fscanf(input, "%s", &buffer);
	fscanf(input, "%s", &buffer);
	fscanf(input, "%s", &buffer);
	numVertices = atoi(buffer);
	fclose(input);
	
	//std::cout << numVertices << std::endl;
	int *id; // dynamic memory allocation
	if ((id = new int[numVertices]) == NULL)
		perror("memory not allocated");
	
	// all vertices default to k+1 value (singletons)
	for (int i = 0; i < numVertices; i++)
	{
		id[i] = (argc - 2);
		//std::cout << id[i] << " " << std::endl;
	}		
	
	// scan comp files and assign cluster numbers to each vertex in id
	for(int i = 3; i < argc; i++)
	{
		if ((input = fopen(argv[i], "r")) == NULL)
			perror("File could not be opened.\n");
		fscanf(input, "%s", &buffer); // "<number>"
		fscanf(input, "%s", &buffer); // "vertices"
		do // all remaining inputs are vertex ids, but .nn files store their values at 1 less than the original
		{
			fscanf(input, "%s", &buffer);
			//std::cout << buffer << std::endl;
			if(feof(input))
				break;
			nextVertex = atoi(buffer);
			id[nextVertex] = i-2;
		}while(1);
	}
	
	
	
	// output to file
	output << (argc - 3) << " clusters:" << std::endl;
	
	for (int i = 0; i < numVertices; i++)
	{
		output << id[i] << " ";
	}	
	
	std::cout << "output complete!" << std::endl;
	
	return 1;	
}