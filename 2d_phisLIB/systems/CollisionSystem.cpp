#include "CollisionSystem.h"



CollisionSystem::CollisionSystem(sf::Vector2f &gravity) : gravity(gravity)
{
	dispatch[Type::CIRCLE][Type::CIRCLE] = isCollidingCC;
	dispatch[Type::CIRCLE][Type::POLYGON] = isCollidingCP;
	dispatch[Type::POLYGON][Type::CIRCLE] = isCollidingPC;
	dispatch[Type::POLYGON][Type::POLYGON] = isCollidingPP;
}


void CollisionSystem::PositionalCorrection(Manifold &m)
{
	Position::Handle posH1 = m.en1.component<Position>(), posH2 = m.en2.component<Position>();
    Mass::Handle massH1 = m.en1.component<Mass>(), massH2 = m.en2.component<Mass>();

    if (equal(massH1->invMass + massH2->invMass, 0))
		return;

    sf::Vector2f gravN = vecNormalize(gravity);
    float d1 = dot(-gravN, posH1->pos);
    float d2 = dot(-gravN, posH2->pos);

    bool not_flip = (d1 > d2 || massH2->invMass == 0) && (massH1->invMass != 0);
    Position::Handle targetPos = not_flip? posH1 : posH2;
    Mass::Handle targetMass = not_flip? massH1 : massH2;
	
    sf::Vector2f correction = (m.normal* 1.0f * m.penetration) ;// (massH1->invMass + massH2->invMass);
    targetPos->pos -=  (-1.0f + (2.0f * not_flip)) *correction * (1.0f * (targetMass->invMass != 0));
}



void CollisionSystem::ResolveCollision(Manifold &m, entityx::EventManager & ev, float dt)
{
	LinearVelocity::Handle velH1 = m.en1.component<LinearVelocity>(), velH2 = m.en2.component<LinearVelocity>();
	Position::Handle posH1 = m.en1.component<Position>(), posH2 = m.en2.component<Position>();
	Mass::Handle massH1 = m.en1.component<Mass>(), massH2 = m.en2.component<Mass>();
	AngularVelocity::Handle angvelH1 = m.en1.component<AngularVelocity>(), angvelH2 = m.en2.component<AngularVelocity>();
	MOfInertia::Handle inertH1 = m.en1.component<MOfInertia>(), inertH2 = m.en2.component<MOfInertia>();
	Friction::Handle frH1 = m.en1.component<Friction>(), frH2 = m.en2.component<Friction>();
	IsResting::Handle isRestH1 = m.en1.component<IsResting>(), isRestH2 = m.en2.component<IsResting>();

	isRestH1->isIt = false;
	isRestH2->isIt = false;

	float restitution = 0.2f; //	!	!	!	!	!	!	!	!	!	!	!	!											!	!	!	!	RESTITUTION HERE
	
	if (equal(massH1->invMass + massH2->invMass, 0))
	{
		
		velH1->vel = sf::Vector2f(0, 0);
		velH2->vel = sf::Vector2f(0, 0);
		return;
	}
	for(int i=0; i<m.contactsCount ; ++i)
    {
		 
        sf::Vector2f contact1 = m.contacts[i] - posH1->pos,
            contact2 = m.contacts[i] - posH2->pos;

        sf::Vector2f relativeVel = velH2->vel + crossSV(angvelH2->radians(), contact2) -
            velH1->vel - crossSV(angvelH1->radians(), contact1);

        float contactVel = dot(relativeVel, m.normal);

        if (contactVel > 0)
            return;

        float relVelLeng = vecLenghtSq(relativeVel);
        m.relVel = relVelLeng;

        float gravityLeng = vecLenghtSq(gravity) * dt;

        if (relVelLeng < gravityLeng + EPSILON )
        {
            restitution = 0.f;
        }
        float velLengh1 = vecLenghtSq(velH1->vel),
            velLenght2 =  vecLenghtSq(velH2->vel);

        if (abs_f(crossVV(m.normal, gravity) < EPSILON && dot(m.normal, gravity) < 0) && velLenght2 < gravityLeng + EPSILON)
        {
            isRestH1->isIt = true;
        }

        if (abs_f(crossVV(m.normal, gravity) < EPSILON && dot(m.normal, gravity) < 0) && velLengh1 < EPSILON + gravityLeng)
        {
            isRestH2->isIt = true;
        }

        float contact1XNormal = crossVV(contact1, m.normal);
        float contact2XNormal = crossVV(contact2, m.normal);
        float invMassSum = massH1->invMass + massH2->invMass + sqr(contact1XNormal) *inertH1->invI + sqr(contact2XNormal) *inertH2->invI;

        float force = -(1.0f + restitution) * contactVel;
        force /= invMassSum;

        m.force =  m.normal * force;
        //friction

        sf::Vector2f t =( relativeVel - ( m.normal * dot(relativeVel, m.normal)));
        t= vecNormalize(t);
        //std::cout << "friction: " << t.x <<" "<<t.y<< std::endl;

        float jt = -dot(relativeVel, t);

        jt /= invMassSum;

        if (!equal(jt, 0.0f))
        {
            sf::Vector2f frictionImpulse;

            if (abs_f(jt) < force * 0.7)
                frictionImpulse = t *jt;
            else
                frictionImpulse = t * -force * 0.5f;

            if (vecLenghtSq(frictionImpulse) > 0.001f)
            m.force += frictionImpulse;
        }
        m.force /= static_cast<float>(m.contactsCount);

        entityx::Entity temp_en1 = m.en1;
        entityx::Entity temp_en2 = m.en2;

        if(m.en2.has_component<isSlave>())
        {
            isSlave::Handle slave = m.en2.component<isSlave>();
            temp_en2 = slave->master;
            //std::cout<<"SLAVEEEEEEE"<<std::endl;
        }
        if(m.en1.has_component<isSlave>())
        {
            isSlave::Handle slave = m.en2.component<isSlave>();
            temp_en1 = slave->master;
            //std::cout<<"SLAVEEEEEEE"<<std::endl;
        }
        ev.emit<ApplyForceEvent>(contact2, m.force, temp_en2);
        ev.emit<ApplyForceEvent>(contact1, -m.force, temp_en1);
	}
}

