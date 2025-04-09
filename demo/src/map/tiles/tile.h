#ifndef TILE_H
#define TILE_H

#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/classes/sprite2d.hpp>
#include <godot_cpp/classes/collision_shape2d.hpp>
#include <godot_cpp/classes/rectangle_shape2d.hpp>
#include <godot_cpp/classes/tile_set_atlas_source.hpp>

#define FLOOR 1
#define WALL 2
#define DOOR 3

namespace godot {

class Tile : public Node2D {
    GDCLASS(Tile, Node2D);

    private:
        int type;
        Sprite2D* sprite;
        CollisionShape2D* collision_shape;

    public:
        Tile();
        ~Tile();

        void _ready() override;
        void set_tile_type(int new_type);
        void set_position(Vector2 position);
        void update_tile();
        int get_tile_type() const;

    protected:
        static void _bind_methods();
    };

} // namespace godot

#endif // TILE_H
