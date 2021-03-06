#include "GameplayStage.h"


GamePlayStage::GamePlayStage() :time(sf::Time::Zero), isWaitingForPlayers(true), isInit(false) ,dt(0.01)
{
}
bool GamePlayStage::init(std::string &str)
{
	if (isInit)
		return true;

	int dt = 0.01; // <-dt !!!
	auto &resource = ResourcesManager::getInstanceRef();
	ex_ptr = std::make_shared<entityx::EntityX>();
	phisics_ptr = std::make_shared<Phisics_2D>((*ex_ptr), ResourcesManager::getInstanceRef().vertCont, sf::Vector2f(0, 3));
	(*phisics_ptr).init();

	resource.camera.reset(sf::FloatRect(0, 0, 17.5, 10));

	ResourcesManager::getInstanceRef().areAllPlatfIncluded = false;

	(*ex_ptr).systems.add<engine_system>((*ex_ptr).events, *phisics_ptr);
	(*ex_ptr).systems.add<DestructionSystem>(*ex_ptr);
	(*ex_ptr).systems.add<CraneSystem>(*ex_ptr, *phisics_ptr);
	(*ex_ptr).systems.add<platform_manager>((*ex_ptr).events);
	(*ex_ptr).systems.add<CargoSystem>();
	(*ex_ptr).systems.add<game_over_system>();
	(*ex_ptr).systems.add<enemy_system>((*ex_ptr), *phisics_ptr);
	(*ex_ptr).systems.add<sliding_doors_system>();

	//loading players
	int xPos = 1;
	playersInfoStr = playersInfoStr + "$1;"; //action code
	for (auto &p : player)
	{
		xPos += 2;
		std::string typeOfShip, shipColor;
		float actualEngineForce;

		typeOfShip = p.bodyName;

		if (typeOfShip.size() == 0)
		{
			auto &stage = ResourcesManager::getInstanceRef();

			std::cout << "WYBIERZ JAKIS STATEK" << std::endl;
			//stage.creator_stage.set();
			return false;
		}

		shipColor = "MAGNETA";//parser.getString();

		auto playerEn = (*ex_ptr).entities.create();
		playerEn.assign<isPlayer>(p.uniqueId);
		(*phisics_ptr).createPolygon(playerEn, sf::Vector2f(xPos, 8),
			sf::Vector2f(0, 0), 0, 1, typeOfShip);


		//
		playersInfoStr = playersInfoStr + std::to_string(p.uniqueId) + ";" + typeOfShip + ";" + std::to_string(xPos) + ";8; " + "0;";
		//


		//PARTS
		std::string partName, partKey, pathToPart, partColor, partVert;
		Conversion conversion;
		float partPosX, partPosY, partDegree;


		for (int i = 0; i < p.parts.namePart.size(); ++i)
		{
			typeOfShip = p.parts.namePart[i];//type of part
			partPosX = p.parts.posPart[i].x;
			partPosY = p.parts.posPart[i].y;
			partDegree = p.parts.degreePart[i];
			//partKey = p.parts.namePart[i];
			ConfigParser parser2;
			std::cout << "PARTS:" << p.parts.namePart[i] << " " << p.parts.posPart[i].x << " " << p.parts.posPart[i].y << " " << p.parts.degreePart[i] << std::endl;
			//PART INFO LOAD engine maybe legs or anything else
			pathToPart.clear();
			pathToPart.insert(pathToPart.size(), "resources/parts/");
			pathToPart.insert(pathToPart.size(), typeOfShip);
			pathToPart.insert(pathToPart.size(), ".cfg");
			if (!parser2.load(pathToPart))
			{
				assert(false);
			}
			while (!parser2.EndOfSection())
			{
				parser2.setSection("name");
				partVert = parser2.getString();
			}
			auto &container = ResourcesManager::getInstanceRef().vertCont;

			auto partEn = (*ex_ptr).entities.create();

			parser2.setSection("engineForce");
			actualEngineForce = parser2.getFloat();
			std::cout << " ENGINE FORCE :" << actualEngineForce << std::endl;

			sf::Vector2f engineForce = sf::Vector2f(0, -actualEngineForce);

			sf::Transform transForce;
			transForce.rotate(partDegree);
			engineForce = transForce * engineForce;

			partEn.assign<AttachToPlayerPoint>(sf::Vector2f(partPosX, partPosY));
			partEn.assign<PolyName>(partVert);
			partEn.assign<PartInfo>(p.uniqueId, i);
			std::cout << std::endl;
			if (partVert == "crane")
			{
				partEn.assign<isCrane>();

				partEn.assign<KeyAssigned>(conversion.string_to_sf_key[partKey]);
			}
			else if (partVert == "small_engine" || partVert == "large_engine")
			{
				std::cout << "engine loaded" << std::endl;
				partEn.assign<isEngine>();

				partEn.assign<ForcePoint>(engineForce);

				partEn.assign<KeyAssigned>(conversion.string_to_sf_key[partKey]);
			}
			else if (partVert == "gun")
			{
				std::cout << "gun loaded" << std::endl;

				partEn.assign<isGun>();

				partEn.assign<KeyAssigned>(conversion.string_to_sf_key[partKey]);
			}
			else {
				std::cout << "legs loaded" << std::endl;

				//partEn.assign<isLegs>();
				partEn.assign<isSlave>(playerEn);
				(*phisics_ptr).createPolygon(partEn, sf::Vector2f(0, 0), sf::Vector2f(0, 0), partDegree, 1, partVert);
				continue;
			}
			partEn.assign<Rotation>(partDegree);
			partEn.assign<VertexArray>(container.getPoly(partVert), container.getNormals(partVert));
			partEn.assign<Transform>(sf::Vector2f(0, 0), 0);
			partEn.assign<LinearVelocity>(sf::Vector2f(0, 0));
		}
		std::cout << "init" << std::endl;
	}
	playersInfoStr = playersInfoStr + ";";
	std::cout << "init" << std::endl;
	isInit = true;
	//system("pause");
}