void CollisionSystem::update(entityx::EntityManager & en, entityx::EventManager & ev, double dt)
{
	Position::Handle posH1, posH2;
	Type::Handle typeH1, typeH2;
	LinearForce::Handle linFH1, linFH2;


	entityx::Entity * ens = new entityx::Entity[en.size()];
	int entitiesCount = 0;

	for (auto en : en.entities_with_components(posH1, typeH1, linFH1))
	{
		ens[entitiesCount] = en;
		++entitiesCount;
	}

	for(int i=0; i<entitiesCount; ++i)
	{


		for (int j=i+1; j<entitiesCount; ++j)
		{			
			Manifold m(ens[i],ens[j]);

			typeH1 = ens[i].component<Type>();
			typeH2 = ens[j].component<Type>();
			posH1 = ens[i].component<Position>();
			posH2 = ens[j].component<Position>();

			dispatch[typeH1->type][typeH2->type](m);

			if (!m.contactsCount)
				continue;
		
			ResolveCollision(m,ev,dt);
            PositionalCorrection(m);

            //std::cout<<"rel_vel: "<<m.relVel<<std::endl;

            ev.emit<CollisionEvent>(ens[i], ens[j], m.relVel, m.contacts[0]);
		}
        IsResting::Handle isRestingH = ens[i].component<IsResting>();
        Mass::Handle mass = ens[i].component<Mass>();
        if (!isRestingH->isIt && !ens[i].has_component<isSlave>())
           ev.emit<ApplyForceEvent>(sf::Vector2f(0, 0), gravity * mass->mass * static_cast<float>(dt), ens[i]); //GRAWITEJSZYN

        isRestingH->isIt = false;
	}
}


CollisionSystem::~CollisionSystem()
{
}
