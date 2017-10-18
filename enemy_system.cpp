#include "enemy_system.h"
#include "ResourceManager.h"


enemy_system::enemy_system(entityx::EntityX &ex, Phisics_2D &phs) : phisics(phs), detectionSpace(sf::Vector2f(0, 0.1)), dir(true)
{
	ex.events.subscribe<CollisionEvent>(*this);
}


void enemy_system::update(entityx::EntityManager & en, entityx::EventManager & ev, double dt)
{
	Position::Handle posCamH, posPlaH;
	isEnemyCam::Handle camH;
	isPlayer::Handle playerH;
	Rotation::Handle rotCamH;	

	//shooting camera
	for (auto ent : en.entities_with_components<>(camH, posCamH))
	{
		if (!camH->isActive)
			continue;

		entityx::Entity newEnt;
		for (auto e : en.entities_with_components<>(playerH))
		{
			newEnt = e;
		}
		posPlaH = newEnt.component<Position>();
		rotCamH = ent.component<Rotation>();
	
		if (vecLenghtSq(detectionSpace) < 1.1 && dir)
		{
			detectionSpace -= sf::Vector2f(0 ,0.4);		
		}
		else
		{
			dir = false;
			if ((vecLenghtSq(detectionSpace) < 0.25))
				dir = true;
			
			detectionSpace += sf::Vector2f(0 ,0.4);
		}
		sf::Transform trans ;
		trans.rotate(rotCamH->degree);
		trans.scale(sf::Vector2f(5,5));
		detectionSpace = trans * detectionSpace;
		detectionSpace += posCamH->pos;

		//std::cout << rotCamH->degree << " det: " << detectionSpace.x << " " << detectionSpace.y << std::endl;

		if (bulletTime.asMilliseconds() > 180 &&
			posPlaH->pos.x > detectionSpace.x - 0.5 &&
			posPlaH->pos.x < detectionSpace.x + 0.5 &&
			posPlaH->pos.y > detectionSpace.y - 0.5 &&
			posPlaH->pos.y < detectionSpace.y + 0.5 )
		{
			auto bulletEn = en.create();
			bulletTime = sf::Time::Zero;

			phisics.createPolygon(bulletEn, posCamH->pos,
				trans * sf::Vector2f(0, -8), 0, 0.1, "bullet");
			bulletEn.assign<isBullet>(true);
		}
		bulletTime += bulletClock.restart();

		//end, don't edit
		detectionSpace -= posCamH->pos;
		detectionSpace = trans.getInverse() * detectionSpace;
	}
}

void enemy_system::cameraDesrtoy(const CollisionEvent &ev)
{

	if ((ev.en1.has_component<isBullet>() || ev.en2.has_component<isBullet>()) &&
		(ev.en1.has_component<isEnemyCam>() || ev.en2.has_component<isEnemyCam>()))
	{
		bool en1IsBullet = ev.en1.has_component<isBullet>(),
			en2IsBullet = ev.en2.has_component<isBullet>();

		entityx::Entity bullet = en1IsBullet ? ev.en1 : ev.en2;

		isBullet::Handle isBulH = bullet.component<isBullet>();

		if (isBulH->isEnemy)
			return;

		bool en1IsCam = ev.en1.has_component<isEnemyCam>(),
			en2IsCam = ev.en2.has_component<isEnemyCam>();

		entityx::Entity cameraEnt = en1IsCam ? ev.en1 : ev.en2;

		VertexArray::Handle verH = cameraEnt.component<VertexArray>();
		isEnemyCam::Handle CamH = cameraEnt.component<isEnemyCam>();
		std::cout << "Camera destroyed" << std::endl;

		CamH->isActive = false;
		verH->vert[0].color = sf::Color::Green;
		verH->vert[1].color = sf::Color::Green;
		verH->vert[2].color = sf::Color::Green;
		verH->vert[3].color = sf::Color::Green;
	}

}


void enemy_system::receive(const CollisionEvent &ev)
{
	cameraDesrtoy(ev);
}

enemy_system::~enemy_system()
{
}
