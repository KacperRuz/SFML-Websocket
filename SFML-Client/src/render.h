#ifndef RENDER_H_INCLUDED
#define RENDER_H_INCLUDED

#include <SFML/Graphics.hpp>

namespace rdr{
bool init_sfml();
void keep_window();
int window_event_handling();
int rendering_loop();
int keyboard_event_handling();
int render_actors();
int render_debug();
}


#endif // RENDER_H_INCLUDED
