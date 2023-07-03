#include "GameoverScene.hpp"
#include "../SceneManager.hpp"
#include "../../Input/InputManager.hpp"

#include "Utility/Constants.hpp"

GameoverScene::GameoverScene()
{
	sf::Text newItem;
	// Initialize m_text with your font, position, etc.
	m_font.loadFromFile("Content/Fonts/PixelAr.ttf");
	newItem.setFont(m_font);
	newItem.setCharacterSize(24);
	newItem.setFillColor(sf::Color::White);

	newItem.setString("GAME OVER");
	newItem.setOrigin(newItem.getLocalBounds().width / 2, newItem.getLocalBounds().height / 2);
	newItem.setPosition(g_windowSize.x / 2, g_windowSize.y / 2);
	m_texts.push_back(newItem);

	newItem.setString("MAP REACHED: " + std::to_string(SceneManager::getInstance().getLevelReached()));
	newItem.setOrigin(newItem.getLocalBounds().width / 2, newItem.getLocalBounds().height / 2);
	newItem.setPosition(g_windowSize.x / 2, (g_windowSize.y / 2) + (newItem.getLocalBounds().height + 42));
	m_texts.push_back(newItem);

	newItem.setString("PRESS START TO GO BACK TO MAIN MENU");
	newItem.setOrigin(newItem.getLocalBounds().width / 2, newItem.getLocalBounds().height / 2);
	newItem.setPosition(g_windowSize.x / 2, (g_windowSize.y / 2) + (newItem.getLocalBounds().height + 84));
	m_texts.push_back(newItem);
}

void GameoverScene::update(float t_deltaTime)
{
	m_texts.at(1).setString("MAP REACHED: " + std::to_string(SceneManager::getInstance().getLevelReached()));
}

void GameoverScene::draw(sf::RenderWindow& t_target)
{
	for (auto& text : m_texts)
	{
		t_target.draw(text);
	}
	sf::View view(sf::FloatRect(0.f, 0.f, g_windowSize.x ,g_windowSize.y));
	view.setCenter(g_windowSize.x / 2, g_windowSize.y / 2);
	t_target.setView(view);
}

void GameoverScene::handleInput(const sf::Event& t_event)
{
	if (InputManager::isButtonPressed(0, 7))
	{
		SceneManager::getInstance().resetLevelReached();
		SceneManager::getInstance().switchTo("MainMenu");
	}
}
