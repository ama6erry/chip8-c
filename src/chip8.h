#ifndef CHIP8_H
#define CHIP8_H

extern unsigned char V[16];
extern unsigned short I; //I register - Used to store memory addresses
extern unsigned char dt; //Delay timer - When not 0, decrement at a rate of 60Hz
extern unsigned char st; //Sound timer - When not 0, decrement at a rate of 60Hz

//Declaring pseudo registers
extern unsigned short pc; //Program counter - Stores the address of the next instruction to be executed
extern unsigned char sp; //Stack pointer - points to the top of the stack

//Other
extern unsigned short stack[16];
extern unsigned char memory[4096];
extern unsigned char display[64*32];

// Font set
extern unsigned char fontset[80];

int load_rom(char* name);
void run_cycle();
void debuglog();

#endif // !CHIP8_H

