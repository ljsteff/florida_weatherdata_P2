#include "front_end.h"
// #include <chrono>
#include <optional>
#include <SFML/Window/Event.hpp>
#include <set>
// make sure to place vsc within cmake-build-debug folder, rn it just reads GNV.csv
FrontEnd front;

// set of all the station names to compare to input
set<string> stations = {"AAF", "BCT", "BKV", "BOW", "F95", "FIN", "CEW", "XMR", "VSH", "NFJ","CLW",
"2IS", "54A", "CTY", "CGC", "DAB", "egi", "54J", "DED", "DTS","VPS", "EGC", "FXE", "FMY", "RSW",
"RSW", "FPR", "FHB", "FLL", "GNV", "HWO", "HST", "HRT", "IMM", "INF", "NIP", "CRG", "VQQ", "HEG", "NEN",
"JAX", "NQX", "ISM", "K70","EYW", "42J", "LAL", "LEE", "LCQ", "X07", "LNA", "24J", "MCF", "MAI", "NRB",
"MLB", "MIA", "TMB", "OPF", "AGR", "MTH", "MKY", "COI", "MBF", "NDZ", "APF", "TTS", "EVB", "ORL", "OMN",
"OCF", "OCR", "OBE","MCO", "PFN", "COF", "NPA", "PNS", "PMP", "PGD", "28J", "XFL", "ECP", "TDR", "FPY", "PCM",
"2J9", "PIE", "SFB", "SRQ", "SGJ", "SEF", "SPG", "TLH", "TPA", "TIX", "PAM", "TPF", "VVG", "X21", "1J0", "BCR",
"VDF", "X59", "X26", "VNC", "VRB", "PBI", "NSE", "GIF", "SUA", "F45", "X60", "ZPH"};

