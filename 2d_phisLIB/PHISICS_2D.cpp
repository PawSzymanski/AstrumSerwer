#include "phisics_2d.h"

Phisics_2D::Phisics_2D(entityx::EntityX &ex, Container &cont, sf::Vector2f &gravity)
    :ex(ex), cont(cont), gravity(gravity)
{}

void Phisics_2D::init()
{
    ex.systems.add<ForcesSystem>(ex.events);
    ex.systems.add<CollisionSystem>(gravity);
    ex.systems.add<MovementSystem>();
    ex.systems.add<JointSystem>();
    ex.systems.configure();
}

void Phisics_2D::update(float dt)
{
    ex.systems.update<ForcesSystem>(dt);
    ex.systems.update<MovementSystem>(dt);
    ex.systems.update<CollisionSystem>(dt);
    ex.systems.update<JointSystem>(dt);
}
/*
*
*/
void Phisics_2D::createCircle(entityx::Entity en, sf::Vector2f pos, sf::Vector2f vel, float mass, sf::Color col, float r)
{
    en.assign<Position>(pos);
    en.assign<Rotation>(0);
    en.assign<LinearVelocity>(vel);
    en.assign<AngularVelocity>(0);
    en.assign<Circle>(r, col);
    en.assign<Line>(sf::Vector2f(0,0),sf::Vector2f(r, 0), sf::Color::Black);
    en.assign<Transform>(pos, 0.0);
    en.assign<AngularForce>();
    en.assign<LinearForce>();
    en.assign<Mass>(mass);
    en.assign<MOfInertia>(0.5f* mass *r*r);
    en.assign<Friction>(0.7);
    en.assign<Type>(Type::CIRCLE);
    en.assign<IsResting>();
}
/*
*en, pos, vel, rot, mass, polyindex
*/
void Phisics_2D::createPolygon(entityx::Entity en, sf::Vector2f pos,sf::Vector2f vel, float rotation, float mass, const std::string & polyName)
{
    en.assign<Rotation>(rotation);
    en.assign<Position>(pos);
    en.assign<LinearVelocity>(vel);
    en.assign<AngularVelocity>(0);
    en.assign<Transform>(pos, rotation);
    en.assign<AngularForce>();
    en.assign<LinearForce>();
    en.assign<Mass>(mass);
    en.assign<MOfInertia>(mass*4);//musimy tu odaæ przy tworzeniu funkcji
    en.assign<Friction>(0.7);
    en.assign<VertexArray>(cont.getPoly(polyName), cont.getNormals(polyName));
    en.assign<PolyName>(polyName);
    en.assign<Type>(Type::POLYGON);
    en.assign<IsResting>();
}

entityx::Entity Phisics_2D::createJoint(entityx::Entity en1, entityx::Entity en2, sf::Vector2f pos1, sf::Vector2f pos2, float lenght)
{
    entityx::Entity en = ex.entities.create();
    en.assign<Joint>(en1,en2,pos1,pos2,lenght);
    en.assign<Line>(sf::Vector2f(0,0), sf::Vector2f(0,0), sf::Color::Yellow);
    return en;
}
