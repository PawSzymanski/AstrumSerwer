#include "cranesystem.h"

CraneSystem::CraneSystem(entityx::EntityX &ex, Phisics_2D & phisics)
    :phisics(phisics), c_ev(nullptr)
{
    ex.events.subscribe<CollisionEvent>(*this);

    isPlayer::Handle player;
    for(auto entity
        : ex.entities.entities_with_components(player))
    {
        this->player = entity;
        play_pos = entity.component<Position>();
        play_rot = entity.component<Rotation>();
    }
}

void CraneSystem::update(entityx::EntityManager &en, entityx::EventManager &ev, double dt)
{
    isCrane::Handle crane;
    KeyAssigned::Handle key;
    AttachToPlayerPoint::Handle point;
	CargoSpace::Handle cargoSpaceH;

    if(c_ev)
    {
        auto en1 = c_ev->en1;
        auto en2 = c_ev->en2;

        entityx::Entity hook = (en1.has_component<isHook>())? en1 : en2;
        entityx::Entity hookable = (en1.has_component<Hookable>())? en1 : en2;

        isHook::Handle isH = hook.component<isHook>();
        isH->joint.destroy();
        entityx::Entity crane = isH->crane;
        sf::Vector2f attachPoint = isH->attachPoint;
        hook.destroy();

        Position::Handle hblePos = hookable.component<Position>();
        Rotation::Handle hbleRot = hookable.component<Rotation>();
        sf::Transform trans;
        trans.rotate(hbleRot->degree);


        isCrane::Handle craneH = crane.component<isCrane>();
        craneH->atached = true;
        craneH->joint = phisics.createJoint(player, hookable, attachPoint, trans.getInverse() * (c_ev->contact - hblePos->pos), HOOK_LENGHT);


        delete c_ev;
        c_ev = nullptr;
    }

    for(auto entity : en.entities_with_components(crane, key, point))
    {
        if(sf::Keyboard::isKeyPressed(key->key))
        {
            if(!crane->block)
            {
                std::cout<<"crane switch"<<std::endl;
                crane->block = true;

                if(crane->atached)
                {
                    crane->joint.destroy();
                    crane->atached = false;
                }
                else if(crane->joint.valid())
                {
                    crane->hook.destroy();
                    crane->joint.destroy();
                }
                else
                {
                    sf::Vector2f attach_point = point->point;
                    sf::Vector2f a_p_norm = vecNormalize(attach_point);
                    attach_point += a_p_norm * HOOK_LENGHT/3.0f;
                    sf::Transform trans;
                    trans.rotate(play_rot->degree);
                    attach_point = trans * attach_point;
                    attach_point += play_pos->pos;

                    entityx::Entity new_hook = en.create();
                    phisics.createPolygon(new_hook,attach_point,sf::Vector2f(0,0),0,0.02f,"hook");
                    isHook::Handle isH = new_hook.assign<isHook>();
                    crane->joint = phisics.createJoint(player,new_hook,point->point, sf::Vector2f(0,0), HOOK_LENGHT);
                    crane->hook = new_hook;
                    isH->joint = crane->joint;
                    isH->attachPoint = point->point;
                    isH->crane = entity;
                }
            }
        }
        else
            crane->block = false;
    }
}

void CraneSystem::receive(const CollisionEvent &ev)
{
    auto en1 = ev.en1;
    auto en2 = ev.en2;

    if((!en1.has_component<isHook>() && !en2.has_component<isHook>()) ||
       (!en1.has_component<Hookable>() && !en2.has_component<Hookable>())|| c_ev)
        return;

    //std::cout<<"contact: ["<<ev.contact.x<<","<<ev.contact.y<<"]"<<std::endl;
    c_ev = new CollisionEvent(ev);
}
