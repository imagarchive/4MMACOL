#ifndef PLAYER_H
#define PLAYER_H

#include "entity.h"
#include <godot_cpp/classes/animated_sprite2d.hpp>

using namespace godot;

class Player : public Entity {
    GDCLASS(Player, Entity)

    private:
        AnimatedSprite2D* player_sprite = nullptr;

        float sprint_multiplier = 1.5;
        int exp;
        int max_xp;
        int lvl;
        bool is_attacking = false;
        double attack_timer = 0.0;
        bool is_hurt = false;
        double hurt_timer = 0.0;


    protected:
        static void _bind_methods();
        void handle_input(double delta);
        void load_sprites();
        void set_sprite_animation();
        void attack();
        
    public:
        Player();
        ~Player();
        void hurt();
        void _ready() override;
        void _process(double delta) override;
        void _physics_process(double delta);

};

#endif