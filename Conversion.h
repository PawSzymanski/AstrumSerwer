#pragma once
#include "LibsAndDeclarations.h"
class Conversion
{

public:
	
	std::map <std::string, sf::Keyboard::Key> string_to_sf_key = { 
	{ "A",sf::Keyboard::A },{ "B",sf::Keyboard::B },{ "C",sf::Keyboard::C },{ "D",sf::Keyboard::D },
	{ "E",sf::Keyboard::E },{ "F",sf::Keyboard::F },{ "G",sf::Keyboard::G },{ "G",sf::Keyboard::H },
	{ "G",sf::Keyboard::I },{ "J",sf::Keyboard::J },{ "K",sf::Keyboard::K },{ "L",sf::Keyboard::L },
	{ "M",sf::Keyboard::M },{ "N",sf::Keyboard::N },{ "O",sf::Keyboard::O },{ "P",sf::Keyboard::P },
	{ "Q",sf::Keyboard::Q },{ "R",sf::Keyboard::R },{ "S",sf::Keyboard::S },{ "T",sf::Keyboard::T },
	{ "U",sf::Keyboard::U },{ "V",sf::Keyboard::V },{ "W",sf::Keyboard::W },{ "X",sf::Keyboard::X },
	{ "Y",sf::Keyboard::Y },{ "Z",sf::Keyboard::Z } };
	
	std::map <std::string, sf::Color> string_to_sf_Color{ 
	{"CYAN",sf::Color::Cyan},					{ "BLACK",sf::Color::Black },
	{ "BLUE",sf::Color::Blue },					{ "GREEN",sf::Color::Green },
	{ "MAGNETA",sf::Color::Magenta },			{ "RED",sf::Color::Red },
	{ "WHITE",sf::Color::White },				{ "YELLOW",sf::Color::Yellow },
	{ "TRANSPARENT",sf::Color::Transparent } };
	Conversion() {};
	~Conversion() {};
};

