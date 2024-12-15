#include <string>
#include "Game.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <math.h>
#include <time.h>

const int framerate = 60;

Game::Game(const std::string& config) {
    init(config);
};

void Game::run() {
    while (m_running) {
        m_entities.update();
        std::cout << "bullet count : " << m_entities.getEntities("bullet").size() << "\n";
        m_currentFrame++;
        if (!m_paused) {
            sLifeSpan();
            sCollision();
            sSpawner();
            sMovement();
        }
        sUserInput();
        sRender();
    }
}

void Game::init(const std::string& config) {    
    m_window.create(sf::VideoMode(800, 600), "My window");
    m_window.setFramerateLimit(framerate);
    srand(time(NULL));
    spawnPlayer();
};

void Game::setPaused(bool paused) {
    m_paused = paused;
};


void Game::sMovement() {
    m_player->cTranfromPtr->velocity = { 0,0 };
    if (m_player->cInputPtr->up) {
        m_player->cTranfromPtr->velocity.y = -5;
    }
    if (m_player->cInputPtr->down) {
        m_player->cTranfromPtr->velocity.y = +5;
    }
    if (m_player->cInputPtr->left) {
        m_player->cTranfromPtr->velocity.x = -5;
    }
    if (m_player->cInputPtr->right) {
        m_player->cTranfromPtr->velocity.x = +5;
    }

    for (auto &i : m_entities.getEntities()) {
        if (i->cTranfromPtr != nullptr) {
            i->cTranfromPtr->pos += i->cTranfromPtr->velocity;
            i->cTranfromPtr->angle += 2.0;
        }
    }
};								//Systems: entity movement


void Game::sUserInput(){
    sf::Event event;
    while (m_window.pollEvent(event))
    {
        // "close requested" event: we close the window
        if (event.type == sf::Event::Closed) {
            m_running = 0;
            m_window.close();
        }

        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Escape) {
                m_running = 0;
            }
            
            else if (event.key.code == sf::Keyboard::P) {
                setPaused(!m_paused);
            }
            else if (event.key.code == sf::Keyboard::W) {
                m_player->cInputPtr->up = 1;
            }
            else if (event.key.code == sf::Keyboard::A) {
                m_player->cInputPtr->left = 1;
            }
            else if (event.key.code == sf::Keyboard::S) {
                m_player->cInputPtr->down = 1;
            }
            else if (event.key.code == sf::Keyboard::D) {
                m_player->cInputPtr->right = 1;
            }
        }
        else if (event.type == sf::Event::KeyReleased) {

            if (event.key.code == sf::Keyboard::W) {
                m_player->cInputPtr->up = 0;
            }
            else if (event.key.code == sf::Keyboard::A) {
                m_player->cInputPtr->left = 0;
            }
            else if (event.key.code == sf::Keyboard::S) {
                m_player->cInputPtr->down = 0;
            }
            else if (event.key.code == sf::Keyboard::D) {
                m_player->cInputPtr->right = 0;
            }
        }

        if (event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                if (!m_player->cInputPtr->leftMouse) {
                    m_player->cInputPtr->leftMouse = 1;
                }
            }
            if (event.mouseButton.button == sf::Mouse::Right) {
                if (!m_player->cInputPtr->rightMouse) {
                    m_player->cInputPtr->rightMouse = 1;
                }
            }
        }
        else if( event.type == sf::Event::MouseButtonReleased)
        {
            if (event.mouseButton.button == sf::Mouse::Left)
            {
                m_player->cInputPtr->leftMouse = 0;
            }
            break;
        }

    }
};								//Systems: User Input


void Game::sLifeSpan(){
    for (auto i : m_entities.getEntities()) {
        if (i->cLifeSpanPtr != nullptr) {
            i->cLifeSpanPtr->reamining--;
            int r, g, b;
            r = i->cShapePtr->circle.getFillColor().r;
            g = i->cShapePtr->circle.getFillColor().g;
            b = i->cShapePtr->circle.getFillColor().b;
            int opacity = (255 * (i->cLifeSpanPtr->reamining)) / (i->cLifeSpanPtr->total);

            i->cShapePtr->circle.setFillColor(sf::Color(r, g, b, opacity));
            i->cShapePtr->circle.setOutlineColor(sf::Color(r, g, b, opacity));
            if (i->cLifeSpanPtr->reamining < 0) {
                i->destroy();
            }
        }
    }
};								//Systems: lifespan of the entities


