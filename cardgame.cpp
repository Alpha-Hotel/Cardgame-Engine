#include <iostream>
#include <vector>
#include <math.h>
#include <map>
#include "includes/entities.hpp"
using namespace std;

class Scene
// Scenes are locations in the game world which may have
// Enemies or Items.
{
protected:
    string area_description;
    vector<Entity *> enemies;
    vector<Item *> items;
    

public:
    string describe_enemies();
    void stock_enemies(Entity *entity)
    // Adds entities to the scene
    {
        this->enemies.push_back(entity);
    }
    void stock_items(Item *item)
    // Adds items to the scene
    {
        this->items.push_back(item);
    }
    void describe_scene()
    // Describes scene visually including background, objects, and enemies.
    {
        cout << area_description;
        cout << describe_enemies();
    }
};
class The_Great_Gate : public Scene
{
public:
    The_Great_Gate()
    {
        this->area_description = "The wide boulevard you woke up on passes between the mammoth buildings around you and strikes out deeper into the city.\n\nToward the center of this great dome the buildings spiral upward and grow grander and more magnificent as they reach for the sky. \n\nWhether those buildings are organic in origin or attempts at replicating the aesthetic of nature is unclear from this far away, but something that's certain is that you've never known any plant or other building material which could hold such massive structures on such spindly legs as some of those central buildings.\n\n\n";
    }
};

class Game_Master
// The game master controls the flow of gameplay and is the interface between different entities
{
private:
    Entity *player;
    vector<Entity *> enemy;
    Scene scene;
    static map<string, int> effects_map;
    static map<string, int> target_map;

public:
    void referee_effects(vector<effect_struct> effects, Entity *target);
    void load_scene(Scene *location)
    {
        location->describe_scene();
    }
};

void test_basic_functions()
// @brief -- used to test various functionalities of the game
{
    Vampire v = Vampire();
    Vampire *vp = &v;
    Game_Master gm = Game_Master();
    Scene a = The_Great_Gate();
    a.stock_enemies(vp);
    gm.load_scene(&a);
    Human_Fighter hf = Human_Fighter();
    a.stock_enemies(&hf);
    gm.load_scene(&a);
    hf.get_deck();
    for (auto i = hf.deck.cards.begin(); i != hf.deck.cards.end(); i++)
    {
        gm.referee_effects((*i)->cast_spell(), &hf);
    }
    gm.referee_effects(hf.deck.cards[0]->cast_spell(), &v);
    hf.get_deck();
    a.describe_scene();
    hf.deck.remove_card(0);

    
}

map<string, int> Game_Master::effects_map = {
    // Used in the Game Master efect_referee switch statement
    // Allows the Game Master to referee the right kind of effect
    {"Damage", 0},
    {"Stun", 1},
    {"Empower", 2},
    {"Bleed", 3},
    {"Heal", 4}};
map<string, int> Game_Master::target_map{
    // Used in the buff portion of Game Master efect_referee switch statement
    // Allows the Game Master to referee the right kind of buff
    {"Accuracy", 0}};



void Game_Master::referee_effects(vector<effect_struct> effects, Entity *target)
// @brief -- Game Master uses this method to implement card effects
// @param -- Effects: vector of the effects of the card being played
//           *target: pointer to target entity i.e player or enemy
//
{
    vector<string> effects_applied;
    for (auto i = effects.begin(); i != effects.end(); i++)
    {
        if (check_accuracy(i->effect_likelihood))
        {
            effects_applied.push_back(i->effect_type);
            switch (effects_map[i->effect_type])
            {
            case 0: // Damage
                target->change_hp(-(i->effect_magnitude));
                break;
            case 1: // Stun
                target->add_condition("Stun", i->effect_magnitude);
                break;
            case 2: // Empower
                switch (target_map[i->buff_type])
                {
                case 0:// Accuracy
                    target->change_global(i->effect_magnitude, i->buff_type);
                }
                break;
            case 3: // Bleed
                cout << "Bleed called\n";
                target->add_condition("Bleed", i->effect_magnitude);
                break;
            case 4: // Heal
                target->change_hp(i->effect_magnitude);
            default:
                break;
            }
        }
    }
}

int main()
{
    test_basic_functions();
}

string Scene::describe_enemies()
    {
        string temp = "There ";
        if (this->enemies.size() == 1)
        {
            temp.append("is ");
            temp.append(this->enemies[0]->description);
            if (this->enemies[0]->bloodied)
            {
                temp.append(this->enemies[0]->bloodied_description);
            }
        }
        else if (this->enemies.size() == 2)
        {
            temp.append("are ");
            temp.append(this->enemies[0]->description);
            temp.append(" and ");
            temp.append(this->enemies[1]->description);
        }
        else if (this->enemies.size() > 2)
        {
            temp.append("are ");
            for (auto i = enemies.begin(); i != enemies.end(); i++)
            {
                if (i == enemies.end())
                {
                    temp.append(" and ");
                }
                temp.append((*i)->description);
                if (i != enemies.end() - 1)
                {
                    temp.append(", ");
                }
            }
        }

        temp.append(" nearby.\n");
        for (auto i = enemies.begin(); i != enemies.end(); i++)
        {
            if ((*i)->bloodied)
            {
                temp = temp + (*i)->bloodied_description;
            }
        }
        return temp;
    }