bool GamePlayStage::LoadPart(ResourcesManager::UniquePlayer player)
{

}

bool GamePlayStage::isFull()
{
	std::cout << std::endl;
	for(auto&p :player)
	std::cout << p.uniqueId  << std::endl;
	if (player.size() > 1)
	{
		return true;
	}
	return false;
}

bool GamePlayStage::waitingForPlayers(std::string & str)
{
	if (!isWaitingForPlayers)
	{
		return false;
	}
	else if (isFull())
	{
		init(str);
		isWaitingForPlayers = false;
		return true;
	}
	else
	{
		std::cout << "waiting" << std::endl;
		return true;
	}
}
bool GamePlayStage::decodeDataFromClient(std::string &str)
{

}
//tldr -> sending position of players;
bool GamePlayStage::collectDataToSend(std::string &str)
{
	auto &resource = ResourcesManager::getInstanceRef();
	Position::Handle posH;
	Rotation::Handle rotH;
	LinearVelocity::Handle linVelH;
	isPlayer::Handle isPlayerH;
	str = str + "$2;";
	//std::cout << "entities size:" << (*ex_ptr).entities.size() <<std::endl;
	for (auto en : (*ex_ptr).entities.entities_with_components<>(posH, rotH, isPlayerH, linVelH))
	{
		//std::cout << "data collecting" << std::endl;
		std::cout << "BEF position in SERWER:" << posH->pos.x << " " <<posH->pos.y << std::endl;
		//posH->pos.x = posH->pos.x % 0.01;
		//if((posH->pos.x > 0.01 || posH->pos.x < -0.01) && (posH->pos.y > 0.01 || posH->pos.y < -0.01))
		str = str + std::to_string(isPlayerH->id) + ";"
			+ std::to_string(posH->pos.x) + ";" + std::to_string(posH->pos.y) + ";"
			+ std::to_string(linVelH->vel.x) + ";" +std::to_string(linVelH->vel.y)+ ";"
			+ std::to_string(rotH->degree) + ";";
	}
	str = str + ";"; // to make ";;" (end of call)
	return true;
}
/*
Does all stuff needed after collision sended by player
*/
bool GamePlayStage::updateVelocity(unsigned int playersId, std::string & bufferStr)
{
	isPlayer::Handle playerH;
	auto& resource = ResourcesManager::getInstanceRef();
	//std::cout << "string:" << bufferStr << std::endl;
	for (auto en : (*ex_ptr).entities.entities_with_components<>(playerH))
	{
		/////////////////////////////////////////////////////////////////////////////!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!poprawic
		if (playerH->id == playersId) ///TU BARDZO ZLE VEEEEEEEERY BAAAD  IF "==" GOOD 
		{
			sf::Vector2f contact, force, pos;
			Position::Handle posH = en.component<Position>();

			contact.x = stof(resource.decodeOneLineDel(bufferStr));
			contact.y = stof(resource.decodeOneLineDel(bufferStr));

			force.x = stof(resource.decodeOneLineDel(bufferStr));
			force.y = stof(resource.decodeOneLineDel(bufferStr));

			if (force.y == 0.0f && force.x == 0.0f)
			{
				return false;
			}
			if (stof(resource.decodeOneLineRead(bufferStr)) != 0.0f &&
				stof(resource.decodeOneLineRead(bufferStr)) != 0.0f)
			{
				//posH->pos.x = 
					stof(resource.decodeOneLineDel(bufferStr));
				//posH->pos.y = 
					stof(resource.decodeOneLineDel(bufferStr));
			}
			(*ex_ptr).events.emit<ApplyForceEvent>(contact, force, en);
			std::cout << "velocity updt: f:" << playersId <<"  "<<force.x << " " << force.y << "posAft:"<< posH->pos.x << posH->pos.y <<std::endl;
			system("pause");
			break;
		}
	}


	return true;
}
bool GamePlayStage::updatePartsAction(unsigned int playersId, std::string & bufferStr)
{
	bool flag = 0;
	auto & resource = ResourcesManager::getInstanceRef();
	std::string message = "NULL", partId;
	PartInfo::Handle partInfoH;
	while (message != "")
	{
		message = resource.decodeOneLineDel(bufferStr);
		if (!flag)
		{
			flag = 1;//
			partId = message;
		}
		else
		{
			flag = 0;//
			for (auto part : (*ex_ptr).entities.entities_with_components(partInfoH))
			{
				if (partInfoH->playerId != playersId)
					continue;
				else if (partInfoH->partId == stoi(partId))
				{
					partInfoH->isActive = stoi(message);
					break;
				}
			}

		}
	}
	(*ex_ptr).systems.update<engine_system>(dt);
	return true;
}

