#include "Terrain.h"
#include <iterator>
#include <iostream>
#include <fstream>

using namespace std;

HeightMapFile Read(string fileName) {
	HeightMapFile file{};
	ifstream input(fileName);

	if (input)
	{
		input >> file.header;
		copy(istream_iterator<float>(input), istream_iterator<float>(), back_inserter(file.data));
	}
	else
	{
		throw runtime_error("Could not open input file");
	}
	return file;
}

HeightMapFile Read() {
	return Read("Entree.txt");
}

void Write(string fileName, const Terrain& terrain){
	ofstream output(fileName);
	if (output)
		output << terrain;
	else
		throw runtime_error("Could not open output file");
}

void Write(const Terrain& terrain) {
	Write("Sortie.txt", terrain);
}

/*
 * Program arguments formats 
 * ./executable 
 * ./executable input_file
 * ./executable input_file output_file
 * ./executable input_file output_file
 */
int main(int argc, char* argv[]) {
	HeightMapFile file{};

	if (argc > 1) 
		file = Read(argv[1]);
	else 
		file = Read();
	
	Terrain t { file };

	if (argc > 2) 
		Write(argv[2], t);
	else 
		Write(t);

	return 0;
}

