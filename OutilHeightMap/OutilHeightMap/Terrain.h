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
	static DirectX::XMFLOAT4 GetPosition(const TerrainFileHeader & info, int index, float z) {
		return { info.Dx * (index % info.X), info.Dy * (index / info.X), z, 1.0 };
	}
	DirectX::XMFLOAT4 GetNormal(const TerrainFileHeader & info, int index) {
		using namespace DirectX;

		XMVECTOR n1;
		XMVECTOR n2;
		XMVECTOR n3;
		XMVECTOR n4;

		XMVECTOR v1;
		XMVECTOR v2;
		XMVECTOR v3;
		XMVECTOR v4;

		n1 = n2 = n3 = n4 = DirectX::XMVectorSet(0, 0, 1, 0);
		if (index < (info.X * info.Y) - info.X)  v1 = XMLoadFloat4(&vertices[index + info.X].position) - XMLoadFloat4(&vertices[index].position);
		if (index%info.X != info.X - 1)			 v2 = XMLoadFloat4(&vertices[index + 1].position) - XMLoadFloat4(&vertices[index].position);
		if (index >= info.X)					 v3 = XMLoadFloat4(&vertices[index - info.X].position) - XMLoadFloat4(&vertices[index].position);
		if (index%info.X != 0)					 v4 = XMLoadFloat4(&vertices[index - 1].position) - XMLoadFloat4(&vertices[index].position);

		if (index < (info.X * info.Y) - info.X    && index%info.X != info.X - 1)   n1 = XMVector3Cross(v2, v1);
		if (index%info.X != info.X - 1		      && index >= info.X)			   n2 = XMVector3Cross(v3, v2);
		if (index >= info.X					      && index%info.X != 0)			   n3 = XMVector3Cross(v4, v3);
		if (index < (info.X * info.Y) - info.X    && index%info.X != 0)			   n4 = XMVector3Cross(v1, v4);

		n1 = (XMVector3Normalize(n1) + XMVector3Normalize(n2) + XMVector3Normalize(n3) + XMVector3Normalize(n4)) / 4;

		XMFLOAT4 resultat;
		XMStoreFloat4(&resultat, n1);
		return resultat;
	}
};