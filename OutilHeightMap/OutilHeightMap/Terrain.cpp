#include "Terrain.h"
#include <algorithm>
#include <iostream>

using namespace std;

void Terrain::BuildVertices(const HeightMapFile& file) {
	vertices.reserve(file.data.size());
	int index{};
	for_each(file.data.begin(), file.data.end(), [&](auto p) {
		vertices.push_back({ GetPosition(info, index++, p), {} });
	});
}

void Terrain::BuildTriangles() {
	int X = info.X;
	int Y = info.Y;
	triangles.reserve((X - 1) * (Y - 1));
	int i{};

	for (int y{}; y < Y - 1; ++y) {
		for (int x{}; x < X - 1; ++x) {
			triangles.push_back({ y*X + x, (y + 1)*X + (x + 1), y*X + (x + 1) });
			triangles.push_back({ y*X + x, (y + 1)*X + x, (y + 1)*X + (x + 1) });
		}
	}
}

void Terrain::BuildNormals()
{
	int index{};
	for_each(vertices.begin(), vertices.end(), [&](auto v) {
		v.normal = { 0.f, 0.f, 1.f, 0.f };
	});
}

std::ostream& operator<<(std::ostream& os, const Terrain & t)
{
	if (os) {
		os.write((char*)&t.GetInfo(), sizeof(TerrainFileHeader));
		os.write((char*)&t.GetVertices().begin(), t.GetVertices().size() * sizeof(Vertex));
		os.write((char*)&t.GetTriangles().begin(), t.GetTriangles().size() * sizeof(Triangle));
	}

	return os;
}

std::istream& operator>>(std::istream& is, TerrainFileHeader& t)
{
	if (is) {
		is >> t.X;
		is >> t.Y;
		is >> t.Dx;
		is >> t.Dy;
	}

	return is;
}