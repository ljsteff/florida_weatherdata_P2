#include "front_end.h"

void LoadFont(sf::Font& font, const string& fontFile) {
    if (!font.openFromFile(fontFile)) {
        cerr << "failure loading fonts" << endl;
    }
}

string getStation(string station) {
    return station;
}
int getYear(int year) {
    return year;
}
