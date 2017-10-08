#pragma once
#include <vector>
#include <DirectXMath.h>
#include "HeightMapFile.h"

struct Vertex {
	DirectX::XMFLOAT4 position{};
	DirectX::XMFLOAT4 normal{};
};

struct Triangle {
	int i1{};
	int i2{};
	int i3{};
};

class Terrain {
public:
	Terrain(const HeightMapFile& file) : info(file.header) {
		BuildVertices(file);
		BuildTriangles();
		BuildNormals();
	};

	friend std::ostream& operator<<(std::ostream&, const Terrain&);

	const TerrainFileHeader& GetInfo() const { return info; };
	const std::vector<Vertex>& GetVertices() const { return vertices; };
	const std::vector<Triangle>& GetTriangles() const { return triangles; };

private:
	const TerrainFileHeader& info;
	std::vector<Vertex> vertices;
	std::vector<Triangle> triangles;

	void BuildVertices(const HeightMapFile& file);
	void BuildTriangles();
	void BuildNormals();

	bool VertexHasNorth(int index) const
	{
		return index >= info.X;
	}
	bool VertexHasSouth(int index) const
	{
		return index < (info.X * info.Y) - info.X;
	}
	bool VertexHasEast(int index) const
	{
		return index%info.X != info.X - 1;
	}
	bool VertexHasWest(int index) const
	{
		return index%info.X != 0;
	}

	DirectX::XMVECTOR GenerateNorthVector(int index) const;
	DirectX::XMVECTOR GenerateSouthVector(int index) const;
	DirectX::XMVECTOR GenerateEastVector(int index) const;
	DirectX::XMVECTOR GenerateWestVector(int index) const;

	DirectX::XMFLOAT4 GeneratePosition(int index, float z) const;
	DirectX::XMFLOAT4 GenerateNormal(int index) const;
};