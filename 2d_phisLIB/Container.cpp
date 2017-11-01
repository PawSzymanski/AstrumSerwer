#include "Container.h"



Container::Container() : textureNumber(0), animationNumber(0)
{
}

void Container::addAnimation(std::string name, sf::Vector2f frameSize, float speed)
{
	animationIndexesMap.insert(std::pair<std::string, int>(name, animationNumber));
	animationFrameSize.push_back(frameSize);
	animationSpeed.push_back(speed);

	++animationNumber;
}

void Container::addPoly(sf::VertexArray & v, int size, std::string name)
{
	std::vector <sf::Vector2f> n;

    indexes[name] = vertexArrays.size();

	vertexArrays.push_back(v);
	for (int i = 0; i <size; ++i)
	{										
		sf::Vector2f a = v[i].position, b = v[(i + 1) % size].position,
			side = a - b;
		sf::Transform trans;
		trans.rotate(90);
		side = trans * side;
		float lenght = sqrt(side.x*side.x + side.y*side.y);
		side /= lenght;
		n.push_back(side);
	}

	normals.push_back(n);
    std::cout << " size :"  <<normals[0].size() << std::endl;
}

void Container::addTexture(sf::Texture texture, std::string vertexName)
{
	textures.push_back(texture);
	textureIndexesMap.insert(std::pair<std::string, int>(vertexName, textureNumber));
	++textureNumber;
}

sf::VertexArray & Container::getPoly(const std::string &name)
{

    if(indexes.find(name) == indexes.end())
    {
        std::cout <<"zla nazwa: "<<name<<std::endl;
		return vertexArrays[indexes["small_engine"]];
        assert(false);
    }

    return vertexArrays[indexes[name]];
}

std::vector<sf::Vector2f> &Container::getNormals(const std::string &name)
{
    if(indexes.find(name) == indexes.end())
    {
        std::cout <<"zla nazwa: "<<name<<std::endl;
        assert(false);
    }

    return normals.at(indexes[name]);
}

bool Container::isTexture(std::string texture)
{
    return (textureIndexesMap.find(texture) != textureIndexesMap.end());
}

sf::Texture & Container::getTexture(std::string texture)
{
    if (textureIndexesMap.find(texture) == textureIndexesMap.end())
	{
		std::cout << "zla nazwa textury: " << texture << std::endl;
		return textures[textureIndexesMap["small_engine"]];
		assert(false);
	}
	return textures[textureIndexesMap[texture]];
	
}


sf::Vector2f Container::getAnimationFrameSize(std::string texture)
{
	return animationFrameSize[animationIndexesMap[texture]];
}

float Container::getAnimationSpeed(sf::String texture)
{
	return animationSpeed[animationIndexesMap[texture]];
}

Container::~Container()
{
}
