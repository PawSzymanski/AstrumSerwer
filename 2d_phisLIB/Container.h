#pragma once
#include <vector>
#include <cmath>
#include <entityx/entityx.h>
#include <SFML/Graphics.hpp>

class Container
{
public:
	Container();
	

	int textureNumber, animationNumber;
	std::vector<sf::VertexArray> vertexArrays;
	std::vector<std::vector<sf::Vector2f>> normals;
	std::vector<sf::Texture> textures;
	std::vector<sf::Vector2f> animationFrameSize;
	std::vector<float> animationSpeed;

    std::map <std::string, int> indexes;
	std::map <std::string, int> textureIndexesMap;
	std::map <std::string, int> animationIndexesMap;

	void addAnimation(std::string name, sf::Vector2f frameSize, float speed);
    void addPoly(sf::VertexArray & v, int size, std::string name);
	void addTexture(sf::Texture texture, std::string vertexName);
    
	sf::VertexArray & getPoly(const std::string& name);
    std::vector <sf::Vector2f> & getNormals(const std::string& name);

    bool isTexture( std::string texture);
	sf::Texture & getTexture(std::string texture);

	sf::Vector2f getAnimationFrameSize(std::string texture);
	float getAnimationSpeed(sf::String texture);

	~Container();
};

