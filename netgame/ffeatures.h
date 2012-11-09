#ifndef __FEATURES_H
#define __FEATURES_H

#include <unordered_map>


namespace features {

typedef std::pair<unsigned int, unsigned int> pt;


struct Feature {
    std::string tag;
    pt xy;
    unsigned int decay;

    Feature() : xy(0, 0), decay(0) {}

    Feature(const std::string& _tag, const pt& _xy, unsigned int d) : 
        tag(_tag), xy(_xy), decay(d)
        {}
};


}


namespace serialize {

template <>
struct reader<features::Feature> {
    void read(Source& s, features::Feature& m) {
        serialize::read(s, m.tag);
        serialize::read(s, m.xy);
        serialize::read(s, m.decay);
    }
};

template <>
struct writer<features::Feature> {
    void write(Sink& s, const features::Feature& m) {
        serialize::write(s, m.tag);
        serialize::write(s, m.xy);
        serialize::write(s, m.decay);
    }
};

}


namespace features {

struct Features {

    std::unordered_map<pt, Feature> feats;

    void init() {
        feats.clear();
    }

    void clear() {
        init();
    }

    void set(unsigned int x, unsigned int y, const std::string& tag) {
        // Check that the tag exists.
        const Terrain& t = terrain().get(tag);

        pt xy(x, y);
        feats[xy] = Feature(tag, xy, t.decay);
    }

    bool get_placement(rnd::Generator& rng, grid::Map& grid, Terrain::placement_t p, pt& ret) {

        switch (p) {

        case Terrain::placement_t::floor:
            if (!grid.one_of_floor(rng, ret)) return false;
            break;

        case Terrain::placement_t::water:
            if (!grid.one_of_lake(rng, ret)) return false;
            break;

        case Terrain::placement_t::corner:
            if (!grid.one_of_corner(rng, ret)) return false;
            break;
        }

        grid.add_nogen(ret.first, ret.second);
        return true;
    }

    void generate(rnd::Generator& rng, grid::Map& grid, const std::string& tag, unsigned int n) {

        // Check that the tag exists.
        const Terrain& ter = terrain().get(tag);

        for (unsigned int i = 0; i < n; ++i) {

            pt xy;

            if (!get_placement(rng, grid, ter.placement, xy)) 
                break;
            
            const Terrain& t = terrain().get(tag);

            std::cout << "+++ " << tag << " " << xy.first << " " << xy.second << std::endl;
            feats[xy] = Feature(tag, xy, t.decay);
        }
    }

    bool get(unsigned int x, unsigned int y, Feature& ret) {
        auto i = feats.find(pt(x, y));

        if (i == feats.end()) {
            return false;
        }

        ret = i->second;
        return true;
    }

    template <typename FUNC>
    void process(grender::Grid& render, FUNC f) {

        std::unordered_set<pt> wipe;

        for (auto& i : feats) {
            const Terrain& t = terrain().get(i.second.tag);

            if (!f(i.second, t)) {
                wipe.insert(i.first);
            }
        }

        for (const auto& xy : wipe) {
            feats.erase(xy);
            render.invalidate(xy.first, xy.second);
        }
    }

    inline void write(serialize::Sink& s) {
        serialize::write(s, feats);
    }

    inline void read(serialize::Source& s) {
        serialize::read(s, feats);
    }
};


}

#endif
