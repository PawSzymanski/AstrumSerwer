#include "LoginStage.h"
#include "ResourceManager.h"


LoginStage::LoginStage()
{
}
bool LoginStage::checkIfNumerExistInBase(unsigned int i)
{
	for (auto player : ResourcesManager::getInstanceRef().uniquePlayers)
	{
		if (player.uniqueId == i)
			return false;
	}
	return true;
}
unsigned int LoginStage::giveUniqueNum()
{
	int num;
	std::random_device rd;
	while (!checkIfNumerExistInBase(num = rd()) || num < 100)
	{ }
	return num;
}

bool LoginStage::receiveDataFromClient(std::string &bufferStr)
{
	//action == 1;ip;login;passoword;
	//serwer
	sf::UdpSocket socket;
	
	char buffer[128] = { 0 };
	unsigned short port;

	std::size_t received = 0;
	sf::IpAddress clientIp;

	auto &resource = ResourcesManager::getInstanceRef();

	std::cout << "buffer:" << bufferStr << std::endl;//"login:" << bufferLogin << "pass:" << bufferPass << std::endl;

	//Decoding buffer
	
		//type of action
		clientIp = resource.decodeOneLineDel(bufferStr);
		loginStr = resource.decodeOneLineDel(bufferStr);
		passStr = resource.decodeOneLineDel(bufferStr);
	
		std::cout << "after dec buff:" << bufferStr << std::endl;

	if(doExistUserInBase())
	{
		unsigned int unNum = giveUniqueNum();
		
		ResourcesManager::UniquePlayer player(unNum, clientIp);
		ResourcesManager::getInstanceRef().uniquePlayers.push_back(player);
		
		bufferStr = "2;" +
					std::to_string(unNum);
	}
	else
	{
		bufferStr = "0";
		return false;
	}
	return true;
}

bool LoginStage::doExistUserInBase()
{
	try {
		sql::Driver *driver;
		sql::Connection *con;
		sql::Statement *stmt;
		//sql::ResultSet *res;
		sql::PreparedStatement *pstmt;

		// Create a connection 
		driver = get_driver_instance();
		con = driver->connect("tcp://127.0.0.1:3306", "root", "");
		std::cout << "logged" << std::endl;
		// Connect to the MySQL test database 

		con->setSchema("astrum-multi");
		//system("pause");
		stmt = con->createStatement();

		//res = stmt->executeQuery();
		//std::cout << "is login in base:  " << stmt->execute("SELECT login FROM players WHERE players.login=2") << std::endl;
		stmt->execute("CALL check_log_pass("+loginStr+","+passStr+",@pop)");// + loginStr + "," + passStr + ",@pop)");
		//std::cout << "poszlo" << std::endl;

		std::auto_ptr<sql::ResultSet> res(stmt->executeQuery("SELECT @pop AS _reply"));

		while (res->next())
			if (!res->getInt("_reply"))
			{
				return false;
			}
			//std::cout << "is player in base (>0?) " << res->getInt("_reply") << std::endl;

		delete stmt;

		// '?' is the supported placeholder syntax */
		//pstmt = con->prepareStatement("INSERT INTO position(id) VALUES (?)");
		//for (int i = 1; i <= 10; i++) {
		//	pstmt->setInt(1, i);
		//	pstmt->executeUpdate();
		//}
		//delete pstmt;

		//		 Select in ascending order 
		//pstmt->add_country();
		//		pstmt = con->prepareStatement("SELECT xDim FROM position ");
		//res = pstmt->executeQuery();

		//Fetch in reverse = descending order!
		//res->afterLast();

		//delete res;

		//delete pstmt;
		delete con;

	}
	catch (sql::SQLException &e) {
		std::cout << "# ERR: SQLException in " << __FILE__;
		std::cout << "(" << __FUNCTION__ << ") on line "
			<< __LINE__ << std::endl;
		std::cout << "# ERR: " << e.what();
		std::cout << " (MySQL error code: " << e.getErrorCode();
		std::cout << ", SQLState: " << e.getSQLState() <<
			" )" << std::endl;
		return false;
	}

	return true;
}

bool LoginStage::update(std::string &data)
{
	receiveDataFromClient(data);

	return true;
}

bool LoginStage::waitingForPlayers(std::string & str)
{
	return false;
}


LoginStage::~LoginStage()
{
}
