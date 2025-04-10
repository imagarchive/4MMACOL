#include "npc.h"
#include <godot_cpp/variant/vector2.hpp>
#include <godot_cpp/classes/animated_sprite2d.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/sprite_frames.hpp>
#include <godot_cpp/classes/scene_tree.hpp>

#include <cstdio>
#include <random>

using namespace godot;

Npc::Npc(){
    // Astar instance to do ath finding
}

Npc::~Npc(){
    //
}

void Npc::_ready() {
    std::random_device rd;
    std::mt19937 gen(rd()); // Mersenne Twister PRNG
    std::uniform_int_distribution<int> distX(0, 1000); // X range
    std::uniform_int_distribution<int> distY(0, 680); // Y range

    // Generate random coordinates directly
    int x = distX(gen);
    int y = distY(gen);

    set_position(Vector2(x,y));

    add_to_group("Enemies");
    Array players = get_tree()->get_nodes_in_group("Players");
    if (!players.is_empty()){
        player = Object::cast_to<Player>(players[0]);
    }

    load_sprites();
    Entity::_ready();

    if(a_star == nullptr){
        a_star = memnew(AStarGrid2D);
        a_star->set_region(Rect2i(0, 0, 50,50));
        a_star->set_cell_size(Vector2(64, 64));
        a_star->update();
    }

}

void Npc::_process(double delta) {
    if (is_dying) {
        death_timer -= delta;
        if (death_timer <= 0.0) {
            // Animation finished, now remove the NPC
            queue_free();
        }
        return; // skip other logic while dying
    }

    if (is_hurt) {
        hurt_timer -= delta;
        if (hurt_timer <= 0.0) {
            is_hurt = false;
        }
        return; // skip other logic while hurt
    }

    if (is_attacking) {
        attack_timer -= delta;
        if (attack_timer <= 0) {
            is_attacking = false;
        }
        return;
    }
    set_sprite_animation();
}

void Npc::_physics_process(double delta){
    Entity::_physics_process(delta);
    direction = Vector2(0, 0);
    if (!player) return;

    find_path();

    Vector2 current_pos = get_position();

    
    if (current_pos.distance_to(player->get_position()) < 64.0) {
        attack();
        return;
    }

    if (path_to_player.size() < 2) {
        return;
    }

    Vector2 next_point = path_to_player[1];

    direction.y += next_point.y - current_pos.y;
    direction.x += next_point.x - current_pos.x;

    if (direction.length() > 0) {
        direction = direction.normalized();
        last_valid_direction = direction;
        set_velocity(direction * speed);
        move_and_slide();
    }
}


void Npc::attack() {
    if (!npc_sprite || is_attacking) return;

    is_attacking = true;
    attack_timer = 1.7; // seconds to stay in attack animation

    printf("Playing atack animation for direction: x=%f, y=%f\n", last_valid_direction.x, last_valid_direction.y);
    
    // Choose animation based on direction, with a fallback
    String anim_name = "attack_down"; // Default animation

    if (last_valid_direction.x < 0) {
        npc_sprite->play("attack_left");
    } else if (last_valid_direction.x > 0) {
        npc_sprite->play("attack_right");
    } else if (last_valid_direction.y > 0) {
        npc_sprite->play("attack_down");
    } else if (last_valid_direction.y < 0) {
        npc_sprite->play("attack_up");
    } else {
        npc_sprite->play("attack_down"); // Default attack animation
    }

    // Check if player is in attack range
    if (player) {
        Vector2 to_player = player->get_position() - get_position();
        
        // Attack only if player is within range (adjust range as needed)
        if (to_player.length() < 64.0) { 
            // Deal damage to player
            player->take_damage(10); // Adjust damage amount as needed
            printf("NPC attacked player for 10 damage\n");
        }
    }
}

void Npc::load_sprites(){
    if (npc_sprite == nullptr){
        npc_sprite = memnew(AnimatedSprite2D);

        add_child(npc_sprite);

        ResourceLoader* loader = ResourceLoader::get_singleton();
        Ref<SpriteFrames> npc_sprite_frames = loader->load("res://orc.tres");

        npc_sprite->set_sprite_frames(npc_sprite_frames);
    }
}

