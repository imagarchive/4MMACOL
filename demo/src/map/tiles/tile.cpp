#include "tile.h"
#include <godot_cpp/classes/sprite2d.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/collision_shape2d.hpp>
#include <godot_cpp/classes/rectangle_shape2d.hpp>
#include <godot_cpp/classes/tile_set_atlas_source.hpp>
#include <godot_cpp/classes/tile_set.hpp>

namespace godot {

Tile::Tile() {
    // sprite = nullptr;
    // collision_shape = nullptr;
    // shape = nullptr;
    // type = FLOOR; // Default to FLOOR type
}

Tile::~Tile() {
    // Cleanup if necessary
    // if (sprite) {
    //     sprite->queue_free();
    // }
    // if (collision_shape) {
    //     collision_shape->queue_free();
    // }
}

void Tile::_ready() {
    // Setup sprite
    sprite = memnew(Sprite2D);
    add_child(sprite);

    // Setup collision shape for walls if needed
    if (type == WALL) {
        collision_shape = memnew(CollisionShape2D);
        RectangleShape2D* rectangle_shape = memnew(RectangleShape2D());
        Vector2 rectangle_size = Vector2 (32,32);
        rectangle_shape->set_size(rectangle_size);
        collision_shape->set_shape(rectangle_shape);

        // Add collision shape to the entity
        add_child(collision_shape);
    } else {
        collision_shape = nullptr; // No collision shape for floors
    }

    update_tile();
}

void Tile::set_tile_type(int new_type) {
    type = new_type;
    update_tile();
}

void Tile::set_position(Vector2 position) {
    Node2D::set_position(position);
    if (collision_shape) {
        collision_shape->set_position(position);
    }
    sprite->set_position(position);
}

int Tile::get_tile_type() const {
    return type;
}

void Tile::update_tile() {
    // Update sprite and collision based on tile type
    ResourceLoader* loader = ResourceLoader::get_singleton();

    // Ref<TileSet> tile_set = ResourceLoader::get_singleton()->load("res://tile_set.tres");
    // int source_id = 0; // Assuming atlas source is the first/only one (it is, well for now)

    // // Get the tile ID based on type
    // Vector2i tile_id = Vector2i(8, 14);

    // // Get the texture from the source
    // Ref<TileSetAtlasSource> source = tile_set->get_source(source_id);
    // Ref<Texture2D> texture = source->get_texture();

    // // Get region
    // Rect2i region = source->get_tile_texture_region(tile_id);

    // // Apply to sprite
    // sprite->set_texture(texture);
    // // sprite->set_region(true);
    // sprite->set_region_rect(region);

    switch (type) {
        case FLOOR:
            sprite->set_texture(loader->load("res://assets/32x32 Tiles/Glowing_Square_Tile.png"));
            // No collision for floors, so set collision_shape to null
            if (collision_shape) {
                collision_shape->queue_free();
                collision_shape = nullptr;
            }
            break;
        case WALL:
             sprite->set_texture(loader->load("res://assets/32x32 Tiles/Brick_wall.png"));
            // Ensure collision shape is present for walls
            if (!collision_shape) {
                collision_shape = memnew(CollisionShape2D);
                RectangleShape2D* rectangle_shape = memnew(RectangleShape2D());
                Vector2 rectangle_size = Vector2 (32,32);
                rectangle_shape->set_size(rectangle_size);
                collision_shape->set_shape(rectangle_shape);

                // Add collision shape to the entity
                add_child(collision_shape);
            }
            break;
        case DOOR:
            // sprite->set_texture(loader->load("res://door.png"));
            // Doors can be handled similarly to floors, but you can add more logic if needed
            if (collision_shape) {
                collision_shape->queue_free();
                collision_shape = nullptr;
            }
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
