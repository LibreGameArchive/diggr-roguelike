#ifndef __ITEMS_H
#define __ITEMS_H

#include <unordered_map>


namespace items {

typedef std::pair<unsigned int, unsigned int> pt;


struct Item {
    std::string tag;
    pt xy;
    unsigned int count;

    Item() : xy(0, 0), count(1) {}

    Item(const std::string& _tag, const pt& _xy, unsigned int c = 1) : 
        tag(_tag), xy(_xy), count(c)
        {}
};


}


namespace serialize {

template <>
struct reader<items::Item> {
    void read(Source& s, items::Item& m) {
        serialize::read(s, m.tag);
        serialize::read(s, m.xy);
        serialize::read(s, m.count);
    }
};

template <>
struct writer<items::Item> {
    void write(Sink& s, const items::Item& m) {
        serialize::write(s, m.tag);
        serialize::write(s, m.xy);
        serialize::write(s, m.count);
    }
};

}


namespace items {

struct Items {

    std::unordered_map< pt, std::vector<Item> > stuff;

    void init() {
        stuff.clear();
    }

    void clear() {
        init();
    }


    template <typename T>
    void generate(neighbors::Neighbors& neigh, rnd::Generator& rng, grid::Map& grid, T& ptsource,
                  counters::Counts& counts, unsigned int level, unsigned int n) {

        bm _z("items::generate");

        std::map<std::string, unsigned int> q = counts.take(rng, level, n);

        for (const auto& i : q) {

            for (unsigned int j = 0; j < i.second; ++j) {

                pt xy;
                if (!ptsource.one_of_floor(rng, xy))
                    return;
                
                const Design& d = designs().get(i.first);

                int _c = 1;

                {
                    double mean = d.gencount.mean;
                    double dev = d.gencount.deviation;

                    if (dev == 0) {
                        _c = mean;

                    } else {
                        _c = rng.gauss(mean, dev);
                    }

                    if (_c <= 0)
                        _c = 1;
                }

                stuff[xy].push_back(Item(i.first, xy, _c));
            }
        }
    }

    size_t stack_size(unsigned int x, unsigned int y) const {
        auto i = stuff.find(pt(x, y));

        if (i == stuff.end()) {
            return false;
        }

        return i->second.size();
    }

    bool get(unsigned int x, unsigned int y, unsigned int z, Item& ret) const {
        auto i = stuff.find(pt(x, y));

        if (i == stuff.end() || z >= i->second.size()) {
            return false;
        }

        auto j = i->second.rbegin() + z;
        ret = *j;
        return true;
    }

    bool take(unsigned int x, unsigned int y, unsigned int z, Item& ret, grender::Grid& grid) {
        auto i = stuff.find(pt(x, y));

        if (i == stuff.end() || z >= i->second.size()) {
            return false;
        }

        auto j = i->second.rbegin() + z;
        ret = *j;
        i->second.erase(j.base()-1);

        if (i->second.empty())
            stuff.erase(i);
        
        grid.invalidate(x, y);

        return true;
    }

    void place(unsigned int x, unsigned int y, const Item& i, grender::Grid& grid) {

        grid.invalidate(x, y);

        auto j = stuff.find(pt(x, y));

        if (j == stuff.end()) {

            auto& v = stuff[pt(x, y)];
            v.push_back(i);
            v.back().xy = pt(x, y);
            return;
        }

        const Design& d = designs().get(i.tag);
        unsigned int icount = i.count;

        for (Item& it : j->second) {

            if (it.tag == i.tag && it.count < d.stackrange) {

                unsigned int n = std::min(icount, d.stackrange - it.count);
                it.count += n;
                icount -= n;

                if (icount == 0)
                    return;
            }
        }

        j->second.push_back(i);
        j->second.back().xy = pt(x, y);
        j->second.back().count = icount;
    }

    void dispose(counters::Counts& counts) {

        for (const auto& j : stuff) {
            for (const Item& i : j.second) {
                const Design& d = designs().get(i.tag);
                counts.replace(d.level, d.tag);
            }
        }
    }

};

}

namespace serialize {

template <>
struct reader<items::Items> {
    void read(Source& s, items::Items& t) {
        serialize::read(s, t.stuff);
    }
};

template <>
struct writer<items::Items> {
    void write(Sink& s, const items::Items& t) {
        serialize::write(s, t.stuff);
    }
};

}


#endif
