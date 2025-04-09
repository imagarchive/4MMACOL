#ifndef NPC_H
#define NPC_H

#include "../entity.h"
#include "../player.h"
#include <godot_cpp/classes/animated_sprite2d.hpp>
#include <godot_cpp/classes/a_star_grid2d.hpp>


using namespace godot;

class Npc : public Entity {
    GDCLASS(Npc, Entity) 
    

    private:
        AnimatedSprite2D* npc_sprite = nullptr;
        Player* player = nullptr;
        AStarGrid2D* a_star = nullptr;
        // walkable grid size
        int grid_size = 100;
        // A* path
        PackedVector2Array path_to_player ;
        bool is_attacking = false;
        double attack_timer = 0.0;

    protected:
        static void _bind_methods();

        void load_sprites();
        void set_sprite_animation();
        void find_path();
        void attack();

    public:
        Npc();
        ~Npc();


        void _ready() override;
        void _process (double delta) override;
        void _physics_process (double delta) override;
};

#endif
