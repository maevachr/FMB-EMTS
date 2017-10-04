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
	const std::vector<Vertex> GetVertices() const { return vertices; };
	const std::vector<Triangle> GetTriangles() const { return triangles; };

private:
	const TerrainFileHeader& info;
	std::vector<Vertex> vertices;
	std::vector<Triangle> triangles;

	void BuildVertices(const HeightMapFile& file);
	void BuildTriangles();
	void BuildNormals();
	DirectX::XMFLOAT4 GetPosition(const TerrainFileHeader & info, int index, std::vector<float> vertices) const {
		return { info.Dx * (index % info.X), info.Dy * (index / info.X), vertices[index], 1.0 };
	}
};