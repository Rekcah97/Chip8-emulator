#include <fstream>
#include "hardware.h"

#define InputStartingAddress 0x050




bool chip8::loadRom(const char* romfile){

    // opening the rom file in binary mode and moving the pointer to end of the file as soon as file opens.
    ifstream file(romfile, ios::binary | ios::ate);

    if(!file) return false;

    //Checking the current location of input (get) pointer which we moved to end so we get size of the file.
    streamsize size = file.tellg();
    char* buffer = new char[size]; // making a block of memory


    file.seekg(0, ios::beg); // moving the get pointer to beggining 
    file.read(buffer,size); // reads the rom file to our buffer
    file.close();


    // this loop moves the rom into the our virtual RAM
    for(int i=0; i<size;i++){
        RAM[STARTING_ADDRESS + i] = buffer[i];
    }

    delete[] buffer;

    return true;

}


unsigned char fontSprite[80] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};


void chip8::loadInput(){
    for(int i = 0; i < 80 ; i++ ){
        RAM[InputStartingAddress + i] = fontSprite[i];
    }
}

uint8_t chip8::randomNumberGenerator(){

    return rand() % 256 ;
}

// INSTRUCTIONS
void chip8::CLS_00E0(){
    memset(screen, 0, sizeof(screen));
}

void chip8::RET_00EE(){
    PC = stack[SP];
    SP--;
}

void chip8::JMP_1nnn(){
    uint16_t address = opcode & 0x0FFF;
    PC = address;
}

void chip8::CALL_2nnn(){
    SP++;
    stack[SP] = PC ; 
    uint16_t address = opcode & 0x0FFF;
    PC = address;
}

void chip8::SE_3xkk(){
     int x = (opcode & 0x0F00) >> 8;
     uint8_t kk = opcode & 0x00FF;
     if(v[x] == kk) {
        PC += 2;
     }

}

void chip8::SNE_4xkk(){
      int x = (opcode & 0x0F00) >> 8;
     uint8_t kk = opcode & 0x00FF;
     if(v[x] != kk) {
        PC += 2;
     }
}

void chip8::SE_5xy0(){
    int x = (opcode & 0x0F00) >> 8;
    int y = (opcode & 0x00F0) >> 4;

    if (v[x]==v[y]){
        PC += 2;
    }

}
void chip8::LD_6xkk(){
    int x = (opcode & 0x0F00) >> 8;
    uint8_t kk = opcode & 0x00FF;

    v[x] = kk ;
}

void chip8::ADD_7xkk(){
    int x = (opcode & 0x0F00) >> 8;
    uint8_t kk = opcode & 0x00FF;

    v[x] += kk;
}

void chip8::LD_8xy0(){
    int x = (opcode & 0x0F00) >> 8;
    int y = (opcode & 0x00F0) >> 4;

    v[x] = v[y];
}

void chip8::OR_8xy1(){
    int x = (opcode & 0x0F00) >> 8;
    int y = (opcode & 0x00F0) >> 4;

    v[x] = v[x] | v[y];
}

void chip8::AND_8xy2(){
    int x = (opcode & 0x0F00) >> 8;
    int y = (opcode & 0x00F0) >> 4;
    
    v[x] = v[x] & v[y];

} 

void chip8::XOR_8xy3(){
    int x = (opcode & 0x0F00) >> 8;
    int y = (opcode & 0x00F0) >> 4;

    v[x] = v[x] ^ v[y];
}

void chip8::ADD_8xy4(){
    int x = (opcode & 0x0F00) >> 8;
    int y = (opcode & 0x00F0) >> 4;

    uint16_t adder = v[x] + v[y];

    v[x] = adder & 0x00FF;
    v[0x0F] = (adder > 0x00FF) ? 1 : 0;

}

void chip8::SUB_8xy5(){
    int x = (opcode & 0x0F00) >> 8;
    int y = (opcode & 0x00F0) >> 4;

    v[0x0F] = (v[x] > v[y] ) ? 1 : 0; 

    v[x] -= v[y];


}

void chip8::SHR_8xy6(){
    int x = (opcode & 0x0F00) >> 8;

    int LSB = v[x] & 0x01;

    v[0x0F] = (LSB == 1) ? 1 : 0; 
    v[x] /= 2;

}

void chip8::SUBN_8xy7(){
    int x = (opcode & 0x0F00) >> 8;
    int y = (opcode & 0x00F0) >> 4;

    v[0x0F] = (v[y] > v[x] ) ? 1 : 0; 

    v[x] = v[y] - v[x];

}

void chip8::SHL_8xyE(){
   int x = (opcode & 0x0F00) >> 8;

   int MSB = v[x] & 0x80;

   v[0x0F] = (MSB == 0x80) ? 1 : 0;
   v[x] *= 2;

}

void chip8::SNE_9xy0(){
    int x = (opcode & 0x0F00) >> 8;
    int y = (opcode & 0x00F0) >> 4;

    if(v[x] != v[y]){
        PC += 2;
    }
}

void chip8::LD_Annn(){
    uint16_t nnn = opcode & 0x0FFF;
    I = nnn;
}

void chip8::JP_Bnnn(){
    uint16_t nnn = opcode & 0x0FFF;
    PC = nnn + v[0x00];
}

void chip8::RND_Cxkk(){
    int x = (opcode & 0x0F00) >> 8 ;
    uint8_t kk = opcode & 0x00FF;
    uint8_t random = randomNumberGenerator();

    v[x] = random & kk ;

}

void chip8::DRW_Dxyn(){
    uint8_t n = opcode & 0x000F;
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t y = (opcode & 0x00F0) >> 4;

    uint8_t cordX = v[x];
    uint8_t cordY = v[y];

    v[0x0F] = 0; 
    
    for (int row = 0 ; row < n ; row++){

        uint8_t spriteByte = RAM[I + row];
        uint8_t screenY = (cordY + row) % 32 ; 

        for (int col = 0; col < 8; col++){
            uint8_t screenX = (cordX + col) % 64;
            uint8_t sprite = (spriteByte >> (7 - col)) & 0x01;
            uint8_t* pixel = &screen[screenY * 64 + screenX];
            if(sprite){
                if(*pixel){ //if both sprite and pixel are one
                    v[0x0F] = 1;
                }

                *pixel ^= sprite;
            }
        }
        

    }
    drawScreen = 1;
}

void chip8::SKP_Ex9E(){

    uint8_t x = (opcode & 0x0F00) >> 8;
    
    uint8_t key = v[x];

    if(input[key]){
        PC += 2;
    }

}

void chip8::SKNP_ExA1(){
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t key = v[x];

    if(!input[key]){
        PC += 2;
    }
}