#include "SceneManager.hpp"

#include "imgui-SFML.h"

void SceneManager::addScene(const std::string& t_name, std::unique_ptr<Scene> t_scene)
{
	m_scenes[t_name] = std::move(t_scene);
}

void SceneManager::switchTo(const std::string& t_name)
{
	const auto it = m_scenes.find(t_name);
	if (it != m_scenes.end())
	{
		m_currentScene = it->second.get();
	}
}

void SceneManager::update(const float t_deltaTime) const
{
	if (m_currentScene)
	{
		m_currentScene->update(t_deltaTime);
	}
}

void SceneManager::draw(sf::RenderWindow& t_target) const
{
	if (m_currentScene)
	{
		m_currentScene->draw(t_target);
	}
}

void SceneManager::handleInput(const sf::Event& t_event) const
{
	if (m_currentScene)
	{
		m_currentScene->handleInput(t_event);
	}
}

int SceneManager::getLevelReached() const
{
	return m_levelReached;
}

void SceneManager::addLevelReached()
{
	m_levelReached++;
}

void SceneManager::resetLevelReached()
{
	m_levelReached = 0;
}
