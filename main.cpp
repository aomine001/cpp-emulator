#include <iostream>

// Registers
uint8_t A = 0;
uint8_t B = 0;

int PC = -1;

int SP = 0; // stack pointer

int TOP = 0; // stack top
int BOTTOM = 0; // stack bottom

int HALTED = false;
int ERRORS = 0;

bool EQUAL = false;
bool N_EQUAL = false;
bool BIGGER = false;
bool SMALLER = false;

// Comparisons:
/*
    EQUAL - A == B
    N_EQUAL - A != B
    BIGGER - A > B
    SMALLER - B > A
*/

// Instructions:
#define NOP 0x00; // nop
#define LDA 0x01; // load to A
#define LDB 0x02; // load to B
#define JMP 0x03; // jump to memory. using 16 bit value
#define INA 0x04; // increment register A
#define INB 0x05; // increment register B
#define DEA 0x06; // decrement register A
#define DEB 0x07; // decrement register B
#define CMP 0x08; // compare register A and B
#define JE 0x09;  // jump if equal
#define JNE 0x0a; // jump if not equal
#define JB 0x0b;  // jump if a is bigger
#define JNB 0x0c; // jump if b is bigger
#define ADD 0x0d; // add register a and b and save it to register a
#define SUB 0x0e; // substract register a and b and save it to register a
#define MUL 0x0f; // multiply register a and b and save it to register a
#define DIV 0x10; // divide register a and register b and save it register a
#define SWP 0x11; // swap register A and register B
#define HLT 0x12; // halt emulation
#define PUSH = 0x13; // push register A to stack
#define POP = 0x14; // pop to register A from stack
#define SSP = 0x15; // set stack pointer
#define SST = 0x16; // set stack top (limit where incrementation will stop)
#define SSB = 0x17; // set stack bottom (limit where decrementation will stop)

// Program code
uint8_t Program[0xffff] = {
    // Stack Initialisation
    SSP, 0xCF, 0x00, // SP = 0xCF00
    SST, 0xFF, 0x00, // ST = 0xFF00
    SSB, 0xCE, 0x00, // SB = 0xCE00

    // Real Code
    LDA, 0xFF,       // MOV A, 0xFF
    PUSH,            // PUSH
    LDA, 0x00,       // MOV A, 0x00
    POP              // POP
};


uint8_t Fetch8() // Fetch first code.
{
    uint8_t Value = Program[PC];

    PC++;

    return Value;
}

uint16_t Fetch16() // Fetch two codes together.
{
    uint8_t Value  = (uint16_t) Fetch8();
    uint8_t Value2 = (uint16_t) Fetch8();

    return Value * 256 + Value2;
}

int GetProgramSize()
{
    return (sizeof(Program) / sizeof(uint8_t));
}

int main()
{
    uint8_t OpCode = Fetch8();

    while (HALTED != true && GetProgramSize() > PC)
    {
        uint8_t OpCode = Fetch8();

        // pre-initialisations
        uint8_t value;
        uint8_t value2;
        uint16_t address;

        switch (OpCode)
        {
        case NOP:
            break;
        case LDA:
            value = Fetch8();
            A = value;
            break;
        case LDB:
            value = Fetch8();
            B = value;
            break;
        case JMP:
            address = Fetch16();
            PC = address;
            break;
        case INA:
            A++; break;
        case INB:
            B++; break;
        case DEA:
            A--; break;
        case DEB:
            B--; break;
        case CMP:
            // Comparisons:
            /*
                EQUAL - A == B
                N_EQUAL - A != B
                BIGGER - A > B
                SMALLER - B > A
            */
            if (A == B) EQUAL = true;
            if (A != B) N_EQUAL = true;
            if (A > B) BIGGER = true;
            if (B > A) SMALLER = true;
            else ERRORS++;

            break;
        case JE:
            address = Fetch16();

            if (EQUAL == true) PC = address;

            break;
        case JNE:
            address = Fetch16();

            if (N_EQUAL == true) PC = address;

            break;
        case JB:
            address = Fetch16();

            if (BIGGER == true) PC = address;

            break;
        case JNB:
            address = Fetch16();

            if (SMALLER == true) PC = address;

            break;
        case ADD:
            A = A + B; break;
        case SUB:
            A = A - B; break;
        case MUL:
            A = A * B; break;
        case DIV:
            A = A / B; break;
        case SWP:
            A, B = B, A;
        case HLT:
            HALTED = true;
        case SSP:
            SP = Fetch16(); break;
        case PUSH:
            if (TOP > SP)
                Program[SP] = A; SP++; break;
        case POP:
            if (BOTTOM < SP)
                A = Program[SP]; SP--; break;
        case SST:
            TOP = Fetch16(); break;
        case SSB:
            BOTTOM = Fetch16(); break;
        default:
            ERRORS++; break;
        }
    }

    std::cout << "Error count: " << std::to_string(ERRORS) << std::endl;
    std::cout << "Register A: " << std::to_string(A) << " Register B:" << std::to_string(B) << std::endl;
    std::cout << "End Address: " << std::to_string(PC) << std::endl;
}
