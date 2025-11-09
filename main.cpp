

#include "front_end.h"
#include "map.h"
#include <chrono>
#include <optional>
#include <SFML/Window/Event.hpp>
#include <set>
#include <sstream>
#include <fstream>
#include "hashmap.h"

// make sure to place vsc within cmake-build-debug folder, rn it just reads GNV.csv
FrontEnd front;

// set of all the station names to compare to input
set<string> stationsList = {"AAF", "BCT", "BKV", "BOW", "F95", "FIN", "CEW", "XMR", "VSH", "NFJ","CLW",
"2IS", "54A", "CTY", "CGC", "DAB", "egi", "54J", "DED", "DTS","VPS", "EGC", "FXE", "FMY", "RSW",
"RSW", "FPR", "FHB", "FLL", "GNV", "HWO", "HST", "HRT", "IMM", "INF", "NIP", "CRG", "VQQ", "HEG", "NEN",
"JAX", "NQX", "ISM", "K70","EYW", "42J", "LAL", "LEE", "LCQ", "X07", "LNA", "24J", "MCF", "MAI", "NRB",
"MLB", "MIA", "TMB", "OPF", "AGR", "MTH", "MKY", "COI", "MBF", "NDZ", "APF", "TTS", "EVB", "ORL", "OMN",
"OCF", "OCR", "OBE","MCO", "PFN", "COF", "NPA", "PNS", "PMP", "PGD", "28J", "XFL", "ECP", "TDR", "FPY", "PCM",
"2J9", "PIE", "SFB", "SRQ", "SGJ", "SEF", "SPG", "TLH", "TPA", "TIX", "PAM", "TPF", "VVG", "X21", "1J0", "BCR",
"VDF", "X59", "X26", "VNC", "VRB", "PBI", "NSE", "GIF", "SUA", "F45", "X60", "ZPH"};


