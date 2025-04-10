#include "tile.h"
#include <godot_cpp/classes/sprite2d.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/collision_shape2d.hpp>
#include <godot_cpp/classes/rectangle_shape2d.hpp>
#include <godot_cpp/classes/tile_set_atlas_source.hpp>
#include <godot_cpp/classes/tile_set.hpp>

namespace godot {

Tile::Tile() {
    //
}

Tile::~Tile() {
    //
}

void Tile::_ready() {
    // Setup sprite
    sprite = memnew(Sprite2D);
    add_child(sprite);

    // Setup collision shape
    collision_tile = memnew(CollisionShape2D);
    RectangleShape2D* rectangle_shape = memnew(RectangleShape2D());
    Vector2 rectangle_size = Vector2 (32,32);
    rectangle_shape->set_size(rectangle_size);
    collision_tile->set_shape(rectangle_shape);
    // Add collision shape to the entity
    add_child(collision_tile);

    update_tile();
}

void Tile::set_tile_type(int new_type) {
    type = new_type;
    update_tile();
}

void Tile::set_position(Vector2 position) {
    Node2D::set_position(position);
    if (collision_tile) {
        collision_tile->set_position(position);
    }
    sprite->set_position(position);
}

int Tile::get_tile_type() const {
    return type;
}

void Tile::update_tile() {
    // Update sprite and collision based on tile type
    ResourceLoader* loader = ResourceLoader::get_singleton();

    switch (type) {
        case FLOOR:
            sprite->set_texture(loader->load("res://assets/32x32 Tiles/Glowing_Square_Tile.png"));
            // No collision for floors, so set collision_shape to null
            collision_tile->set_disabled(true);
            break;
        case WALL:
            sprite->set_texture(loader->load("res://assets/32x32 Tiles/Brick_wall.png"));
            // Ensure collision shape is present for walls
            collision_tile->set_disabled(false);
            break;
        case DOOR:
            // sprite->set_texture(loader->load("res://door.png"));
            collision_tile->set_disabled(false);
            break;
    }
}

void Tile::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_tile_type", "new_type"), &Tile::set_tile_type);
    ClassDB::bind_method(D_METHOD("set_position", "position"), &Tile::set_position);
    ClassDB::bind_method(D_METHOD("get_tile_type"), &Tile::get_tile_type);
    ClassDB::bind_method(D_METHOD("update_tile"), &Tile::update_tile);
}

} // namespace godot
