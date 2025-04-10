#include "entity.h"
#include <godot_cpp/variant/vector2.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/animated_sprite2d.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/sprite_frames.hpp>
#include <godot_cpp/classes/collision_shape2d.hpp>
#include <godot_cpp/classes/rectangle_shape2d.hpp>

using namespace godot ;

Entity::Entity(){

}

Entity::~Entity(){
    // still dunno 
}

void Entity::_ready() {
   // 
    collision_shape = memnew(CollisionShape2D);
    RectangleShape2D* rectangle_shape = memnew(RectangleShape2D());
    Vector2 rectangle_size = Vector2 (32,32);
    rectangle_shape->set_size(rectangle_size);
    collision_shape->set_shape(rectangle_shape);

    // Add collision shape to the entity
    add_child(collision_shape);
}

void Entity::_process(double delta){
    // to be overriden in subclasses,
    // animation and ui
}

void Entity::_physics_process(double delta){
    // collisions and movement
}

void Entity::take_damage(int dmg_amount){
    hp -= dmg_amount;

    // Will be overrdien in certain enemies to introduce certain 
    // effects if a certain amount of damage is dealt
    // ex: stun effects, comboes ... 
    if (hp <= 0) {
        die();
    }
}

void Entity::die(){
    // handle death logic
    // to be overriden in each subclass
    // the player's death could trigger a restart, show a score idk
    // mob death despawns the mob, leaves loot, adds exp to the player
    queue_free();
}

void Entity::set_hp(int value){
    hp = value;
}

int Entity::get_hp() const {
    return hp;
}

void Entity::set_mana(int value){
    mana = value;
}

int Entity::get_mana() const {
    return mana;
}

void Entity::_bind_methods(){
    // binding methods to be used in godot
    // ClassDB::bind_method(D_METHOD("attack"), &Entity::attack);
    ClassDB::bind_method(D_METHOD("take_damage"), &Entity::take_damage);
    ClassDB::bind_method(D_METHOD("die"), &Entity::die);
    ClassDB::bind_method(D_METHOD("set_hp", "value"), &Entity::set_hp);
    ClassDB::bind_method(D_METHOD("get_hp"), &Entity::get_hp);
    ClassDB::bind_method(D_METHOD("set_mana", "value"), &Entity::set_mana);
    ClassDB::bind_method(D_METHOD("get_mana"), &Entity::get_mana);
}
