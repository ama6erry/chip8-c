#ifndef IO_H
#define IO_H

void init_display();
void draw_display(unsigned char display[64][32]);
void stop_display();
void event_handler();

extern int running;
extern int draw_flag;
extern int display_scale;

#endif // IO_H
