#pragma once

#include "Generator.hpp"

class Dungeon : public Generator
{
protected:
	void removeCorridors() const;
};

class ClassicDungeon final : public Dungeon
{
private:
	void onGenerate() override;
};
