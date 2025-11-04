#pragma once
#include <sstream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
using namespace std;

class FrontEnd {
public:
    unsigned int width = 1600;
    unsigned int height = 900;
    string station;
};
void LoadFont(sf::Font& font, const string& fontFile);
string getStation(string station);
