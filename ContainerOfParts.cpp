#include "ContainerOfParts.h"

ContainerOfParts::ContainerOfParts()
{
}

void ContainerOfParts::addPart(std::string partName, sf::Vector2f partPos, int partDegree, int id)
{
	namePart.push_back(partName);
	degreePart.push_back(partDegree);
	posPart.push_back(partPos);
	isActivePart[id] = false;
	std::cout << "part loaded" << std::endl;
}

ContainerOfParts::~ContainerOfParts()
{
}
