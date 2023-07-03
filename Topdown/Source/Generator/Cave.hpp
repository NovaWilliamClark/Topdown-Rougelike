#pragma once

#include "Generator.hpp"

class Cave : public Generator
{
public:
	Cave();

protected:
	void removeWallChunks(int t_minSize = 10);
};

class BigCave final : public Cave
{
private:
	void onGenerate() override;
};

class ConnectedCaves final : public Cave
{
private:
	void onGenerate() override;
};

class CavesAndLava final : public Cave
{
private:
	void onGenerate() override;
};

class CavesAndRooms final : public Cave
{
private:
	void onGenerate() override;
};
