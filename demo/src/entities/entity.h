#ifndef ENTITY_H
#define ENTITY_H

#include <godot_cpp/classes/character_body2d.hpp>
#include <godot_cpp/classes/collision_shape2d.hpp>
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

class Entity : public CharacterBody2D {
    GDCLASS(Entity, CharacterBody2D)
    protected:
        // Health points (life) of the entity
        // max_hp is to be used in healing and such so as to not heal too much
        // while hp will contain the current hp of the entity
        int hp = 100;
        int max_hp = 100;

        int mana = 50;
        int max_mana = 50;
        // The movement speed of the entity
        double speed = 300.0f;
        double base_speed = 300.0f;
        // The direction of the movement or the velocity if you prefer
        Vector2 direction = Vector2(0, 0);
        // A godot built in node that helps in simulating collisons between 
        // different entites
        CollisionShape2D* collision_shape;

    protected:
        static void _bind_methods();  

    public:
        // Constructor
        Entity();
        // Destructor
        ~Entity();
        // The ready function is called when spawning an entity, it does all the setup necessary 
        virtual void _ready() override;
        // _process is called every frame to handle different actions like animations
        // delta is the time between two consecutive frames, it is used to make movement and other actions 
        // independant from the frame rate of the device used to play.
        virtual void _process(double delta) override;
        // _physics_process is the same as _process in principle, but it handles all the physics of the game
        // such as movement, atacking, pathfinding. Godot sets it to 60fps by default
        virtual void _physics_process(double delta) override;

        void take_damage(int dmg_amount);
        virtual void die();
        
        void set_hp(int value);
        int get_hp() const;
        
        void set_mana(int value);
        int get_mana() const;
};

#endif
