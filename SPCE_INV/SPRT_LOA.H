#ifndef SPRT_LOA_H_INCLUDED
#define SPRT_LOA_H_INCLUDED

struct sprite{
    char value[13][9];    
};

extern struct sprite sprites[11];
void load_sprite();

#endif