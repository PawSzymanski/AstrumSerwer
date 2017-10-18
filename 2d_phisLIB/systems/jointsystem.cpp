#include "jointsystem.h"

JointSystem::JointSystem()
{

}

sf::Vector2f JointSystem::resolve(Joint::Handle joint, float& sep, sf::Vector2f &normal)
{
    const float restitution = 0.1f;

    Transform::Handle t = joint->en.component<Transform>();
    Transform::Handle t1 = joint->en1.component<Transform>();

    LinearVelocity::Handle vel = joint->en.component<LinearVelocity>();
    LinearVelocity::Handle vel1 = joint->en1.component<LinearVelocity>();

    AngularVelocity::Handle angvel = joint->en.component<AngularVelocity>();
    AngularVelocity::Handle angvel1 = joint->en1.component<AngularVelocity>();

    Mass::Handle mass = joint->en.component<Mass>();
    Mass::Handle mass1 = joint->en1.component<Mass>();

    MOfInertia::Handle inert = joint->en.component<MOfInertia>();
    MOfInertia::Handle inert1 = joint->en1.component<MOfInertia>();

    sf::Vector2f point = (t->trans * joint->pos);
    sf::Vector2f point1 = (t1->trans * joint->pos1);
    sf::Vector2f joint_vec = point1 - point;
    float dist = vecLenght(joint_vec);

    if(dist <= joint->lenght)
        return sf::Vector2f(0,0);

    sep = joint->lenght - dist;
    normal = vecNormalize(joint_vec);

    sf::Vector2f relativeVel = vel1->vel + crossSV(angvel1->radians(), joint->pos1) -
        vel->vel - crossSV(angvel->radians(), joint->pos);

    float contactVel = dot(relativeVel, normal);
    if (contactVel < 0)
        return sf::Vector2f(0,0);

    float contact1XNormal = crossVV(joint->pos, normal);
    float contact2XNormal = crossVV(joint->pos1, normal);
    float invMassSum = mass->invMass + mass1->invMass + sqr(contact1XNormal) *inert->invI + sqr(contact2XNormal) *inert1->invI;

    float force = -(1.0f + restitution) * contactVel;
    force /= invMassSum;
    return(normal * force);
}

void JointSystem::correct(Joint::Handle joint, float sep, sf::Vector2f normal)
{
    Position::Handle pos = joint->en.component<Position>();
    Position::Handle pos1 = joint->en1.component<Position>();
    Mass::Handle mass = joint->en.component<Mass>();
    Mass::Handle mass1 = joint->en1.component<Mass>();

    sf::Vector2f correction = (normal* 0.2f * sep);
    pos->pos -=  correction * (1.0f * (mass->invMass != 0));
    pos1->pos += correction * (1.0f * (mass1->invMass != 0));
}

void JointSystem::update(entityx::EntityManager &en, entityx::EventManager &ev, double dt)
{
    Joint::Handle joint;
    Line::Handle line;
    for (auto entity : en.entities_with_components(joint, line))
    {
        sf::Vector2f normal;
        float separation;
        auto force = resolve(joint, separation, normal);
        correct(joint, separation, normal);

        Transform::Handle t = joint->en.component<Transform>();
        Transform::Handle t1 = joint->en1.component<Transform>();
        sf::Vector2f point = (t->trans * joint->pos);
        sf::Vector2f point1 = (t1->trans * joint->pos1);
        Rotation::Handle rot = joint->en.component<Rotation>();
        Rotation::Handle rot1 = joint->en1.component<Rotation>();
        sf::Transform rotm, rot1m;
        rotm.rotate(rot->degree);
        rot1m.rotate(rot1->degree);

        ev.emit<ApplyForceEvent>( rotm * joint->pos, -force, joint->en);
        ev.emit<ApplyForceEvent>( rot1m * joint->pos1, force, joint->en1);

        line->line[0].position = point;
        line->line[1].position = point1;
        line->line[0].color = sf::Color::Yellow;
        line->line[1].color = sf::Color::Yellow;
    }
}
