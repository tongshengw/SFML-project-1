#ifndef CARD_H
#define CARD_H

#include <string>
#include <SFML/Graphics.hpp>

class Card {

    public:
        Card(const std::string &name, int cost, float xpos, float ypos);

        void render(sf::RenderWindow &window) const;

        int get_cost() const;
        std::string get_name() const;

    private:

        std::string name;
        int cost;
        sf::RectangleShape backgroundShape;
        int xpos;
        int ypos;

};

#endif