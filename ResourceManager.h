#pragma once
#include "LibsAndDeclarations.h"

//#include "GameplayStage.h"
#include "configparser.h"



//#include "2d_phisLIB/Container.h"
//#include "configparser.h"
/*
*We keep here data that can be transefed 
* or visible to any class or function in whole code.
*
*
*/
class ResourcesManager
{
private:
	ResourcesManager();
	ResourcesManager(const ResourcesManager&) = delete;
	ResourcesManager& operator=(const ResourcesManager&) = delete;
	ResourcesManager(ResourcesManager&&) = delete;
	ResourcesManager& operator=(ResourcesManager&&) = delete;
	~ResourcesManager();

private:
    static ResourcesManager *instance;

private:
	
	void loadContainer();

public:
	static ResourcesManager* getInstance();
	static ResourcesManager& getInstanceRef();
	static void deleteInstance();

public:
	//player in session data
	class UniquePlayer
	{

	public:
		UniquePlayer()
		{

		}
		UniquePlayer(unsigned int i, sf::IpAddress ip) : uniqueId(i), ip(ip), session(0)
		{

		}
		bool isAdmin;
		unsigned int uniqueId, session;
		sf::IpAddress ip;
		std::string name, bodyName;
		std::map <std::string, bool> actionKeys;
		Container parts;
	};

public:
	std::vector< UniquePlayer> uniquePlayers;
	//std::vector<GamePlayStage> GamePlaySt;
 	std::string decodeOneLineDel(std::string & array);
	std::string decodeOneLineRead(std::string  array);
	bool areAllPlatfIncluded, areAllCargoSpaceIncluded, isMouseButtonReleased, isGameOver, isPauseTime, isMovingCameraOn;
	sf::View camera;
	std::string levelInfo;
	std::string shipInfo;
	Container vertCont;
	//Container vertCont;
	//Container textureCont;
    sf::Font font;
	sf::RenderWindow window;
	//stages

	std::shared_ptr<sf::Event> mainEvent;
	

};