void Game::sRender(){
    entityVec entities =  m_entities.getEntities();
    //std::cout << entities.size() << "\n";
    m_window.clear();
    for (auto i : entities) {
        if (i->cTranfromPtr != nullptr && i->cShapePtr != nullptr) {
            i->cShapePtr->circle.setPosition(i->cTranfromPtr->pos.x , i->cTranfromPtr->pos.y);
            i->cShapePtr->circle.setRotation(i->cTranfromPtr->angle);
        }
    }
    for (auto i : entities) {
        if (i->cShapePtr != nullptr && i->isActive()) {
            m_window.draw(i->cShapePtr->circle);
        }
    }
    m_window.display();
};									//Systems: Render


void Game::sSpawner(){
    if (m_currentFrame - m_lastEnemySpawnTime >= framerate) {
        m_lastEnemySpawnTime = m_currentFrame;
        spawnEnemy();
    }

    if (m_player->cInputPtr->leftMouse) {
        m_player->cInputPtr->leftMouse = 0;
        Vec2 mousePos( sf::Mouse::getPosition(m_window).x, sf::Mouse::getPosition(m_window).y );
        spawnBullet(m_player,mousePos);
    }

    if (m_player->cInputPtr->rightMouse) {
        m_player->cInputPtr->rightMouse = 0;
        Vec2 mousePos(sf::Mouse::getPosition(m_window).x, sf::Mouse::getPosition(m_window).y);
        spawnSpecialWeapon(m_player, mousePos);
    }

    for (auto i : m_entities.getEntities("enemy")) {
        if (!i->isActive()) {
            spawnSmallEnemies(i);
        }
    }
};							//Systems: Spawns enemies





