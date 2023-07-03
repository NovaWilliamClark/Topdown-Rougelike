#include "Application.hpp"

#include <SFML/Window/Event.hpp>

#include <imgui.h>
#include <imgui-SFML.h>

#include "Input/InputManager.hpp"

#include "Scene/SceneManager.hpp"
#include "Scene/Gameover/GameoverScene.hpp"
#include "Scene/Gameplay/GameplayScene.hpp"
#include "Scene/MainMenu/MainMenuScene.hpp"
#include "Scene/Title/TitleScene.hpp"
#include "Scene/Gameover/GameoverScene.hpp"

#include "Utility/Constants.hpp"

Application::Application()
{
	m_window.create(sf::VideoMode(g_windowSize.x, g_windowSize.y), "Rougelike", sf::Style::Close);
	m_window.setFramerateLimit(60);
	m_window.setVerticalSyncEnabled(true);
	ImGui::SFML::Init(m_window);

	SceneManager::getInstance().addScene("Title", std::make_unique<TitleScene>());
	SceneManager::getInstance().addScene("MainMenu", std::make_unique<MainMenuScene>());
	SceneManager::getInstance().addScene("Gameover", std::make_unique<GameoverScene>());

	SceneManager::getInstance().switchTo("Title");
}

void Application::run()
{

	sf::Clock clock;
	// Open Menu ask for new game or load game
	while (m_window.isOpen())
	{
		const sf::Time dt = clock.restart();
		// Update ImGui-SFML
		processInput();
		update(dt);
		render();
	}
}

void Application::processInput()
{
	sf::Event event{};

	while (m_window.pollEvent(event))
	{
		ImGui::SFML::ProcessEvent(event);

		if (event.type == sf::Event::Closed)
		{
			m_window.close();
		}

		InputManager::update();
		SceneManager::getInstance().handleInput(event);
	}
}

void Application::update(const sf::Time t_dt)
{
	ImGui::SFML::Update(m_window, t_dt);
	SceneManager::getInstance().update(t_dt.asSeconds());
}

void Application::render()
{
	m_window.clear();
	SceneManager::getInstance().draw(m_window);
	ImGui::SFML::Render(m_window);
	m_window.display();
}
