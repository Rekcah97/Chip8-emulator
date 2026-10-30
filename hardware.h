#include <cstdint>
#include <stdint.h>
#include <cstring>
#include <iostream>

using namespace std;

#define RAM_SIZE 4096
#define STARTING_ADDRESS 0x200





class chip8{
    private:

    uint8_t RAM[RAM_SIZE]; // Ram
    uint8_t v[16];  // General purpose register
    uint8_t SP; // stack pointer


    uint8_t delay_timer;
    uint8_t sound_timer;


    uint16_t stack[16]; // Stack
    uint16_t PC; // Program counter
    uint16_t I; // Index Register

    uint16_t opcode; //opcode

    public:
        bool drawScreen; 
        bool input[16]; // 16 Input Keys
        uint8_t screen[ 64 * 32]; // 64x32 Monochrome Display Memory


        bool loadRom(const char* romfile);
        void loadInput();
        uint8_t randomNumberGenerator();

        chip8(){ 
            PC = STARTING_ADDRESS;
            srand(time(NULL));
            loadInput();
        }

        // Instructions
        void CLS_00E0();
        void RET_00EE();
        void JMP_1nnn();
        void CALL_2nnn();
        void SE_3xkk();
        void SNE_4xkk();
        void SE_5xy0();
        void LD_6xkk();
        void ADD_7xkk();
        void LD_8xy0();
        void OR_8xy1();
        void AND_8xy2();
        void XOR_8xy3();
        void ADD_8xy4();
        void SUB_8xy5();
        void SHR_8xy6();
        void SUBN_8xy7();
        void SHL_8xyE();
        void SNE_9xy0();
        void LD_Annn();
        void JP_Bnnn();
        void RND_Cxkk();
        void DRW_Dxyn();
        void SKP_Ex9E();
        void SKNP_ExA1();
        void LD_Fx07();


};