string stationInput;
int main() {
    // it took me 10 minutes to figure out why the size was giving an error (it wanted {} :(    )
    sf::RenderWindow window(sf::VideoMode({front.width, front.height}), "T.I.M.");
    window.setFramerateLimit(60);
    sf::Color background(sf::Color::White);

    // fonts
    sf::Font font;
    LoadFont(font, "Fonts/G_ari_bd.ttf");
    // clock text object, displayed as font above ^
    sf::Text clock(font);
    // sf::Clock clockTimer;

    // Title text
    sf::Text title(font);
    title.setString("Temperature Increase Mapping");
    title.setCharacterSize(90);
    title.setFillColor(sf::Color(0, 28, 127));
    title.setOutlineColor(sf::Color::Black);
    title.setOutlineThickness(2);
    auto titleSize = title.getLocalBounds().size;
    title.setOrigin({titleSize.x / 2.f, titleSize.y / 2.f});
    title.setPosition(sf::Vector2f(front.width / 2.f, 50.f));

    // Input station text
    sf::Text station(font);
    station.setString("Input Station: ");
    station.setCharacterSize(50);
    station.setFillColor(sf::Color(0, 88, 187));
    station.setOutlineColor(sf::Color::Black);
    station.setOutlineThickness(2);
    auto stationSize = station.getLocalBounds().size;
    station.setOrigin({stationSize.x, stationSize.y});
    station.setPosition(sf::Vector2f(front.width / 2.5f, front.height / 3.f));

    // Textbox
    sf::RectangleShape textBox(sf::Vector2f(150, 50));
    textBox.setFillColor(sf::Color::Transparent);
    textBox.setOutlineColor(sf::Color::Black);
    textBox.setOutlineThickness(2);
    auto textBoxSize = textBox.getLocalBounds().size;
    textBox.setOrigin({textBoxSize.x, textBoxSize.y});
    textBox.setPosition(sf::Vector2f(front.width / 2.f, front.height / 2.85f));

    // Text that'll be inside textbox
    sf::Text stationText(font);
    stationText.setFont(font);
    stationText.setCharacterSize(45);
    stationText.setFillColor(sf::Color::Black);
    auto stationTextSize = stationText.getLocalBounds().size;
    stationText.setOrigin({stationTextSize.x, stationTextSize.y});
    stationText.setPosition(sf::Vector2f(front.width / 2.4f, front.height / 3.5f));

    // Station not found error text
    sf::Font secondary;
    LoadFont(secondary, "Fonts/arial.ttf");
    sf::Text notFound(secondary);
    notFound.setString("Station Not Found!");
    notFound.setCharacterSize(20);
    notFound.setFillColor(sf::Color(255, 0, 0));
    notFound.setOutlineColor(sf::Color::Black);
    notFound.setOutlineThickness(2);
    auto notFoundSize = notFound.getLocalBounds().size;
    notFound.setOrigin({notFoundSize.x, notFoundSize.y});
    notFound.setPosition(sf::Vector2f(front.width / 1.98f, front.height / 2.7f));

    sf::Text found(secondary);
    found.setString("Station Found!");
    found.setCharacterSize(20);
    found.setFillColor(sf::Color(0, 255, 0));
    found.setOutlineColor(sf::Color::Black);
    found.setOutlineThickness(2);
    auto foundSize = found.getLocalBounds().size;
    found.setOrigin({foundSize.x, foundSize.y});
    found.setPosition(sf::Vector2f(front.width / 2.03f, front.height / 2.7f));

    bool want2type = false;
    bool notFoundBool = false;
    bool foundBool = false;

    while (window.isOpen()) {

        // lets you exit program by clicking x in the top right
        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
            if (event->is<sf::Event::MouseButtonPressed>()) {
                const auto& mouseButton = event->getIf<sf::Event::MouseButtonPressed>();
                sf::Vector2f mousePress(static_cast<float>(mouseButton->position.x), static_cast<float>(mouseButton->position.y));

                // if the textbox is clicked by the mouse, indicates user wants to type in the box, set to true
                if (textBox.getGlobalBounds().contains(mousePress)) {
                    want2type = true;
                }
            }

            if (event->is<sf::Event::TextEntered>() && want2type == true) {
                const auto& stationSelect = event->getIf<sf::Event::TextEntered>();
                    char input = static_cast<char>(stationSelect->unicode);
                // if backspace
                    if (input == '\b') {
                        // if it's not empty, deletes most recent
                        if (!stationInput.empty()) {
                            stationInput.pop_back();
                        }
                    }else if (isdigit(input) && !isspace(input) && stationInput.size() < 4){
                    stationInput.push_back(input);
                    }else if(!isdigit(input) && !isspace(input) && stationInput.size() < 4) {
                        stationInput.push_back(toupper(input));
                    }
                stationText.setString(stationInput);
                if (stationInput.empty()) {
                    notFoundBool = false;
                    foundBool = false;
                }
                }

        if (const auto* enter = event->getIf<sf::Event::KeyPressed>() ) {
            if (enter->code == sf::Keyboard::Key::Enter) {
                if (stations.find(stationInput) != stations.end()) {
                    front.station = stationInput;
                    notFoundBool = false;
                     foundBool = true;
                }else {
                    notFoundBool = true;
                    foundBool = false;
                }
            }
        }
        }

        // making things display/appear
        window.clear(background);
        window.draw(clock);
        window.draw(title);
        window.draw(station);
        window.draw(textBox);
        window.draw(stationText);
        if (notFoundBool) {
            window.draw(notFound);
        }
        if (foundBool) {
            window.draw(found);
        }
        window.display();
    }

    // vector<metData> test;
    // readCSV("GNV.csv", test);
    // for (int i = 0; i < test.size(); i++) {
    //     cout << test[i].station << ", ";
    //     cout << test[i].valid << ", ";
    //     cout << test[i].tmpf <<"F. "<< endl;
    //
    // }
    getStation(stationInput);
    cout << front.station << endl;
}
