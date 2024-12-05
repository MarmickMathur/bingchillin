#include <SFML/Graphics.hpp>
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
float chsz;
int chr, chg, chb;
sf::Font font;

class shape { 
public:
    float sx;
    float sy;
    std::shared_ptr<sf::Shape> shapeptr;
    sf::Text text;
    shape(std::string name, float sx, float sy, std::shared_ptr<sf::Shape> shapeptr ):
        sx(sx),
        sy(sy),
        shapeptr(shapeptr)
    {
        text.setFont(font);
        text.setString(name);
        text.setCharacterSize(chsz);
        text.setFillColor(sf::Color(chr, chg, chb));
    }
};

void posUpdate(std::vector<shape> &shapes) {
    for (auto& i: shapes) {
        sf::Vector2f pos = i.shapeptr->getPosition();
        i.shapeptr->setPosition(pos.x + i.sx, pos.y + i.sy);
        sf::Vector2f pos2 = i.shapeptr->getPosition();
        sf::FloatRect box = i.shapeptr->getGlobalBounds();
        sf::FloatRect textBox = i.text.getGlobalBounds();
        i.text.setPosition(pos2.x + box.width / 2 - textBox.width / 2, pos2.y + box.height / 2 - textBox.height / 2);
    }
}
  int width,height;

void collisionDetection(std::vector<shape>& shapes) {
    for (auto& i : shapes) {
        sf::Vector2f pos = i.shapeptr->getPosition();
        sf::FloatRect size = i.shapeptr->getGlobalBounds();
        if (pos.x <= 0 || pos.x + size.width >= width) {
            i.sx *= -1;
        }
        if (pos.y <= 0 || pos.y + size.height >= height) {
            i.sy *= -1;
        }
    }
}

int main()
{
    std::ifstream fin("shapes.txt");
    std::string temp;
    std::vector<shape> shapes;

    while (fin >> temp) {
        if (temp == "window") {
            fin >> height;
            fin >> width;
        }else if(temp == "Font") {
            std::string fontName;
            fin >> fontName;
            if (!font.loadFromFile(fontName))
            {
                std::cout << "ERROR loading font" << "\n";
                exit(1);
            }
            fin >> chsz >> chr >> chg >> chb;
        }else {
            std::cout << temp << "\n";
            sf::Vector2f pos;
            int r, g, b;
            float sx, sy;
            std::string name;
            fin >> name;
            std::cout << name << "\n";
            fin >> pos.x >> pos.y;
            std::cout << pos.x << " " << pos.y << "\n";
            fin >> sx >> sy;
            std::cout << sx << " " << sy << "\n";
            fin >> r >> g >> b;

            if (temp == "rectangle") {
                sf::Vector2f size;
                fin >> size.x >> size.y;
                //sf::RectangleShape* rect = new sf::RectangleShape;
                std::shared_ptr<sf::RectangleShape> rect = std::make_shared<sf::RectangleShape>();
                rect->setFillColor(sf::Color(r, g, b));
                rect->setPosition(pos);
                rect->setSize(size);
                shape newShape(name, sx, sy, rect);
                shapes.push_back(newShape);
            }
            else if (temp == "circle") {
                int rad;
                fin >> rad;
                //sf::CircleShape *circ = new sf::CircleShape;
                std::shared_ptr<sf::CircleShape> circ = std::make_shared<sf::CircleShape>();
                circ->setFillColor(sf::Color(r, g, b));
                circ->setPosition(pos);
                circ->setRadius(rad);
                shape newShape(name, sx, sy, circ);
                shapes.push_back(newShape);
            }
        }    
    }

    sf::RenderWindow window(sf::VideoMode(height, width), "My window");
    window.setFramerateLimit(60); // call it once, after creating the window


    // run the program as long as the window is open
    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
        }

        posUpdate(shapes);
        collisionDetection(shapes);

        window.clear(sf::Color::Black);
        for (auto& i : shapes) {
            window.draw(*i.shapeptr);
            window.draw(i.text);
        }
        window.display();

    }


    return 0;
}