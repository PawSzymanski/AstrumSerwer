#include <entityx/entityx.h>
#include <SFML/Graphics.hpp>

class ContainerOfParts
{
public:
	ContainerOfParts();


	//parts to one player
	std::vector<std::string> namePart;
	std::vector<int> degreePart;
	std::vector<sf::Vector2f> posPart;
	std::map<int, bool> isActivePart; //iD and bool
	void addPart(std::string name, sf::Vector2f partPos, int partDegree, int id);

	~ContainerOfParts();
};
