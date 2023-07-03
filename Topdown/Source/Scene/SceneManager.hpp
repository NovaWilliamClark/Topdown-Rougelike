#pragma once
#include <memory>
#include <unordered_map>
#include "Scene.hpp"

class SceneManager {
public:
	static SceneManager& getInstance()
	{
		static SceneManager instance;
		return instance;
	}

	void addScene(const std::string& t_name, std::unique_ptr<Scene> t_scene);
	void switchTo(const std::string& t_name);

	void update(float t_deltaTime) const;
	void draw(sf::RenderWindow& t_target) const;
	void handleInput(const sf::Event& t_event) const;

	[[nodiscard]] int getLevelReached() const;
	void addLevelReached();
	void resetLevelReached();

private:
	SceneManager() = default;
	~SceneManager() = default;

	std::unordered_map<std::string, std::unique_ptr<Scene>> m_scenes;
	Scene* m_currentScene = nullptr;

	// I probably shouldn't do this here, but it's a quick and its preserved over scenes
	int m_levelReached = 0;
};
