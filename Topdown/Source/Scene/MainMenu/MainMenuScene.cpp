#include "MainMenuScene.hpp"

#include <iostream>

#include "Scene/SceneManager.hpp"
#include "Input/InputManager.hpp"

#include "Scene/Gameplay/GameplayScene.hpp"

#include "Utility/Constants.hpp"

MainMenuScene::MainMenuScene() : m_selectedItemIndex(0)
{
	sf::Text newMenuItem;
	// Initialize m_text with your font, position, etc.
	m_font.loadFromFile("Content/Fonts/PixelAr.ttf");
	newMenuItem.setString("New Game");
	newMenuItem.setFont(m_font);
	newMenuItem.setCharacterSize(24);
	newMenuItem.setFillColor(sf::Color::White);
	newMenuItem.setOrigin(newMenuItem.getLocalBounds().width / 2, newMenuItem.getLocalBounds().height / 2);

	newMenuItem.setString("New Game");
	newMenuItem.setOrigin(newMenuItem.getLocalBounds().width / 2, newMenuItem.getLocalBounds().height / 2);
	newMenuItem.setPosition(g_windowSize.x / 2, g_windowSize.y / 2);
	m_menuItems.push_back(newMenuItem);

	newMenuItem.setString("Load Game");
	newMenuItem.setOrigin(newMenuItem.getLocalBounds().width / 2, newMenuItem.getLocalBounds().height / 2);
	newMenuItem.setPosition(g_windowSize.x / 2, (g_windowSize.y / 2) + (newMenuItem.getLocalBounds().height + 24));
	m_menuItems.push_back(newMenuItem);

	newMenuItem.setString("Exit");
	newMenuItem.setOrigin(newMenuItem.getLocalBounds().width / 2, newMenuItem.getLocalBounds().height / 2);
	newMenuItem.setPosition(g_windowSize.x / 2, (g_windowSize.y / 2) + (newMenuItem.getLocalBounds().height + 24) * 2);
	m_menuItems.push_back(newMenuItem);

	// Set the selected item to be highlighted
	m_menuItems[m_selectedItemIndex].setFillColor(sf::Color::Yellow);
}

void MainMenuScene::update(float t_deltaTime)
{
}

void MainMenuScene::draw(sf::RenderWindow& t_target)
{
	for (const auto& menuItem : m_menuItems)
	{
		t_target.draw(menuItem);
	}
}

void MainMenuScene::handleInput(const sf::Event& t_event)
{
	const EInputDirection inputDirection = InputManager::getSingleVerticalInput(0);
	if (inputDirection == EInputDirection::UP)
	{
		selectPreviousItem();
	}
	else if (inputDirection == EInputDirection::DOWN)
	{
		selectNextItem();
	}

	if (InputManager::isButtonPressed(0, 0))
	{
		activateSelectedItem();
	}
}

void MainMenuScene::selectNextItem()
{
	// Unhighlight the current item
	m_menuItems[m_selectedItemIndex].setFillColor(sf::Color::White);

	// Move to the next item, wrapping around if necessary
	m_selectedItemIndex = (m_selectedItemIndex + 1) % m_menuItems.size();

	// Highlight the new item
	m_menuItems[m_selectedItemIndex].setFillColor(sf::Color::Yellow);
}

void MainMenuScene::selectPreviousItem()
{
	// Unhighlight the current item
	m_menuItems[m_selectedItemIndex].setFillColor(sf::Color::White);

	// Move to the previous item, wrapping around if necessary
	m_selectedItemIndex = static_cast<int>((m_selectedItemIndex - 1 + m_menuItems.size()) % m_menuItems.size());

	// Highlight the new item
	m_menuItems[m_selectedItemIndex].setFillColor(sf::Color::Yellow);
}

void MainMenuScene::activateSelectedItem() const
{
	// Perform the action associated with the selected item
	// For example, if "New Game" is selected, switch to the GameplayScene
	if (m_selectedItemIndex == 0)
	{
		std::cout << "New Game" << std::endl;
		SceneManager::getInstance().addScene("Gameplay", std::make_unique<GameplayScene>(false));
		SceneManager::getInstance().switchTo("Gameplay");
	}
	else if (m_selectedItemIndex == 1)
	{
		std::cout << "Load Game" << std::endl;
		SceneManager::getInstance().addScene("Gameplay", std::make_unique<GameplayScene>(true));
		SceneManager::getInstance().switchTo("Gameplay");
	}
	else if (m_selectedItemIndex == 2)
	{
		std::cout << "Exit" << std::endl;
		std::exit(0);
	}
}
