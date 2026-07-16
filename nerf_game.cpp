#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <array>
#include <string>
#include <random>

int main() {
    sf::RenderWindow window(sf::VideoMode(1100, 1000), "Nerf Game");
    window.setKeyRepeatEnabled(false);

    float clocktime = 0;

    sf::CircleShape player(25.f, 3);

    int px = 550;
    int py = 500;
    int pangle = 0;
    int pspeed = 3;
    int pbulletsloaded = 5;
    float lastreloaded = 0;
    float lastshot;
    bool hasnotshotonce = true;
    bool hasnotreloadedonce = true;
    player.setFillColor(sf::Color::Blue);
    player.setPosition(px, py);
    player.setOrigin(25.f, 25.f);

    sf::CircleShape pdir(5.f, 3);
    pdir.setFillColor(sf::Color(255, 117, 0));
    pdir.setOrigin(5.f, 5.f);

    std::vector<sf::RectangleShape> bullets;

    std::array<std::array<int, 2>, 10> starting_enemy_positons = {{{-100, -100}, {110, 25}, {220, 25}, {330, 25}, {440, 25}, {550, 25}, {660, 25}, {770, 25}, {880, 25}, {990, 25}}};
    std::vector<sf::RectangleShape> enemies;
    std::vector<int> enemieshp = {20, 20, 20, 20, 20, 20, 20, 20, 30, 40};
    int espeed = pspeed-1;

    sf::Font mainfont;
    mainfont.loadFromFile("Crillee_Italic_Std_Regular.otf");
    sf::Text cooldowntext;
    cooldowntext.setFont(mainfont);
    cooldowntext.setFillColor(sf::Color(255, 0, 0));
    cooldowntext.setString("COOLDOWN");

    for (std::array<int, 2> pos : starting_enemy_positons) {
        sf::RectangleShape newEnemy(sf::Vector2f(30, 30));
        newEnemy.setOrigin(20, 20);
        newEnemy.setPosition(pos[0], pos[1]);
        newEnemy.setFillColor(sf::Color::Red);
        enemies.push_back(newEnemy);
    }
    window.setFramerateLimit(60);
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Space) {
                    if (clocktime-lastshot > 0.5 || hasnotshotonce) {
                        if (pbulletsloaded > 0) {
                            sf::RectangleShape newBullet(sf::Vector2f(2, 10));
                            newBullet.setFillColor(sf::Color::Blue);
                            if (pangle == 0) {
                                newBullet.setPosition(px, py-5-(player.getLocalBounds().height / 2.f));
                            } else if (pangle == 90) {
                                newBullet.setPosition(px+5+(player.getLocalBounds().width / 2.f), py);
                            } else if (pangle == 180) {
                                newBullet.setPosition(px, py+5+(player.getLocalBounds().height / 2.f));
                            } else if (pangle == 270) {
                                newBullet.setPosition(px-5-(player.getLocalBounds().width / 2.f), py);
                            }
                            
                            newBullet.setRotation(pangle);
                            bullets.push_back(newBullet);
                            pbulletsloaded--;
                        }
                        lastshot = clocktime;
                        hasnotshotonce = false;
                    }
                }
                if (event.key.code == sf::Keyboard::Enter) {
                    if (clocktime - lastreloaded > 5 || hasnotreloadedonce) {
                        pbulletsloaded = 5;
                        lastreloaded = clocktime;
                        hasnotreloadedonce = false;
                    }
                }
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            py -= pspeed;
            pangle = 0;
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            py += pspeed;
            pangle = 180;
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            px += pspeed;
            pangle = 90;
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            px -= pspeed;
            pangle = 270;
        }

        window.clear(sf::Color(20, 20, 20));
        player.setPosition(px, py);
        player.setRotation(pangle);

        if (pangle == 0) {
            pdir.setPosition(px, py - (player.getLocalBounds().height / 2.f) - (pdir.getLocalBounds().height / 2.f));
        } else if (pangle == 180) {
            pdir.setPosition(px, py + (player.getLocalBounds().height / 2.f) + (pdir.getLocalBounds().height / 2.f));
        } else if (pangle == 90) {
            pdir.setPosition(px + (player.getLocalBounds().width / 2.f), py);
        } else if (pangle == 270) {
            pdir.setPosition(px - (player.getLocalBounds().width / 2.f), py);
        }
        pdir.setRotation(pangle);

        // Screen boundaries
        if (px < 0) {
            px += pspeed;
        }
        if (px > 1100) {
            px -= pspeed;
        }
        if (py < 0) {
            py += pspeed;
        }
        if (py > 1000) {
            py -= pspeed;
        }

        
        window.draw(player);
        window.draw(pdir);

        int bi = 0;
        for (sf::RectangleShape &b : bullets) {
            float bx = b.getPosition().x;
            float by = b.getPosition().y;
            int bangle = b.getRotation();
            if (bangle == 0) {
                b.move(0, -5);
            } else if (bangle == 90) {
                b.move (5, 0);
            } else if (bangle == 180) {
                b.move(0, 5);
            } else if (bangle == 270) {
                b.move(-5, 0);
            }
            if (bx < 0 || bx > 1100 || by < 0 || by > 1000) {
                bullets.erase(bullets.begin() + bi);
            } else {
                bi++;
            }
            window.draw(b);
        }
        if (enemies.size() > 0) {
            for (int ei = static_cast<int>(enemies.size()) - 1; ei >= 0; --ei) {
                bool erased = false;
                for (int bi = static_cast<int>(bullets.size()) - 1; bi >= 0; --bi) {
                    if (ei < static_cast<int>(enemies.size()) && enemies[ei].getGlobalBounds().intersects(bullets[bi].getGlobalBounds())) {
                        bullets.erase(bullets.begin() + bi);
                        enemieshp[ei] -= 10;
                        if (enemieshp[ei] <= 0) {
                            enemies.erase(enemies.begin() + ei);
                            enemieshp.erase(enemieshp.begin() + ei);
                            // If killing the second-last real enemy leaves only the off-screen placeholder,
                            // also remove that placeholder so the win condition triggers.
                            if (enemies.size() == 1) {
                                sf::Vector2f pos = enemies[0].getPosition();
                                if (pos.x < 0 || pos.y < 0) {
                                    enemies.erase(enemies.begin());
                                    enemieshp.erase(enemieshp.begin());
                                }
                            }
                            erased = true;
                        }
                        break;
                    }
                }
                if (!erased && ei < static_cast<int>(enemies.size()) && ei > 0) {
                    // AI
                    // Get the actual enemy for less repeating
                    sf::RectangleShape &e = enemies[ei];
                    int xtomove = player.getPosition().x - e.getPosition().x;
                    int ytomove = player.getPosition().y - e.getPosition().y;

                    std::random_device rd;

                    std::mt19937 gen(rd());

                    std::uniform_int_distribution<int> distrib(-2, 2);
                    if (xtomove != 0) {
                        if (xtomove > 0) e.move(espeed+distrib(gen), 0);
                        else e.move((espeed)*-1+distrib(gen), 0);
                    }
                    if (ytomove != 0) { 
                        if (ytomove > 0) e.move(0, espeed+distrib(gen));
                        else e.move(0, (espeed) *-1 + distrib(gen));
                    }
                    sf::Text hptext;
                    hptext.setFont(mainfont);
                    hptext.setFillColor(sf::Color(255, 117, 0));
                    hptext.setCharacterSize(10);
                    hptext.setPosition(e.getPosition().x - 15, (e.getPosition().y - e.getLocalBounds().height / 2.f) - 20);
                    hptext.setString(std::to_string(enemieshp[ei]));
                    // Check if player has been touched by enemy
                    if (e.getGlobalBounds().intersects(player.getGlobalBounds())) {
                        // Game over
                        std::cout << "You lost :(\n";
                        window.close();
                    }
                    window.draw(e);
                    window.draw(hptext);
                }
            }
        } else {
            std::cout << "You won :)\n";
            window.close();
        }
        if (clocktime-lastreloaded < 5 || clocktime-lastshot < 0.5) {
            window.draw(cooldowntext);
        }
        clocktime += 1.f/60;
        window.display();
    }

    return 0;
}
