#include "room.h"
#include <godot_cpp/classes/resource_loader.hpp>
#include "tiles/tile.h"

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
    initialize_room(50,50);
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

            tile->set_position(Vector2(x * 32, y * 32));

            // Borders are walls
            if (x == 0 || y == 0 || x == width - 1 || y == height - 1) {
                tile->set_tile_type(WALL);
            }
            // Middle walls (example: vertical line in center, horizontal cross)
            else if ((x == width / 2) || (y == height / 2 && x >= width / 3 && x <= 2 * width / 3)) {
                tile->set_tile_type(WALL);
            }
            else {
                tile->set_tile_type(FLOOR);
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

void Room::_bind_methods() {
    ClassDB::bind_method(D_METHOD("initialize_room", "width", "height"), &Room::initialize_room);
    ClassDB::bind_method(D_METHOD("set_tile", "x", "y", "type"), &Room::set_tile);
    ClassDB::bind_method(D_METHOD("get_tile", "x", "y"), &Room::get_tile);
    ClassDB::bind_method(D_METHOD("update_room"), &Room::update_room);
}

} // namespace godot
