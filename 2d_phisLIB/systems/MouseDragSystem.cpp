#include "MouseDragSystem.h"

MouseDragSystem::MouseDragSystem(sf::RenderWindow & win, sf::View & camera, sf::Vector2f& mouse_pos)
    :win(win), camera(camera), line_started(false), mouse_pos(mouse_pos)
{
}

MouseDragSystem::~MouseDragSystem()
{

}

void MouseDragSystem::update(entityx::EntityManager & en, entityx::EventManager & ev, double dt)
{
	
	if (line_started)
	{
		
		lineH->line[0].position = obj_trans->trans * click_pos;
        lineH->line[1].position = mouse_pos;
		
		sf::Vector2f impulse = lineH->line[1].position - lineH->line[0].position;
	
		impulse /= 10.0f;
	
		sf::Transform t;
		sf::Vector2f p = t.rotate(rot->degree) * click_pos;

		ev.emit<ApplyForceEvent>(p, impulse, this->en);
	}

	if (!line_started && sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		for (auto entity : en.entities_with_components(circ, obj_trans, rot))
		{	
            click_pos = mouse_pos ;
			click_pos = obj_trans->trans.getInverse() * click_pos;
			if (circ->r * circ->r > click_pos.x * click_pos.x + click_pos.y * click_pos.y)
			{
				line = en.create();
				lineH = line.assign<Line>(sf::Vector2f(0,0), sf::Vector2f(0,0), sf::Color::Magenta);
				line_started = true;
				this->en = entity;
				return;
			}
		}


		for (auto entity : en.entities_with_components(vert, obj_trans, rot))
		{
            click_pos = mouse_pos;
			click_pos = obj_trans->trans.getInverse() * click_pos;

			float max_sep = -FLT_MAX;
			int vertCount = vert->vert.getVertexCount();
			for (int i=0; i< vertCount; ++i)
			{
				float sep = dot(vert->normals.at(i), click_pos - vert->vert[i].position);
				max_sep = (sep > max_sep) ? sep : max_sep;
			}

			if (max_sep < 0.0f)
			{
				line = en.create();
				lineH = line.assign<Line>(sf::Vector2f(0, 0), sf::Vector2f(0, 0), sf::Color::Magenta);
				line_started = true;
				this->en = entity;
				return;
			}
		}
	}
	else if (line_started && !sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		line.destroy();
		line_started = false;
	}
}