void Game::sCollision(){

    for (auto& i : m_entities.getEntities("bullet")) {
        auto entity = i;
        float mxr = entity->cTranfromPtr->pos.x + entity->cCollisionPtr->collisionRadius;
        float mxl = entity->cTranfromPtr->pos.x - entity->cCollisionPtr->collisionRadius;

        float myu = entity->cTranfromPtr->pos.y - entity->cCollisionPtr->collisionRadius;
        float myd = entity->cTranfromPtr->pos.y + entity->cCollisionPtr->collisionRadius;

        if (mxl <= 0 || mxr >= m_window.getSize().x || myu <= 0 || myd >= m_window.getSize().y) {
            i->destroy();
        }
    }

    for (auto &i : m_entities.getEntities("enemy")) {
        auto entity = i;
        float mxr = entity->cTranfromPtr->pos.x + entity->cCollisionPtr->collisionRadius;
        float mxl = entity->cTranfromPtr->pos.x - entity->cCollisionPtr->collisionRadius;

        float myu = entity->cTranfromPtr->pos.y - entity->cCollisionPtr->collisionRadius;
        float myd = entity->cTranfromPtr->pos.y + entity->cCollisionPtr->collisionRadius;

        if (mxl <= 0 || mxr >= m_window.getSize().x) {
            entity->cTranfromPtr->velocity.x *= -1;
        }
        if (myu <= 0 || myd >= m_window.getSize().y) {
            entity->cTranfromPtr->velocity.y *= -1;   
        }
    }


    {   //player collision with walls
        auto entity = m_player;
        float mxr = entity->cTranfromPtr->pos.x + entity->cCollisionPtr->collisionRadius;
        float mxl = entity->cTranfromPtr->pos.x - entity->cCollisionPtr->collisionRadius;

        float myu = entity->cTranfromPtr->pos.y - entity->cCollisionPtr->collisionRadius;
        float myd = entity->cTranfromPtr->pos.y + entity->cCollisionPtr->collisionRadius;
        if (mxl <= 0) {
            entity->cTranfromPtr->pos.x = entity->cCollisionPtr->collisionRadius;
        }
        if (mxr >= m_window.getSize().x) {
            entity->cTranfromPtr->pos.x = m_window.getSize().x - entity->cCollisionPtr->collisionRadius;
        }
        if (myu <= 0) {
            entity->cTranfromPtr->pos.y = entity->cCollisionPtr->collisionRadius;
        }
        if (myd >= m_window.getSize().y) {
            entity->cTranfromPtr->pos.y = m_window.getSize().y - entity->cCollisionPtr->collisionRadius;
        }
    }

    for (auto &i : m_entities.getEntities("enemy")) {
        for (auto &j : m_entities.getEntities("bullet")) {
            float enemRad = i->cCollisionPtr->collisionRadius;
            Vec2 enemPos = i->cTranfromPtr->pos;

            float bullRad = j->cCollisionPtr->collisionRadius;
            Vec2 bullPos = j->cTranfromPtr->pos;
            Vec2 disp = (enemPos - bullPos);
            if (((disp.x) * (disp.x) + (disp.y) * (disp.y)) <= (enemRad + bullRad) * (enemRad + bullRad)) {
                i->destroy();
                j->destroy();
            }
        }
    }

    for (auto& i : m_entities.getEntities("small enemy")) {
        for (auto& j : m_entities.getEntities("bullet")) {
            float enemRad = i->cCollisionPtr->collisionRadius;
            Vec2 enemPos = i->cTranfromPtr->pos;

            float bullRad = j->cCollisionPtr->collisionRadius;
            Vec2 bullPos = j->cTranfromPtr->pos;
            Vec2 disp = (enemPos - bullPos);
            if (((disp.x) * (disp.x) + (disp.y) * (disp.y)) <= (enemRad + bullRad) * (enemRad + bullRad)) {
                i->destroy();
                j->destroy();
            }
        }
    }

    for (auto& i : m_entities.getEntities("small enemy")) {
        float enemRad = i->cCollisionPtr->collisionRadius;
        Vec2 enemPos = i->cTranfromPtr->pos;

        float playRad = m_player->cCollisionPtr->collisionRadius;
        Vec2 playPos = m_player->cTranfromPtr->pos;
        Vec2 disp = (enemPos - playPos);
        if (((disp.x) * (disp.x) + (disp.y) * (disp.y)) <= (enemRad + playRad) * (enemRad + playRad)) {
            i->destroy();
            m_player->destroy();
            spawnPlayer();
        }
    }

    for (auto& i : m_entities.getEntities("enemy")) {
        float enemRad = i->cCollisionPtr->collisionRadius;
        Vec2 enemPos = i->cTranfromPtr->pos;

        float playRad = m_player->cCollisionPtr->collisionRadius;
        Vec2 playPos = m_player->cTranfromPtr->pos;
        Vec2 disp = (enemPos - playPos);
        if (((disp.x) * (disp.x) + (disp.y) * (disp.y)) <= (enemRad + playRad) * (enemRad + playRad)) {
            i->destroy();
            m_player->destroy();
            spawnPlayer();
        }
    }


    for (auto& i : m_entities.getEntities("enemy")) {
        for (auto& j : m_entities.getEntities("special")) {
            float enemRad = i->cCollisionPtr->collisionRadius;
            Vec2 enemPos = i->cTranfromPtr->pos;

            float bullRad = j->cCollisionPtr->collisionRadius;
            Vec2 bullPos = j->cTranfromPtr->pos;
            Vec2 disp = (enemPos - bullPos);
            if (((disp.x) * (disp.x) + (disp.y) * (disp.y)) <= (enemRad + bullRad) * (enemRad + bullRad)) {
                i->destroy();
            }
        }
    }
    for (auto& i : m_entities.getEntities("small enemy")) {
        for (auto& j : m_entities.getEntities("special")) {
            float enemRad = i->cCollisionPtr->collisionRadius;
            Vec2 enemPos = i->cTranfromPtr->pos;

            float bullRad = j->cCollisionPtr->collisionRadius;
            Vec2 bullPos = j->cTranfromPtr->pos;
            Vec2 disp = (enemPos - bullPos);
            if (((disp.x) * (disp.x) + (disp.y) * (disp.y)) <= (enemRad + bullRad) * (enemRad + bullRad)) {
                i->destroy();
            }
        }
    }

    for (auto& i : m_entities.getEntities("special")) {
        auto entity = i;
        float mxr = entity->cTranfromPtr->pos.x + entity->cCollisionPtr->collisionRadius;
        float mxl = entity->cTranfromPtr->pos.x - entity->cCollisionPtr->collisionRadius;

        float myu = entity->cTranfromPtr->pos.y - entity->cCollisionPtr->collisionRadius;
        float myd = entity->cTranfromPtr->pos.y + entity->cCollisionPtr->collisionRadius;

        if (mxl <= 0 || mxr >= m_window.getSize().x) {
            entity->cTranfromPtr->velocity.x *= -1;
        }
        if (myu <= 0 || myd >= m_window.getSize().y) {
            entity->cTranfromPtr->velocity.y *= -1;
        }
    }
};								//Systems: Collision detection

