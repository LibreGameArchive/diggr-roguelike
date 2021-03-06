#ifndef __DESIGNS_H
#define __DESIGNS_H

#include <string>

#include "counters.h"

struct Design {

    std::string tag;
    std::string name;
    maudit::glyph skin;
    
    unsigned int level;
    unsigned int count;

    std::string slot;
    std::string descr;

    damage::attacks_t attacks;
    damage::defenses_t defenses;

    unsigned int stackrange;

    double heal;
    double feed;

    bool usable;

    unsigned int throwrange;

    unsigned int blastradius;
    unsigned int blastrange;

    struct mean_deviation_t {
        double mean;
        double deviation;
        
        mean_deviation_t(double m = 0.0, double d = 0.0) : mean(m), deviation(d) {}
    };

    mean_deviation_t gencount;

    bool melee;

    unsigned int lightradius;

    double digging;

    struct useflags_t {
        bool blink;

        useflags_t() : blink(false) {}
    };

    useflags_t flags;


    Design() : level(0), count(0), stackrange(0), heal(0), feed(0), usable(false), throwrange(0),
               blastradius(0), blastrange(0), gencount(1, 0), melee(false), lightradius(0), digging(0) {}
    
};

#endif
