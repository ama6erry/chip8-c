#ifndef IO_H
#define IO_H

void init_display();
void draw_display(unsigned char* display);
void stop_display();
void event_handler();

extern int running;

#endif // IO_H
