#include "player.h"
#include "npc/npc.h"
#include "../map/room.h"

#include <godot_cpp/variant/vector2.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/animated_sprite2d.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/sprite_frames.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/window.hpp> 
#include <godot_cpp/classes/camera2d.hpp>
#include <godot_cpp/classes/global_constants.hpp>



using namespace godot;

Player::Player() {}

Player::~Player() {}

void Player::_ready()
{   
    add_to_group("Players");
    
    // CAmera settings
    Camera2D* camera = memnew(Camera2D);
    add_child(camera);
    camera->make_current();
    camera->set_limit(SIDE_TOP, -16);
    camera->set_limit(SIDE_LEFT, -16);
    camera->set_limit(SIDE_RIGHT, 40*64-16);
    camera->set_limit(SIDE_BOTTOM, 20*64-16);

    camera->set_drag_margin(SIDE_TOP, 0.5);
    camera->set_drag_margin(SIDE_LEFT, 0.5);
    camera->set_drag_margin(SIDE_RIGHT, 0.5);
    camera->set_drag_margin(SIDE_BOTTOM, 0.5);

    camera->set_drag_horizontal_enabled(true);
    camera->set_drag_vertical_enabled(true);

    load_sprites();
    Entity::_ready();
    set_position(Vector2(120, 120));
}

// processes what happens every frame, delta is used to
// take into consideration the difference in frame rates
// the higher the fps is the lower delta is sooo
// math is mathing :b
void Player::_process(double delta)
{
    if (is_dying) {
        death_timer -= delta;
        if (death_timer <= 0.0) {
            // Animation finished, now handle player death
            // For example, show game over screen or respawn
            printf("Player death animation finished\n");
            // is they existed we could use smething like :
            // respawn(); // You would need to implement this
            // get_tree()->call_deferred("reload_current_scene");
            return;
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

void Player::_physics_process(double delta)
{
    Entity::_physics_process(delta);
    handle_input(delta);
}

void Player::handle_input(double delta)
{
    Input *input = Input::get_singleton();
    direction = input->get_vector("ui_left", "ui_right", "ui_up", "ui_down");

    if (input->is_key_pressed(KEY_SHIFT))
    {
        speed = base_speed * sprint_multiplier;
    }
    else
    {
        speed = base_speed;
    }

    if (input->is_key_pressed(KEY_X)){
        attack();
    }

    // Normalize velocity to prevent diagonal movement boost
    if (direction.length() > 0)
    {
        direction = direction.normalized();
        last_valid_direction = direction;
        set_velocity(direction * speed); // Apply velocity
        move_and_slide();               // Move character
    }

    // set_sprite_animation();

    // TODO : delete just using it to reset to 0,0 when i go out of thge map
    if (input->is_key_pressed(KEY_R))
    {
        set_position(Vector2(20, 20));
    }
}

void Player::load_sprites()
{
    if (player_sprite == nullptr)
    {
        player_sprite = memnew(AnimatedSprite2D);

        add_child(player_sprite);

        ResourceLoader *loader = ResourceLoader::get_singleton();
        Ref<SpriteFrames> player_sprite_frames = loader->load("res://vampire1.tres");

        player_sprite->set_sprite_frames(player_sprite_frames);
    }
}

void Player::set_sprite_animation()
{
    if (!player_sprite)
        return;

    if (direction.x > 0)
    {
        player_sprite->play("walk_right");
    }
    else if (direction.x < 0)
    {
        player_sprite->play("walk_left");
    }
    else if (direction.y > 0)
    {
        player_sprite->play("walk_down");
    }
    else if (direction.y < 0)
    {
        player_sprite->play("walk_up");
    }
    else
    {
        player_sprite->stop();
    }
}

void Player::hurt() {
    printf("player hurting\n");
    if (!player_sprite || is_hurt)
        return;

    is_hurt = true;
    hurt_timer = 1.2; // duration in seconds

    if (last_valid_direction.x > 0) {
        player_sprite->play("hurt_right");
    }
    else if (last_valid_direction.x < 0) {
        player_sprite->play("hurt_left");
    }
    else if (last_valid_direction.y > 0) {
        player_sprite->play("hurt_down");
    }
    else if (last_valid_direction.y < 0) {
        player_sprite->play("hurt_up");
    }
    else {
        // Default hurt animation
        player_sprite->play("hurt_down");
    }
}


void Player::attack() {
    if (!player_sprite || is_attacking) return;

    is_attacking = true;
    attack_timer = 1.7; // seconds to stay in attack animation

    if (last_valid_direction.x > 0) {
        player_sprite->play("attack_right");
    } else if (last_valid_direction.x < 0) {
        player_sprite->play("attack_left");
    } else if (last_valid_direction.y > 0) {
        player_sprite->play("attack_down");
    } else if (last_valid_direction.y < 0) {
        player_sprite->play("attack_up");
    }

    // --- ATTACK LOGIC ---
    Array enemies = get_tree()->get_nodes_in_group("Enemies");
    for (int i = 0; i < enemies.size(); i++) {
        Npc* npc = Object::cast_to<Npc>(enemies[i]);
        if (npc) {
            Vector2 to_npc = npc->get_position() - get_position();

            // Adjust range and direction to your liking
            if (to_npc.length() < 64.0) {
                // Optional: only attack in front of you
                Vector2 facing = last_valid_direction.normalized();
                if (facing.dot(to_npc.normalized()) > 0.5) {
                    npc->take_damage(25);
                    break; // only hit one NPC
                }
            }
        }
    }
}

void Player::die() {
    printf("player dying\n");
    if (!player_sprite || is_dying)
        return;

    is_dying = true;
    death_timer = 1.5; // duration in seconds for death animation to play

    // Play death animation based on the last direction
    if (last_valid_direction.x > 0) {
        player_sprite->play("die_right");
    }
    else if (last_valid_direction.x < 0) {
        player_sprite->play("die_left");
    }
    else if (last_valid_direction.y > 0) {
        player_sprite->play("die_down");
    }
    else if (last_valid_direction.y < 0) {
        player_sprite->play("die_up");
    }
    else {
        // Default death animation if no direction
        player_sprite->play("die_down");
    }
    
    // Game over or respawn logic goes here
    // For example:
    // get_tree()->call_deferred("reload_current_scene");
    // Or show a game over screen
}

void Player::take_damage(int dmg_amount) {
    Entity::take_damage(dmg_amount); // Call base class method
    
    // Check if we should die or just get hurt
    if (get_hp() <= 0) {
        die();
    } else {
        hurt();
    }
}

void Player::_bind_methods()
{
    // binding methods to be used in godot
    ClassDB::bind_method(D_METHOD("set_sprite_animation"), &Player::set_sprite_animation);
    ClassDB::bind_method(D_METHOD("load_sprites"), &Player::load_sprites);
    ClassDB::bind_method(D_METHOD("handle_input"), &Player::handle_input);
    ClassDB::bind_method(D_METHOD("hurt"), &Player::hurt);
    ClassDB::bind_method(D_METHOD("attack"), &Player::attack);
    ClassDB::bind_method(D_METHOD("die"), &Player::die);
    ClassDB::bind_method(D_METHOD("take_damage"), &Player::take_damage);
    // ClassDB::ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "animated_sprite", PROPERTY_HINT_RESOURCE_TYPE, "AnimatedSprite2D"),);
}