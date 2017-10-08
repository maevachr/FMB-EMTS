#include "Terrain.h"
#include <algorithm>
#include <iostream>

using namespace std;
using namespace DirectX;

void Terrain::BuildVertices(const HeightMapFile& file) {
	vertices.reserve(file.data.size());
	int index{};
	for_each(file.data.begin(), file.data.end(), [&](auto p) {
		vertices.push_back({ GeneratePosition(index++, p), {} });
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
	for_each(vertices.begin(), vertices.end(), [&](Vertex& v) {
		v.normal = GenerateNormal(index++);
	});
}

XMVECTOR Terrain::GenerateNorthVector(int index) const
{
	return XMLoadFloat4(&vertices[index - info.X].position) - XMLoadFloat4(&vertices[index].position);
}

XMVECTOR Terrain::GenerateSouthVector(int index) const
{
	return XMLoadFloat4(&vertices[index + info.X].position) - XMLoadFloat4(&vertices[index].position);
}

XMVECTOR Terrain::GenerateEastVector(int index) const
{
	return XMLoadFloat4(&vertices[index + 1].position) - XMLoadFloat4(&vertices[index].position);
}

XMVECTOR Terrain::GenerateWestVector(int index) const
{
	return XMLoadFloat4(&vertices[index - 1].position) - XMLoadFloat4(&vertices[index].position);
}

XMFLOAT4 Terrain::GeneratePosition(int index, float z) const
{
	return { info.Dx * (index % info.X), info.Dy * (index / info.X), z, 1.0 };
}

XMFLOAT4 Terrain::GenerateNormal(int index) const
{
	auto edge = XMVectorSet(0, 0, 1, 0);
	bool north = VertexHasNorth(index);
	bool south = VertexHasSouth(index);
	bool east = VertexHasEast(index);
	bool west = VertexHasWest(index);

	XMVECTOR v3 = north ? GenerateNorthVector(index) : XMVECTOR{};
	XMVECTOR v1 = south ? GenerateSouthVector(index) : XMVECTOR{};
	XMVECTOR v2 = east ? GenerateEastVector(index) : XMVECTOR{};
	XMVECTOR v4 = west ? GenerateWestVector(index) : XMVECTOR{};

	XMVECTOR n1 = south && east ? XMVector3Normalize(XMVector3Cross(v2, v1)) : edge;
	XMVECTOR n2 = east && north ? XMVector3Normalize(XMVector3Cross(v3, v2)) : edge;
	XMVECTOR n3 = north && west ? XMVector3Normalize(XMVector3Cross(v4, v3)) : edge;
	XMVECTOR n4 = west && south ? XMVector3Normalize(XMVector3Cross(v1, v4)) : edge;

	n1 = (n1 + n2 + n3 + n4) / 4;

	XMFLOAT4 result;
	XMStoreFloat4(&result, n1);
	return result;
}

ostream& operator<<(ostream& os, const Terrain & t)
{
	if (os) {
		os.write((char*)&t.GetInfo(), sizeof(TerrainFileHeader));
		os.write((char*)&t.GetVertices().begin(), t.GetVertices().size() * sizeof(Vertex));
		os.write((char*)&t.GetTriangles().begin(), t.GetTriangles().size() * sizeof(Triangle));
	}

	return os;
}

istream& operator>>(istream& is, TerrainFileHeader& t)
{
	if (is) {
		is >> t.X;
		is >> t.Y;
		is >> t.Dx;
		is >> t.Dy;
	}

	return is;
}