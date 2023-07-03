#pragma once

#include <SFML/Graphics.hpp>

class Entity {
public:
    Entity();
    virtual ~Entity() = default;

    void setPosition(const sf::Vector2f& t_position);
    sf::Vector2f getPosition() const;

    void setTexture(const sf::Texture& t_texture);
    void setSize(const sf::Vector2f& t_size);
    void setOrigin(const sf::Vector2f& t_origin);

    std::string getName();

    virtual void update(float t_deltaTime) = 0;
    virtual void draw(sf::RenderWindow& t_window) const;

protected:
    sf::Vector2f position;
    sf::Sprite sprite;
private:
    std::unique_ptr<sf::Texture> m_texture;
};