void GamePlayStage::addPlayersInfoToBuff(unsigned int playersId, std::string & buffer)
{
	//std::cout << "lpay info:" << playersInfoStr << std::endl;
	//buffer.pop_back();
	buffer = buffer + playersInfoStr;
}

bool GamePlayStage::update(std::string &buffer)
{
	//here messsage must look like this :
	//player id; action code; do sth;;
	
	time += clock.restart();
	auto &resource = ResourcesManager::getInstanceRef();

	//std::cout << "dt:" << dt << "time:" << time.asMilliseconds() <<"buffer:" << buffer <<std::endl;
	int playersId = stoi(resource.decodeOneLineDel(buffer));

	if (time.asSeconds() >= dt && isWaitingForPlayers == false)
	{
		//std::cout << "serwer update!" << std::endl;
		std::string actionCode = "-1";

		resource.temporaryId = playersId;
			
		while(!buffer.empty())
			{
			//std::cout <<"buffer in while:" << buffer << std::endl;
			actionCode = resource.decodeOneLineDel(buffer);// != "" ? resource.decodeOneLineDel(buffer) : "0";
				//
				if (actionCode == "#1")
				{
					updateVelocity(playersId, buffer);
				}
				//
				else if (actionCode == "#2")
				{
					updatePartsAction(playersId, buffer);
					//std::cout << "UPDATE PARTS ACTION" << std::endl;
				}
				//forcing players info sending
				else if (actionCode == "#3")
				{
					addPlayersInfoToBuff(playersId, buffer);
					break;
				}
			}
		Position::Handle posH;
		Rotation::Handle rotH;
		LinearVelocity::Handle linVelH;
		isPlayer::Handle isPlayerH;
		//std::cout << "entities size:" << (*ex_ptr).entities.size() <<std::endl;
		for (auto en : (*ex_ptr).entities.entities_with_components<>(posH, rotH, isPlayerH, linVelH))
		{
			std::cout << "AFTER position in SERWER:" << isPlayerH->id << "  " << posH->pos.x << " " << posH->pos.y << std::endl;
		}
		(*phisics_ptr).update(dt);

		//(*ex_ptr).systems.update<DestructionSystem>(dt);
		//(*ex_ptr).systems.update<CraneSystem>(dt);
		//(*ex_ptr).systems.update<platform_manager>(dt);
		//(*ex_ptr).systems.update<CargoSystem>(dt);
		//(*ex_ptr).systems.update<game_over_system>(dt);
		//(*ex_ptr).systems.update<enemy_system>(dt);
		//(*ex_ptr).systems.update<sliding_doors_system>(dt);

		collectDataToSend(buffer);
		//
		time -= sf::seconds(dt);
	}

	return true;
}
bool GamePlayStage::release()
{
	ex_ptr.reset();
	phisics_ptr.reset();

}
GamePlayStage::~GamePlayStage()
{
}