void Game::spawnPlayer() {
    auto e = m_entities.addEntity("player");
    m_player = e;

    float mx = m_window.getSize().x/2.0;
    float my = m_window.getSize().y/2.0;

    e->cInputPtr = std::make_shared<cInput>();
    e->cTranfromPtr = std::make_shared<cTransform>(Vec2(mx , my) , Vec2(0,0) , 0);
    e->cShapePtr = std::make_shared<cShape>(10, 50, sf::Color(255, 0, 0), sf::Color(0, 255, 0), 2);
    e->cCollisionPtr = std::make_shared<cCollision>(50);
};

void Game::spawnEnemy() {
    auto e = m_entities.addEntity("enemy");
    float rad = (std::rand()) % 15 + 15;
    int sides = (std::rand()) % 10 + 3;
    float mx = (std::rand()) % (m_window.getSize().x - 2* (int)rad) + rad;
    float my = (std::rand()) % (m_window.getSize().y - 2* (int)rad) + rad;
    float dir = std::rand() % 360;
    Vec2 vel(cosf(dir) , sinf(dir));
    vel *= (rand() % 5) + 1;
    int r, g, b;
    r = std::rand() % 255;
    g = std::rand() % 255;
    b = std::rand() % 255;

    e->cTranfromPtr = std::make_shared<cTransform>(Vec2(mx, my), vel, 0);
    e->cShapePtr = std::make_shared<cShape>(sides, rad, sf::Color(r,g,b), sf::Color(255, 0, 0), 2);
    e->cCollisionPtr = std::make_shared<cCollision>(rad);
};


void Game::spawnSmallEnemies(std::shared_ptr<Entity> entity) {
    float rad = entity->cShapePtr->circle.getRadius()/2;
    float sides = entity->cShapePtr->circle.getPointCount();
    sf::Color col = entity->cShapePtr->circle.getFillColor();
    sf::Color oCol = entity->cShapePtr->circle.getOutlineColor();
    float angle = 0;
    for (int i = 0; i < entity->cShapePtr->circle.getPointCount(); i++) {
        auto e = m_entities.addEntity("small enemy");

        Vec2 vel(cosf(angle), sinf(angle));
        vel *= entity->cTranfromPtr->velocity.length() / 2;
        angle += 2.0 * (22.0 / 7.0) / sides;;
        
        e->cTranfromPtr = std::make_shared<cTransform>(entity->cTranfromPtr->pos, vel, 0);
        e->cShapePtr = std::make_shared<cShape>(sides, rad, col, oCol, 1);
        e->cCollisionPtr = std::make_shared<cCollision>(rad);
        e->cLifeSpanPtr = std::make_shared<cLifeSpan>(60);
    }
};

void Game::spawnBullet(std::shared_ptr<Entity> entity, const Vec2& mousePos) {
    //std::cout << "bullet spawn";
    auto e = m_entities.addEntity("bullet");

    Vec2 dir = mousePos - entity->cTranfromPtr->pos;
    dir.normalize();
    Vec2 vel = dir * 15;

    e->cTranfromPtr = std::make_shared<cTransform>(entity->cTranfromPtr->pos, vel, 0);
    e->cLifeSpanPtr = std::make_shared<cLifeSpan>(60);
    e->cShapePtr = std::make_shared<cShape>(30, 20, sf::Color(255, 255, 0), sf::Color(255, 0, 255), 2);
    e->cCollisionPtr = std::make_shared<cCollision>(20);
};

void Game::spawnSpecialWeapon(std::shared_ptr<Entity> entity, const Vec2& mousePos) {
    auto e = m_entities.addEntity("special");
    
    Vec2 dir = mousePos - entity->cTranfromPtr->pos;
    dir.normalize();
    Vec2 vel = dir * 15;

    e->cTranfromPtr = std::make_shared<cTransform>(entity->cTranfromPtr->pos, vel, 0);
    e->cLifeSpanPtr = std::make_shared<cLifeSpan>(300);
    e->cShapePtr = std::make_shared<cShape>(15, 20, sf::Color(255,0, 255), sf::Color(255, 255, 255), 2);
    e->cCollisionPtr = std::make_shared<cCollision>(20);
};

