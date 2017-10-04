#pragma once
#include <vector>

struct TerrainFileHeader {
	int X{}, Y{};
	float Dx{}, Dy{};
};

struct HeightMapFile {
	TerrainFileHeader header{};
	std::vector<float> data{};
};

std::istream& operator>>(std::istream& is, TerrainFileHeader& t);