void Npc::set_sprite_animation(){
    if (!npc_sprite) return;

    if (direction.x < 0) {
        npc_sprite->play("walk_left");
    }else if (direction.x > 0) {
        npc_sprite->play("walk_right");
    }else if (direction.y > 0) {
        npc_sprite->play("walk_down");
    }else if (direction.y < 0) {
        npc_sprite->play("walk_up");
    }else {
        npc_sprite->stop();
    }
}

void Npc::find_path(){
    if (!player) return;  // Check if player is valid

    Vector2i player_pos = Vector2i (player->get_position());
    Vector2i player_id = Vector2i(player_pos.x/64, player_pos.y/64);

    Vector2i npc_pos = Vector2i (get_position());
    Vector2i npc_id = Vector2i(npc_pos.x/64, npc_pos.y/64);

    path_to_player = a_star->get_point_path(npc_id, player_id);

    // Check if path is valid
    if (path_to_player.size() < 2) {
        path_to_player.clear();
    }
}

void Npc::take_damage(int damage) {
    hp -= damage;
    printf("took %d damage, hp :%d\n", damage, hp);
    
    if (hp <= 0) {
        die(); // Call die instead of queue_free() directly
    } else {
        hurt(); // Only trigger hurt animation if not dying
    }
}

void Npc::hurt() {
    printf("npc hurting\n");
    if (!npc_sprite || is_hurt)
        return;

    is_hurt = true;
    hurt_timer = 1.6; // duration in seconds
    
    printf("Playing hurt animation for direction: x=%f, y=%f\n", last_valid_direction.x, last_valid_direction.y);
    
    // Choose animation based on direction, with a fallback
    String anim_name = "hurt_down"; // Default animation
    
    if (last_valid_direction.x > 0) {
        anim_name = "hurt_right";
    } else if (last_valid_direction.x < 0) {
        anim_name = "hurt_left";
    } else if (last_valid_direction.y > 0) {
        anim_name = "hurt_down";
    } else if (last_valid_direction.y < 0) {
        anim_name = "hurt_up";
    }
    
    printf("Trying to play animation: %s\n", anim_name.utf8().get_data());
    
    // Check if animation exists
    if (npc_sprite->get_sprite_frames()->has_animation(anim_name)) {
        npc_sprite->play(anim_name);
    } else {
        printf("Animation %s not found, trying generic hurt\n", anim_name.utf8().get_data());
        // Try a fallback animation
        if (npc_sprite->get_sprite_frames()->has_animation("hurt")) {
            npc_sprite->play("hurt");
        } else {
            printf("No hurt animation found\n");
        }
    }
}

void Npc::die() {
    printf("npc dying\n");
    if (!npc_sprite || is_dying)
        return;

    is_dying = true;
    death_timer = 1.6; // duration in seconds for death animation to play

    printf("Playing death animation for direction: x=%f, y=%f\n", last_valid_direction.x, last_valid_direction.y);
    
    // Choose animation based on direction, with a fallback
    String anim_name = "die_down"; // Default animation
    
    if (last_valid_direction.x > 0) {
        anim_name = "die_right";
    } else if (last_valid_direction.x < 0) {
        anim_name = "die_left";
    } else if (last_valid_direction.y > 0) {
        anim_name = "die_down";
    } else if (last_valid_direction.y < 0) {
        anim_name = "die_up";
    }
    
    printf("Trying to play animation: %s\n", anim_name.utf8().get_data());
    
    // Check if animation exists
    if (npc_sprite->get_sprite_frames()->has_animation(anim_name)) {
        npc_sprite->play(anim_name);
    } else {
        printf("Animation %s not found, trying generic death\n", anim_name.utf8().get_data());
        // Try a fallback animation
        if (npc_sprite->get_sprite_frames()->has_animation("die")) {
            npc_sprite->play("die");
        } else {
            printf("No death animation found\n");
        }
    }
}

void Npc::_bind_methods(){
    // binding methods to be used in godot
    ClassDB::bind_method(D_METHOD("set_sprite_animation"), &Npc::set_sprite_animation);
    ClassDB::bind_method(D_METHOD("load_sprites"), &Npc::load_sprites);
    ClassDB::bind_method(D_METHOD("find_path"), &Npc::find_path);
    ClassDB::bind_method(D_METHOD("attack"), &Npc::attack);
    ClassDB::bind_method(D_METHOD("take_damage"), &Npc::take_damage);
    ClassDB::bind_method(D_METHOD("hurt"), &Npc::hurt);
    ClassDB::bind_method(D_METHOD("die"), &Npc::die);
}