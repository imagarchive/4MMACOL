#include "player.h"
#include <godot_cpp/variant/vector2.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/animated_sprite2d.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/sprite_frames.hpp>


using namespace godot;

Player::Player() {}

Player::~Player() {}

void Player::_ready()
{   
    add_to_group("Players");
    load_sprites();
    Entity::_ready();
    set_position(Vector2(400, 620));
}

// processes what happens every frame, delta is used to
// take into consideration the difference in frame rates
// the higher the fps is the lower delta is sooo
// math is mathing :b
void Player::_process(double delta)
{
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

    if (direction.x > 0) {
        player_sprite->play("hurt_right");
    }
    else if (direction.x < 0) {
        player_sprite->play("hurt_left");
    }
    else if (direction.y > 0) {
        player_sprite->play("hurt_down");
    }
    else if (direction.y < 0) {
        player_sprite->play("hurt_up");
    }
    else {
        player_sprite->stop();
    }
}


void Player::attack() {
    if (!player_sprite || is_attacking) return;

    is_attacking = true;
    attack_timer = 1.7; // seconds to stay in attack animation

    if (direction.x > 0) {
        player_sprite->play("attack_right");
    } else if (direction.x < 0) {
        player_sprite->play("attack_left");
    } else if (direction.y > 0) {
        player_sprite->play("attack_down");
    } else if (direction.y < 0) {
        player_sprite->play("attack_up");
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
    // ClassDB::ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "animated_sprite", PROPERTY_HINT_RESOURCE_TYPE, "AnimatedSprite2D"),);
}