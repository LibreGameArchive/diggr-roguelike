#ifndef __NEIGHBORS_H
#define __NEIGHBORS_H

#include <vector>
#include <map>
#include <set>


namespace neighbors {

typedef std::pair<unsigned int, unsigned int> pt;


struct Neighbors {
    
    typedef std::map<pt, std::set<pt> > ns_t;
    ns_t nbmap;
    unsigned int w;
    unsigned int h;

    void init(unsigned int _w, unsigned int _h) {

        w = _w;
        h = _h;

        nbmap.clear();

        for (unsigned int x = 0; x < w; x++) {
            for (unsigned int y = 0; y < h; y++) {

                auto& v = nbmap[std::make_pair(x, y)];

                for (int xi = -1; xi <= 1; xi++) {
                    for (int yi = -1; yi <= 1; yi++) { 

                        if (xi == 0 && yi == 0) continue;

                        pt tmp(x+xi, y+yi);

                        if (tmp.first < 0 || tmp.first >= w || tmp.second < 0 || tmp.second >= h)
                            continue;

                        v.insert(v.end(), tmp);
                    }
                }
            }
        }
    }

    void clear() {
        init(w, h);
    }

    const std::set<pt>& operator()(const pt& xy) const {
        static std::set<pt> empty;

        ns_t::const_iterator i = nbmap.find(xy);

        if (i != nbmap.end()) {
            return i->second;
	}
        
        return empty;
    }

    bool linked(const pt& xy1, const pt& xy2) const {
        return operator()(xy1).count(xy2);
    }

    //***  ***//

    inline void write(serialize::Sink& s) {
        serialize::write(s, w);
        serialize::write(s, h);
    }

    inline void read(serialize::Source& s) {
        serialize::read(s, w);
        serialize::read(s, h);
        init(w, h);
    }

};


inline Neighbors& get() {
    static Neighbors ret;
    return ret;
}


}

#endif
