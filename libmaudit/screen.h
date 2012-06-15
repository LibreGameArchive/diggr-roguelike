#ifndef __MAUDIT_BUFFER_H
#define __MAUDIT_BUFFER_H

#include <ctype.h>
#include <stdlib.h>

#include <string>

namespace maudit {

enum class color : uint32_t {
    none = 0,

        dim_black,
        dim_red,
        dim_green,
        dim_yellow,
        dim_blue,
        dim_magenta,
        dim_cyan,
        dim_white,

        bright_black,
        bright_red,
        bright_green,
        bright_yellow,
        bright_blue,
        bright_magenta,
        bright_cyan,
        bright_white
};

struct glyph {
    std::string text;
    color fore;
    color back;

    glyph() : text(" "), fore(color::none), back(color::bright_black) {}

    glyph(const std::string& c, color f, color b) : text(c),  fore(f), back(b) {}
};


enum class keycode : uint32_t {
    none = 0,

        left,
        right,
        up,
        down,
        esc
};


struct keypress {
    unsigned char letter;
    keycode key;

    keypress() : letter(0), key(keycode::none) {}
};


#define CSI "\033["

template <typename IO>
struct screen {
    
    IO& io;

    size_t w;
    size_t h;

    screen(IO& _io) : io(_io), w(80), h(25) {

        // Turn off echo in the client.
        io.write("\xFF\xFB\x01"); 

        // Turn off line buffering in the client.
        io.write("\xFF\xFB\x03");
    }

    void get_size(size_t& _w, size_t& _h) {
        
        //io.write(CSI "18t");

        // Request terminal size.
        io.write("\xFF\xFD\x1F");

        keypress nothing;
        wait_key(nothing, true);

        _w = w;
        _h = h;

        return;

        /*
        char* tmp = ::getenv("LINES");
        //std::cout << "{{" << tmp << std::endl;
        if (tmp != NULL) {
            int t = ::atoi(tmp);
            if (t > 0) 
                h = t;
        }

        tmp = ::getenv("COLUMNS");
        if (tmp != NULL) {
            int t = ::atoi(tmp);
            if (t > 0)
                w = t;
        }
        */
    }

    template <typename FUNC>
    bool refresh(FUNC f) {

        size_t w;
        size_t h;

        //FILE* ff = fopen("/home/itkachev/diggr-roguelike-test/libmaudit/qqpp", "a");
        //fprintf(ff, "---------------------------\n");
        //fclose(ff);
        
        get_size(w, h);

        std::string data;

        data += CSI "2J";      // Erase screen.
        data += CSI "1;1H";    // Move cursor to home position.

        color fore_prev = color::none;
        color back_prev = color::none;

        for (size_t y = 0; y < h; ++y) {
            for (size_t x = 0; x < w; ++x) {

                glyph g = f(x, y, w, h);

                bool do_fore = false;
                bool do_back = false;

                if (x == 0 && y == 0) {
                    do_fore = true;
                    do_back = true;

                } else if (fore_prev != g.fore) {
                    do_fore = true;

                }  else if (back_prev != g.back) {
                    do_back = true;
                }
                
                if (do_fore || do_back) {

                    data += CSI;

                    if (do_fore) {

                        switch (g.fore) {
                        case color::none:        data += "39"; break;
                        case color::dim_black:   data += "30"; break;
                        case color::dim_red:     data += "31"; break;
                        case color::dim_green:   data += "32"; break;
                        case color::dim_yellow:  data += "33"; break;
                        case color::dim_blue:    data += "34"; break;
                        case color::dim_magenta: data += "35"; break;
                        case color::dim_cyan:    data += "36"; break;
                        case color::dim_white:   data += "37"; break;
                        case color::bright_black:   data += "1;30"; break;
                        case color::bright_red:     data += "1;31"; break;
                        case color::bright_green:   data += "1;32"; break;
                        case color::bright_yellow:  data += "1;33"; break;
                        case color::bright_blue:    data += "1;34"; break;
                        case color::bright_magenta: data += "1;35"; break;
                        case color::bright_cyan:    data += "1;36"; break;
                        case color::bright_white:   data += "1;37"; break;
                        }
                    }

                    if (do_back) {

                        if (do_fore) data += ";";

                        switch (g.back) {
                        case color::none:        data += "49"; break;
                        case color::dim_black:   data += "40"; break;
                        case color::dim_red:     data += "41"; break;
                        case color::dim_green:   data += "42"; break;
                        case color::dim_yellow:  data += "43"; break;
                        case color::dim_blue:    data += "44"; break;
                        case color::dim_magenta: data += "45"; break;
                        case color::dim_cyan:    data += "46"; break;
                        case color::dim_white:   data += "47"; break;
                        case color::bright_black:   data += "40"; break;
                        case color::bright_red:     data += "41"; break;
                        case color::bright_green:   data += "42"; break;
                        case color::bright_yellow:  data += "43"; break;
                        case color::bright_blue:    data += "44"; break;
                        case color::bright_magenta: data += "45"; break;
                        case color::bright_cyan:    data += "46"; break;
                        case color::bright_white:   data += "47"; break;
                        }
                    }
                    
                    data += 'm';
                }

                data += g.text;

                fore_prev = g.fore;
                back_prev = g.back;
            }
        }

        return io.write(data);
    }


    bool wait_key(keypress& out, bool system_event=false) {

      again:

        unsigned char c;

        bool ok = io.read(c);
        if (!ok) return false;

        // Handle telnet escape codes.

        if (c == 0xFF) {

            ok = io.read(c);
            if (!ok) return false;

            switch (c) {

            case 0xFF:
                // Escape 0xFF
                out.letter = 0xFF;
                return true;

            case 0xFB:
            case 0xFC:
            case 0xFD:
            case 0xFE:
                // Will/won't/do/don't silliness, ignore it.
                ok = io.read(c);
                if (!ok) return false;
                goto again;

            case 0xFA:
                ok = io.read(c);
                if (!ok) return false;

                if (c != 0x1F)
                    goto again;

                // 0xFA 0x1F: response to screen size request.

                ok = io.read(c);
                if (!ok) return false;

                w = ((size_t)c << 8);

                ok = io.read(c);
                if (!ok) return false;

                w |= (size_t)c;
    
                ok = io.read(c);
                if (!ok) return false;

                h = ((size_t)c << 8);

                ok = io.read(c);
                if (!ok) return false;

                h |= (size_t)c;
                
                if (system_event) return true;
                goto again;

            default:
                goto again;
            }
        }

        // Handle ANSI/vt100 terminal escapes.

        if (c == '\033') {

            ok = io.read(c);
            if (!ok) return false;

            if (c == '[') {
                // Try for some special characters.

                ok = io.read(c);
                if (!ok) return false;

                switch (c) {
                case 'D':  out.key = keycode::left;  break;
                case 'A':  out.key = keycode::up;    break;
                case 'C':  out.key = keycode::right; break;
                case 'B':  out.key = keycode::down;  break;
                }

            } else {
                out.key = keycode::esc;
            }

            return true;
        }

        out.letter = c;
        return true;
    }

};


}

#endif
