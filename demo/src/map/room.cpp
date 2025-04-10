#include "room.h"
#include "tiles/tile.h"
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/packed_scene.hpp>

namespace godot {

Room::Room() {
    width = 0;
    height = 0;
}

Room::~Room() {
    // Clean up the tiles array
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (tiles[i][j]) {
                tiles[i][j]->queue_free();
            }
        }
    }
}

void Room::_ready() {
    // 
    initialize_room(80,40);
}

void Room::initialize_room(int room_width, int room_height) {
    width = room_width;
    height = room_height;

    // Resize the tiles array to fit the room dimensions
    tiles.resize(height);
    for (int i = 0; i < height; i++) {
        tiles[i].resize(width);
    }

    // Initialize the room with default floor tiles
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            Tile* tile = memnew(Tile);
            tiles[y][x] = tile;
            add_child(tile);
    
            tile->set_position(Vector2(x * 16, y * 16));
    
            // Outer walls
            if (x == 0 || y == 0 || x == width - 1 || y == height - 1) {
                tile->set_tile_type(WALL);
            }else {
                tile->set_tile_type(FLOOR);
            }

            // Add a door in the top wall
            if (y == 0 && x == 10) {
                tile->set_tile_type(DOOR);
            }

        }
    }
    


    update_room();
}

void Room::set_tile(int x, int y, int type) {
    if (x >= 0 && x < width && y >= 0 && y < height) {
        tiles[y][x]->set_tile_type(type);
        tiles[y][x]->update_tile();
    }
}

Tile* Room::get_tile(int x, int y) {
    if (x >= 0 && x < width && y >= 0 && y < height) {
        return tiles[y][x];
    }
    return nullptr;
}

void Room::update_room() {
    // Update all tiles in the room
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            tiles[y][x]->update_tile();
        }
    }
}

int Room::get_width(){
    return width;
}

int Room::get_height(){
    return height;
}


// Update _bind_methods in room.cpp:
void Room::_bind_methods() {
    ClassDB::bind_method(D_METHOD("initialize_room", "width", "height"), &Room::initialize_room);
    ClassDB::bind_method(D_METHOD("set_tile", "x", "y", "type"), &Room::set_tile);
    ClassDB::bind_method(D_METHOD("get_tile", "x", "y"), &Room::get_tile);
    ClassDB::bind_method(D_METHOD("update_room"), &Room::update_room);
    ClassDB::bind_method(D_METHOD("get_width"), &Room::get_width);
    ClassDB::bind_method(D_METHOD("get_height"), &Room::get_height);
    ClassDB::bind_method(D_METHOD("set_next_room"), &Room::set_next_room);
    ClassDB::bind_method(D_METHOD("get_next_room"), &Room::get_next_room);
    ClassDB::bind_method(D_METHOD("set_previous_room"), &Room::set_previous_room);
    ClassDB::bind_method(D_METHOD("get_previous_room"), &Room::get_previous_room);
    
    ClassDB::add_property(get_class_static(), PropertyInfo(Variant::STRING, "next_room"), "set_next_room", "get_next_room");
    ClassDB::add_property(get_class_static(), PropertyInfo(Variant::STRING, "previous_room"), "set_previous_room", "get_previous_room");
}

// Add these accessor methods to room.cpp
void Room::set_next_room(const String& path) {
    next_room = path;
}

String Room::get_next_room() const {
    return next_room;
}

void Room::set_previous_room(const String& path) {
    previous_room = path;
}

String Room::get_previous_room() const {
    return previous_room;
}

} // namespace godot
