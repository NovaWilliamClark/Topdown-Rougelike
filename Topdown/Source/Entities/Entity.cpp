#include "Entity.hpp"

#include "Utility/Constants.hpp"

Entity::Entity()
= default;

void Entity::setPosition(const sf::Vector2f& t_position)
{
    position = t_position;
    sprite.setPosition(t_position.x * g_tileSize.x, t_position.y * g_tileSize.y);
}

sf::Vector2f Entity::getPosition() const
{
    return position;
}

void Entity::setTexture(const sf::Texture& t_texture)
{
    m_texture = std::make_unique<sf::Texture>(t_texture);
    sprite.setTexture(*m_texture);
}

void Entity::setSize(const sf::Vector2f& t_size)
{
    sprite.setScale(t_size.x / sprite.getLocalBounds().width, t_size.y / sprite.getLocalBounds().height);
}

void Entity::setOrigin(const sf::Vector2f& t_origin)
{
    sprite.setOrigin(t_origin);
}

std::string Entity::getName()
{
    const auto memAddress = reinterpret_cast<size_t>(this);
    return "Entity: " + std::to_string(memAddress);
}

void Entity::update(float t_deltaTime)
{}

void Entity::draw(sf::RenderWindow& t_window) const
{
    t_window.draw(sprite);
}
