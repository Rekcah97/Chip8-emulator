#include <iostream>
#include "src/hardware.h"
#include "src/platform.h"
using namespace std;

int main(int argc, char** argv){
    cout << "Chip-8 Emulator" << endl;

    if(argc < 2){
        cerr << "Chips 8 rom not found " << endl ;
        return 1;
    }

    chip8 emu;
    if(!emu.loadRom(argv[1])){
        cerr<< "failed to load Chip8 rom" << endl ;
        return 1;
    }

    platform emuDisplay("Chip-8",32*10,64*10,32,64);

    bool running = true;

    while(running){
        running = emuDisplay.processInput(emu.input);

        for(int i=0;i<10;i++) emu.cycle();
            emu.updateTimers();

            emuDisplay.update(emu.screen);
        
    }
    return 0;
}