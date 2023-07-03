#include "TitleScene.hpp"
#include "../SceneManager.hpp"
#include "../../Input/InputManager.hpp"

#include "Utility/Constants.hpp"

TitleScene::TitleScene()
{
	sf::Text newMenuItem;
	// Initialize m_text with your font, position, etc.
	m_font.loadFromFile("Content/Fonts/PixelAr.ttf");
	newMenuItem.setFont(m_font);
	newMenuItem.setCharacterSize(24);
	newMenuItem.setFillColor(sf::Color::White);

	newMenuItem.setString("Please Connect a Controller");
	newMenuItem.setOrigin(newMenuItem.getLocalBounds().width / 2, newMenuItem.getLocalBounds().height / 2);
	newMenuItem.setPosition(g_windowSize.x / 2, g_windowSize.y / 2);
	m_texts.push_back(newMenuItem);

	newMenuItem.setString("Press Start");
	newMenuItem.setOrigin(newMenuItem.getLocalBounds().width / 2, newMenuItem.getLocalBounds().height / 2);
	newMenuItem.setPosition(g_windowSize.x / 2, (g_windowSize.y / 2) + (newMenuItem.getLocalBounds().height + 42));
	m_texts.push_back(newMenuItem);
}

void TitleScene::update(float t_deltaTime)
{

}

void TitleScene::draw(sf::RenderWindow& t_target)
{
	for (auto& text : m_texts)
	{
		t_target.draw(text);
	}
}

void TitleScene::handleInput(const sf::Event& t_event)
{
	if (InputManager::isButtonPressed(0, 7))
	{
		SceneManager::getInstance().switchTo("MainMenu");
	}
}
