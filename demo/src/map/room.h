#ifndef ROOM_H
#define ROOM_H

#include "tiles/tile.h"
#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/classes/scene_tree.hpp>  // Add this header to work with SceneTree
#include <godot_cpp/classes/packed_scene.hpp> // For loading scenes
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <vector>

namespace godot {

class Room : public Node2D {
    GDCLASS(Room, Node2D);

private:
    std::vector<std::vector<Tile*>> tiles; // 2D grid of Tile pointers
    int width, height; // Dimensions of the room
    
public:
    String next_room;
    String previous_room;
    
    Room();
    ~Room();
    
    void _ready() override;
    
    // Initialize the room with the specified dimensions
    void initialize_room(int room_width, int room_height);
    
    // Set the tile at a specific position
    void set_tile(int x, int y, int type);
    
    // Get the tile at a specific position
    Tile* get_tile(int x, int y);
    
    // Update all tiles in the room
    void update_room();
    
    int get_width();
    int get_height();
    
    void switch_room(const String& room_path);

    void _on_door_entered(Tile* door_tile);
    void set_next_room(const String& path);
    String get_next_room() const;
    void set_previous_room(const String& path);
    String get_previous_room() const;

protected:
    static void _bind_methods();
};

} // namespace godot

#endif // ROOM_H
