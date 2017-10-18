#include "Terrain.h"
#include <iterator>
#include <iostream>
#include <fstream>
#include <algorithm>

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
	ofstream output(fileName, ios::binary);
	if (output)
		output << terrain;
	else
		throw runtime_error("Could not open output file");
}

void Write(const Terrain& terrain) {
	Write("Sortie.txt", terrain);
}

void Dump(const Terrain& terrain) {
	ofstream output("Dump.txt");
	if (output) {
		output << "X: " << terrain.GetInfo().X << " ";
		output << "Y: " << terrain.GetInfo().Y << " ";
		output << "Dx: " << terrain.GetInfo().Dx << " ";
		output << "Dy: " << terrain.GetInfo().Dy << endl << endl;

		int i{};
		for_each(terrain.GetVertices().begin(), terrain.GetVertices().end(), [&](Vertex v) {
			output << "Vertex: " << i++ << endl;
			output << "Position: " << v.position.x << " " << v.position.y << " " << v.position.z << " " << v.position.w << endl;
			output << "Normal: " << v.normal.x << " " << v.normal.y << " " << v.normal.z << " " << v.normal.w << endl << endl;
		});

		for_each(terrain.GetTriangles().begin(), terrain.GetTriangles().end(), [&](Triangle t) {
			output << "Triangle: " << t.i1 << " " << t.i2 << " " << t.i3 << endl << endl;
		});
	}
	else
		throw runtime_error("Could not open dump file");
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

	Dump(t);

	return 0;
}