// Utilized this for much of the SFML content https://github.com/SFML/SFML.git
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
    sf::Clock mapClock;
    sf::Clock hashmapClock;
    clock.setCharacterSize(50);
    clock.setFillColor(sf::Color(150, 50, 150));
    clock.setOutlineColor(sf::Color::Black);
    clock.setOutlineThickness(2);
    auto clockSize = clock.getLocalBounds().size;
    clock.setOrigin({clockSize.x, clockSize.y});
    clock.setPosition(sf::Vector2f(front.width / 4.f, front.height / 1.2f));

    // Title text
    sf::Text titleLetters(font);
    titleLetters.setString("Temperature Increase Mapping");
    titleLetters.setCharacterSize(90);
    titleLetters.setFillColor(sf::Color(0, 28, 127));
    titleLetters.setOutlineColor(sf::Color::Black);
    titleLetters.setOutlineThickness(2);
    auto titleSize = titleLetters.getLocalBounds().size;
    titleLetters.setOrigin({titleSize.x / 2.f, titleSize.y / 2.f});
    titleLetters.setPosition(sf::Vector2f(front.width / 2.f, 50.f));

    // Input station text
    sf::Text station(font);
    station.setString("Input Station: ");
    station.setCharacterSize(50);
    station.setFillColor(sf::Color(0, 88, 187));
    station.setOutlineColor(sf::Color::Black);
    station.setOutlineThickness(2);
    auto stationSize = station.getLocalBounds().size;
    station.setOrigin({stationSize.x, stationSize.y});
    station.setPosition(sf::Vector2f(front.width / 3.f, front.height / 3.f));

    // Select date text
    sf::Text date(font);
    date.setString("Date Range: ");
    date.setCharacterSize(50);
    date.setFillColor(sf::Color(0, 88, 187));
    date.setOutlineColor(sf::Color::Black);
    date.setOutlineThickness(2);
    auto dateSize = date.getLocalBounds().size;
    date.setOrigin({dateSize.x, dateSize.y});
    date.setPosition(sf::Vector2f(front.width / 1.5f, front.height / 3.f));

    // Textbox
    sf::RectangleShape textBox(sf::Vector2f(150, 50));
    textBox.setFillColor(sf::Color::Transparent);
    textBox.setOutlineColor(sf::Color::Black);
    textBox.setOutlineThickness(2);
    auto textBoxSize = textBox.getLocalBounds().size;
    textBox.setOrigin({textBoxSize.x, textBoxSize.y});
    textBox.setPosition(sf::Vector2f(front.width / 2.3f, front.height / 2.85f));

    // Text that'll be inside textbox
    sf::Text stationText(font);
    stationText.setFont(font);
    stationText.setCharacterSize(45);
    stationText.setFillColor(sf::Color::Black);
    auto stationTextSize = stationText.getLocalBounds().size;
    stationText.setOrigin({stationTextSize.x, stationTextSize.y});
    stationText.setPosition(sf::Vector2f(front.width / 2.81f, front.height / 3.5f));

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
    notFound.setPosition(sf::Vector2f(front.width / 2.27f, front.height / 2.7f));

    sf::Text found(secondary);
    found.setString("Station Found!");
    found.setCharacterSize(20);
    found.setFillColor(sf::Color(0, 255, 0));
    found.setOutlineColor(sf::Color::Black);
    found.setOutlineThickness(2);
    auto foundSize = found.getLocalBounds().size;
    found.setOrigin({foundSize.x, foundSize.y});
    found.setPosition(sf::Vector2f(front.width / 2.33f, front.height / 2.7f));

    // Date specifics begin
    sf::Text oneYear(font);
    oneYear.setString("1 Year");
    oneYear.setCharacterSize(45);
    oneYear.setFillColor(sf::Color(100, 100, 100));
    oneYear.setOutlineColor(sf::Color::Black);
    oneYear.setOutlineThickness(2);
    auto oneYearSize = oneYear.getLocalBounds().size;
    oneYear.setOrigin({oneYearSize.x, oneYearSize.y});
    oneYear.setPosition(sf::Vector2f(front.width / 1.29f, front.height / 3.05f));

    sf::RectangleShape oneYearBox(sf::Vector2f(200, 50));
    oneYearBox.setFillColor(sf::Color::Transparent);
    oneYearBox.setOutlineColor(sf::Color::Black);
    oneYearBox.setOutlineThickness(2);
    auto oneYearBoxSize = textBox.getLocalBounds().size;
    oneYearBox.setOrigin({oneYearBoxSize.x, oneYearBoxSize.y});
    oneYearBox.setPosition(sf::Vector2f(front.width / 1.3f, front.height / 2.85f));

    sf::Text fiveYear(font);
    fiveYear.setString("5 Year");
    fiveYear.setCharacterSize(45);
    fiveYear.setFillColor(sf::Color(100, 100, 100));
    fiveYear.setOutlineColor(sf::Color::Black);
    fiveYear.setOutlineThickness(2);
    auto fiveYearSize = fiveYear.getLocalBounds().size;
    fiveYear.setOrigin({fiveYearSize.x, fiveYearSize.y});
    fiveYear.setPosition(sf::Vector2f(front.width / 1.29f, front.height / 2.45f));

    sf::RectangleShape fiveYearBox(sf::Vector2f(200, 50));
    fiveYearBox.setFillColor(sf::Color::Transparent);
    fiveYearBox.setOutlineColor(sf::Color::Black);
    fiveYearBox.setOutlineThickness(2);
    auto fiveYearBoxSize = fiveYearBox.getLocalBounds().size;
    fiveYearBox.setOrigin({fiveYearBoxSize.x, fiveYearBoxSize.y});
    fiveYearBox.setPosition(sf::Vector2f(front.width / 1.25f, front.height / 2.32f));

    sf::Text tenYear(font);
    tenYear.setString("10 Year");
    tenYear.setCharacterSize(45);
    tenYear.setFillColor(sf::Color(100, 100, 100));
    tenYear.setOutlineColor(sf::Color::Black);
    tenYear.setOutlineThickness(2);
    auto tenYearSize = tenYear.getLocalBounds().size;
    tenYear.setOrigin({tenYearSize.x, tenYearSize.y});
    tenYear.setPosition(sf::Vector2f(front.width / 1.28f, front.height / 2.06f));

    sf::RectangleShape tenYearBox(sf::Vector2f(200, 50));
    tenYearBox.setFillColor(sf::Color::Transparent);
    tenYearBox.setOutlineColor(sf::Color::Black);
    tenYearBox.setOutlineThickness(2);
    auto tenYearBoxSize = tenYearBox.getLocalBounds().size;
    tenYearBox.setOrigin({tenYearBoxSize.x, tenYearBoxSize.y});
    tenYearBox.setPosition(sf::Vector2f(front.width / 1.25f, front.height / 1.96f));

    sf::Text fifteenYear(font);
    fifteenYear.setString("15 Year");
    fifteenYear.setCharacterSize(45);
    fifteenYear.setFillColor(sf::Color(100, 100, 100));
    fifteenYear.setOutlineColor(sf::Color::Black);
    fifteenYear.setOutlineThickness(2);
    auto fifteenYearSize = fifteenYear.getLocalBounds().size;
    fifteenYear.setOrigin({fifteenYearSize.x, fifteenYearSize.y});
    fifteenYear.setPosition(sf::Vector2f(front.width / 1.28f, front.height / 1.77f));

    sf::RectangleShape fifteenYearBox(sf::Vector2f(200, 50));
    fifteenYearBox.setFillColor(sf::Color::Transparent);
    fifteenYearBox.setOutlineColor(sf::Color::Black);
    fifteenYearBox.setOutlineThickness(2);
    auto fifteenYearBoxSize = fifteenYearBox.getLocalBounds().size;
    fifteenYearBox.setOrigin({fifteenYearBoxSize.x, fifteenYearBoxSize.y});
    fifteenYearBox.setPosition(sf::Vector2f(front.width / 1.25f, front.height / 1.7f));

    sf::Text dateInfo(secondary);
    dateInfo.setString("Date ranges from 11/1 - 11/1\neg. 1 year = 11/01/25 - 11/01/24");
    dateInfo.setCharacterSize(22);
    dateInfo.setFillColor(sf::Color::Black);
    dateInfo.setOutlineColor(sf::Color::Black);
    dateInfo.setOutlineThickness(1);
    auto dateInfoSize = dateInfo.getLocalBounds().size;
    dateInfo.setOrigin({dateInfoSize.x, dateInfoSize.y});
    dateInfo.setPosition(sf::Vector2f(front.width / 1.5f, front.height / 2.48f));

    sf::Text units(font);
    units.setString("Units:");
    units.setCharacterSize(50);
    units.setFillColor(sf::Color(0, 88, 187));
    units.setOutlineColor(sf::Color::Black);
    units.setOutlineThickness(2);
    auto unitsSize = units.getLocalBounds().size;
    units.setOrigin({unitsSize.x, unitsSize.y});
    units.setPosition(sf::Vector2f(front.width / 4.8f, front.height / 2.26f));

    sf::Text fahrenheit(font);
    fahrenheit.setString("F");
    fahrenheit.setCharacterSize(45);
    fahrenheit.setFillColor(sf::Color(230, 20, 50));
    fahrenheit.setOutlineColor(sf::Color::Black);
    fahrenheit.setOutlineThickness(2);
    auto fahrenheitSize = fahrenheit.getLocalBounds().size;
    fahrenheit.setOrigin({fahrenheitSize.x, fahrenheitSize.y});
    fahrenheit.setPosition(sf::Vector2f(front.width / 3.55f, front.height / 2.27f));

    sf::Text celsius(font);
    celsius.setString("C");
    celsius.setCharacterSize(45);
    celsius.setFillColor(sf::Color(20, 20, 188));
    celsius.setOutlineColor(sf::Color::Black);
    celsius.setOutlineThickness(2);
    auto celsiusSize = celsius.getLocalBounds().size;
    celsius.setOrigin({celsiusSize.x, celsiusSize.y});
    celsius.setPosition(sf::Vector2f(front.width / 2.55f, front.height / 2.26f));

    sf::RectangleShape unitsBoxF(sf::Vector2f(150, 50));
    unitsBoxF.setFillColor(sf::Color::Transparent);
    unitsBoxF.setOutlineColor(sf::Color::Black);
    unitsBoxF.setOutlineThickness(2);
    auto unitsBoxFSize = unitsBoxF.getLocalBounds().size;
    unitsBoxF.setOrigin({unitsBoxFSize.x, unitsBoxFSize.y});
    unitsBoxF.setPosition(sf::Vector2f(front.width / 3.1f, front.height / 2.15f));

    sf::RectangleShape unitsBoxC(sf::Vector2f(150, 50));
    unitsBoxC.setFillColor(sf::Color::Transparent);
    unitsBoxC.setOutlineColor(sf::Color::Black);
    unitsBoxC.setOutlineThickness(2);
    auto unitsBoxCSize = unitsBoxC.getLocalBounds().size;
    unitsBoxC.setOrigin({unitsBoxCSize.x, unitsBoxCSize.y});
    unitsBoxC.setPosition(sf::Vector2f(front.width / 2.3f, front.height / 2.15f));

    sf::RectangleShape goButtonBox(sf::Vector2f(150, 50));
    goButtonBox.setFillColor(sf::Color::Transparent);
    goButtonBox.setOutlineColor(sf::Color::Black);
    goButtonBox.setOutlineThickness(2);
    auto goButtonBoxSize = goButtonBox.getLocalBounds().size;
    goButtonBox.setOrigin({goButtonBoxSize.x, goButtonBoxSize.y});
    goButtonBox.setPosition(sf::Vector2f(front.width / 1.75f, front.height / 1.4f));

    sf::Text goButton(font);
    goButton.setString("Go!");
    goButton.setCharacterSize(45);
    goButton.setFillColor(sf::Color(20, 188, 50));
    goButton.setOutlineColor(sf::Color::Black);
    goButton.setOutlineThickness(2);
    auto goButtonSize = goButton.getLocalBounds().size;
    goButton.setOrigin({goButtonSize.x, goButtonSize.y});
    goButton.setPosition(sf::Vector2f(front.width / 1.85f, front.height / 1.45f));

    sf::Text mapText(font);
    mapText.setString("Map");
    mapText.setCharacterSize(45);
    mapText.setFillColor(sf::Color(200, 88, 88));
    mapText.setOutlineColor(sf::Color::Black);
    mapText.setOutlineThickness(2);
    auto mapTextSize = mapText.getLocalBounds().size;
    mapText.setOrigin({mapTextSize.x, mapTextSize.y});
    mapText.setPosition(sf::Vector2f(front.width / 2.39f, front.height / 1.75f));

    sf::Text hashmapText(font);
    hashmapText.setString("Hashmap");
    hashmapText.setCharacterSize(30);
    hashmapText.setFillColor(sf::Color(200, 88, 88));
    hashmapText.setOutlineColor(sf::Color::Black);
    hashmapText.setOutlineThickness(2);
    auto hashmapTextSize = hashmapText.getLocalBounds().size;
    hashmapText.setOrigin({hashmapTextSize.x, hashmapTextSize.y});
    hashmapText.setPosition(sf::Vector2f(front.width / 3.15f, front.height / 1.75f));

    sf::Text DS(font);
    DS.setString("DS: ");
    DS.setCharacterSize(50);
    DS.setFillColor(sf::Color(0, 88, 187));
    DS.setOutlineColor(sf::Color::Black);
    DS.setOutlineThickness(2);
    auto DSSize = DS.getLocalBounds().size;
    DS.setOrigin({DSSize.x, DSSize.y});
    DS.setPosition(sf::Vector2f(front.width / 5.4f, front.height / 1.78f));

    sf::RectangleShape mapTextBox(sf::Vector2f(150, 50));
    mapTextBox.setFillColor(sf::Color::Transparent);
    mapTextBox.setOutlineColor(sf::Color::Black);
    mapTextBox.setOutlineThickness(2);
    auto mapTextBoxSize = mapTextBox.getLocalBounds().size;
    mapTextBox.setOrigin({mapTextBoxSize.x, mapTextBoxSize.y});
    mapTextBox.setPosition(sf::Vector2f(front.width / 3.1f, front.height / 1.7f));

    sf::RectangleShape hashmapTextBox(sf::Vector2f(150, 50));
    hashmapTextBox.setFillColor(sf::Color::Transparent);
    hashmapTextBox.setOutlineColor(sf::Color::Black);
    hashmapTextBox.setOutlineThickness(2);
    auto hashmapTextBoxSize = hashmapTextBox.getLocalBounds().size;
    hashmapTextBox.setOrigin({hashmapTextBoxSize.x, hashmapTextBoxSize.y});
    hashmapTextBox.setPosition(sf::Vector2f(front.width / 2.3f, front.height / 1.7f));

    // c for celsius
    bool c = false;
    bool want2type = false;
    bool notFoundBool = false;
    bool foundBool = false;
    int year = 0;
    bool mapButtonBool = false;
    bool hashmapButtonBool = false;
    long long startYear = 0;
    long long endYear = 2025;

    while (window.isOpen()) {
        // lets you exit program by clicking x in the top right
        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }

            // UNIT BOX CLICK REGISTRY START
            if (event->is<sf::Event::MouseButtonPressed>()) {
                const auto& mouseButton = event->getIf<sf::Event::MouseButtonPressed>();
                sf::Vector2f mousePress(static_cast<float>(mouseButton->position.x), static_cast<float>(mouseButton->position.y));
                if (unitsBoxF.getGlobalBounds().contains(mousePress)) {
                    c = false;
                    unitsBoxF.setOutlineColor(sf::Color(0, 200, 0));
                    unitsBoxF.setOutlineThickness(4);
                    unitsBoxC.setOutlineColor(sf::Color::Black);
                    unitsBoxC.setOutlineThickness(2);
                }
                else if (unitsBoxC.getGlobalBounds().contains(mousePress)) {
                    c = true;
                    unitsBoxC.setOutlineColor(sf::Color(0, 200, 0));
                    unitsBoxC.setOutlineThickness(4);
                    unitsBoxF.setOutlineColor(sf::Color::Black);
                    unitsBoxF.setOutlineThickness(2);
                }
                // UNIT BOX CLICK REGISTRY END

                // YEAR BOX START
                if (oneYearBox.getGlobalBounds().contains(mousePress)) {
                    year = 1;
                    oneYearBox.setOutlineColor(sf::Color(0, 200, 0));
                    oneYearBox.setOutlineThickness(4);
                    fiveYearBox.setOutlineColor(sf::Color::Black);
                    fiveYearBox.setOutlineThickness(2);
                    tenYearBox.setOutlineColor(sf::Color::Black);
                    tenYearBox.setOutlineThickness(2);
                    fifteenYearBox.setOutlineColor(sf::Color::Black);
                    fifteenYearBox.setOutlineThickness(2);
                }
                else if (fiveYearBox.getGlobalBounds().contains(mousePress)) {
                    year = 5;
                    oneYearBox.setOutlineColor(sf::Color::Black);
                    oneYearBox.setOutlineThickness(2);
                    fiveYearBox.setOutlineColor(sf::Color(0, 200, 0));
                    fiveYearBox.setOutlineThickness(4);
                    tenYearBox.setOutlineColor(sf::Color::Black);
                    tenYearBox.setOutlineThickness(2);
                    fifteenYearBox.setOutlineColor(sf::Color::Black);
                    fifteenYearBox.setOutlineThickness(2);
                }
                else if (tenYearBox.getGlobalBounds().contains(mousePress)) {
                    year = 10;
                    oneYearBox.setOutlineColor(sf::Color::Black);
                    oneYearBox.setOutlineThickness(2);
                    fiveYearBox.setOutlineColor(sf::Color::Black);
                    fiveYearBox.setOutlineThickness(2);
                    tenYearBox.setOutlineColor(sf::Color(0, 200, 0));
                    tenYearBox.setOutlineThickness(4);
                    fifteenYearBox.setOutlineColor(sf::Color::Black);
                    fifteenYearBox.setOutlineThickness(2);

                }
                else if (fifteenYearBox.getGlobalBounds().contains(mousePress)) {
                    year = 15;
                    oneYearBox.setOutlineColor(sf::Color::Black);
                    oneYearBox.setOutlineThickness(2);
                    fiveYearBox.setOutlineColor(sf::Color::Black);
                    fiveYearBox.setOutlineThickness(2);
                    tenYearBox.setOutlineColor(sf::Color::Black);
                    tenYearBox.setOutlineThickness(2);
                    fifteenYearBox.setOutlineColor(sf::Color(0, 200, 0));
                    fifteenYearBox.setOutlineThickness(4);
                }

                // YEAR BOX END
                // TEXTBOX BOOL START
                if (textBox.getGlobalBounds().contains(mousePress)) {
                    want2type = true;
                }
                //TEXTBOX BOOL END

                // MAP & HASHMAP BOX START
                if (mapTextBox.getGlobalBounds().contains(mousePress)) {
                    mapButtonBool = true;
                    hashmapButtonBool = false;
                    mapTextBox.setOutlineColor(sf::Color(0, 200, 0));
                    mapTextBox.setOutlineThickness(4);
                    hashmapTextBox.setOutlineColor(sf::Color::Black);
                    hashmapTextBox.setOutlineThickness(2);

                }
                if (hashmapTextBox.getGlobalBounds().contains(mousePress)) {
                    hashmapButtonBool = true;
                    mapButtonBool = false;
                    hashmapTextBox.setOutlineColor(sf::Color(0, 200, 0));
                    hashmapTextBox.setOutlineThickness(4);
                    mapTextBox.setOutlineColor(sf::Color::Black);
                    mapTextBox.setOutlineThickness(2);

                }
                // MAP AND HASHMAP BOX END

                // GO BUTTON & IMPLEMENTATION START
                if (foundBool == true && year != 0 && goButtonBox.getGlobalBounds().contains(mousePress)) {
                    goButtonBox.setOutlineColor(sf::Color(0, 200, 0));
                    goButtonBox.setOutlineThickness(4);
                    // MAP IMPLEMENTATION START
                    if (hashmapButtonBool == true && mapButtonBool == false) {
                        hashmapClock.restart();
                        vector<pair<long long,string>> result;
                        vector<pair<long long,string>> resultCelsius;
                        map<string, map<long long,string>> createdMap;
                        vector<pair<long long,string>> resultFahrenheit;

                        createdMap = createMap("15YearStations.csv");

                        if (year == 1) {
                            startYear = 202411010053;
                            endYear = 202511010053;
                        }if (year == 5) {
                            startYear = 202011010053;
                            endYear = 202511010053;
                        }if (year == 10) {
                            startYear = 201511010053;
                            endYear = 202511010053;
                        }if (year == 15) {
                            startYear = 201011010053;
                            endYear = 202511010053;
                        }

                        resultFahrenheit = outputFromMap(createdMap, startYear, endYear, stationInput);
                        result = averageCalc(resultFahrenheit);
                        // celsius
                        if (c == true) {
                            result = celsiusCalc(result);
                            for (const auto& [date, temp] : result) {
                                string dateString = to_string(date);
                                cout << dateString << " ";
                                cout << setprecision(4);
                                cout << temp << "C" << endl;
                            }
                        }
                        // fahrenheit
                        if (c == false) {
                            for (const auto& [date, temp] : result) {
                                string dateString = to_string(date);
                                cout << dateString << " ";
                                cout << setprecision(4);
                                cout << temp << "F" << endl;
                            }
                        }
                        sf::Time hashmapTime = hashmapClock.getElapsedTime();
                        stringstream ss;
                        ss << "Map Time : " << hashmapTime.asSeconds() << " seconds";
                        clock.setString(ss.str());
                    }
                    // MAP IMPLEMENTATION END

                    // HASHMAP IMPLEMENTATION START
                    if (mapButtonBool == true && hashmapButtonBool == false) {
                        mapClock.restart();
                        vector<pair<long long, float>> hashMap;
                        if (year == 1) {
                            int tempYear = 2024;
                            hashMap = weatherMap("15YearStations.csv", tempYear, 2025, c, stationInput);
                        }
                        if (year == 5) {
                            int tempYear = 2020;
                            hashMap = weatherMap("15YearStations.csv", tempYear, 2025, c, stationInput);
                        }
                        if (year == 10) {
                            int tempYear = 2015;
                            hashMap = weatherMap("15YearStations.csv", tempYear, 2025, c, stationInput);
                        }
                        if (year == 15) {
                            int tempYear = 2010;
                            hashMap = weatherMap("15YearStations.csv", tempYear, 2025, c, stationInput);
                        }

                        for (const auto& f : hashMap) {
                            if (c == true) {
                                cout << f.first << " " << setprecision(4) << f.second << "C" << endl;
                            }
                            if (c == false) {
                                cout << f.first << " " << setprecision(4) <<  f.second << "F" << endl;
                            }
                        }
                        sf::Time hashmapTime = hashmapClock.getElapsedTime();
                        stringstream ss;
                        ss << "Hashmap Time : " << hashmapTime.asSeconds() << " seconds";
                        clock.setString(ss.str());
                    }
                    // HASHMAP IMPLEMENTATION END
                }
                // GO BUTTON END
            }

            // TYPING IN STATION TEXTBOX START
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
                    if (stationsList.find(stationInput) != stationsList.end()) {
                        front.station = stationInput;
                        notFoundBool = false;
                        foundBool = true;
                    }else {
                        notFoundBool = true;
                        foundBool = false;
                    }
                }
            }
            // TYPING IN TEXTBOX END
        }
        // WINDOW DISPLAY
        window.clear(background);
        window.draw(clock);
        window.draw(titleLetters);
        window.draw(station);
        window.draw(textBox);
        window.draw(stationText);
        window.draw(date);
        window.draw(oneYear);
        window.draw(oneYearBox);
        window.draw(fiveYear);
        window.draw(fiveYearBox);
        window.draw(tenYear);
        window.draw(tenYearBox);
        window.draw(fifteenYear);
        window.draw(fifteenYearBox);
        window.draw(dateInfo);
        window.draw(units);
        window.draw(unitsBoxF);
        window.draw(fahrenheit);
        window.draw(unitsBoxC);
        window.draw(celsius);
        window.draw(mapTextBox);
        window.draw(mapText);
        window.draw(hashmapText);
        window.draw(hashmapTextBox);
        window.draw(clock);
        window.draw(DS);

        if (notFoundBool) {
            window.draw(notFound);
        }
        if (foundBool) {
            window.draw(found);
        }
        if (foundBool == true && year != 0 && (hashmapButtonBool == true || mapButtonBool == true)) {
            window.draw(goButton);
            window.draw(goButtonBox);
        }
        window.display();
    }
}
