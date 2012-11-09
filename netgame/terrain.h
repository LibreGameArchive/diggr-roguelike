#ifndef __TERRAIN_H
#define __TERRAIN_H

#include <string>

struct Terrain {
    enum class placement_t : unsigned int {
        floor,
        water,
        corner
    };

    std::string tag;
    std::string name;
    maudit::glyph skin;

    unsigned int count;

    placement_t placement;

    unsigned int stairs;
    int tunnel_x;
    int tunnel_y;

    bool viewblock;
    bool walkblock;

    unsigned int decay;
    
    Terrain() : count(0), placement(placement_t::floor), stairs(0), tunnel_x(0), tunnel_y(0),
                viewblock(false), walkblock(false), decay(0) {}

};

#endif
