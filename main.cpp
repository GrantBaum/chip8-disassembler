#include <iostream>
#include <string>
#include <stdexcept>
#include <format>
#include <vector>
#include <fstream>
#include <iterator>

//constants
static const uint16_t OP_MASK = 0xF000;
static const uint16_t X_MASK = 0x0F00;
static const uint16_t Y_MASK = 0x00F0;
static const uint16_t N_MASK = 0x000F;
static const uint16_t NN_MASK = 0x00FF;
static const uint16_t NNN_MASK = 0x0FFF;
static const uint16_t H1_MASK = 0xFF00;
static const uint16_t H2_MASK = 0x00FF;

//declarations
uint16_t pos = 0; //current position of reader
uint16_t pc = 0x200; //where it would be starting in memory in the chip

//helpers
std::string dataLine(uint16_t code){
    uint8_t second = static_cast<uint8_t>(code); //low byte
    uint8_t first = static_cast<uint8_t>(code >> 8); //high byte

    return std::format("DB 0x{:02X}, 0x{:02X}", first, second);
}
std::string dataLine(uint8_t byte){
    return std::format("DB 0x{:02X}", byte);
}
std::string disassemble(uint16_t code){
    //nibbles
    uint16_t op_nibble = (code & OP_MASK) >> 12;
    uint16_t x = (code & X_MASK) >> 8;
    uint16_t y = (code & Y_MASK) >> 4;
    uint16_t n = code & N_MASK;
    uint16_t nn = code & NN_MASK;
    uint16_t nnn = code & NNN_MASK;

    switch(op_nibble){
        case 0x0:
            if(nn == 0xEE){
                return "RET";
                break;
            }
            else if(nn == 0xE0){
                return "CLS";
                break;
            }
            else return dataLine(code);
            break;

        case 0x1: //1NNN JUMP
            return std::format("JP 0x{:03X}", nnn);
            break;

        case 0x2: //2NNN
            return std::format("CALL 0x{:03X}", nnn);
            break;

        case 0x3: //3XNN
            return std::format("SE V{:X}, 0x{:02X}", x, nn);
            break;

        case 0x4: //4XNN
            return std::format("SNE V{:X}, 0x{:02X}", x, nn);
            break;

        case 0x5: //5XY0
            return std::format("SE V{:X}, V{:X}", x, y);
            break;

        case 0x6: //6XNN
            return std::format("LD V{:X}, 0x{:02X}", x, nn);
            break;

        case 0x7: //7XNN
            return std::format("ADD V{:X}, 0x{:02X}", x, nn);
            break;

        case 0x8:
            if(n == 0){
                return std::format("LD V{:X}, V{:X}", x, y);
                break;
            }
            else if(n == 0x1){
                return std::format("OR V{:X}, V{:X}", x, y);
                break;
            }
            else if(n == 0x2){
                return std::format("AND V{:X}, V{:X}", x, y); 
                break;
            }
            else if(n == 0x3){
                return std::format("XOR V{:X}, V{:X}", x, y);
                break;
            }
            else if(n == 0x4){
                return std::format("ADD V{:X}, V{:X}", x, y);
                break;
            }
            else if(n == 0x5){
                return std::format("SUB V{:X}, V{:X}", x, y);
                break;
            }
            else if(n == 0x6){
                return std::format("SHR V{:X}", x); 
                break;
            }
            else if(n == 0x7){
                return std::format("SUBN V{:X}, V{:X}", x, y);
                break;
            }
            else if(n == 0xE){
                return std::format("SHL V{:X}", x);
                break;
            }
            else return dataLine(code);
            break;

        case 0x9: //9XY0
            return std::format("SNE V{:X}, V{:X}", x, y);
            break;

        case 0xA: //ANNN
            return std::format("LD I, 0x{:03X}", nnn);
            break;

        case 0xB: //BNNN
            return std::format("JP V0, 0x{:03X}", nnn);
            break;

        case 0xC: //CXNN
            return std::format("RND V{:X}, 0x{:02X}", x, nn);
            break;

        case 0xD: //DXYN
            return std::format("DRW V{:X}, V{:X}, 0x{:X}", x, y, n);
            break;

        case 0xE:
            if(nn == 0x9E){
                return std::format("SKP V{:X}", x);
                break;
            }
            else if(nn == 0xA1){
                return std::format("SKNP V{:X}", x);
                break;
            }
            else return dataLine(code);
            break;

        case 0xF:
            if(nn == 0x07){
                return std::format("LD V{:X}, DT", x);
                break;
            }
            else if(nn == 0x15){
                return std::format("LD DT, V{:X}", x);
                break;
            }
            else if(nn == 0x18){
                return std::format("LD ST, V{:X}", x);
                break;
            }
            else if(nn == 0x29){
                return std::format("LD F, V{:X}", x);
                break;
            }
            else if(nn == 0x33){
                return std::format("LD B, V{:X}", x);
                break;
            }
            else if(nn == 0x55){
                return std::format("LD [I], V{:X}", x);
                break;
            }
            else if(nn == 0x65){
                return std::format("LD V{:X}, [I]", x);
                break;
            }
            else if(nn == 0x0A){
                return std::format("LD V{:X}, K", x);
                break;
            }
            else if(nn == 0x1E){
                return std::format("ADD I, V{:X}", x);
                break;
            }
            else return dataLine(code);
            break;

        default:
            return dataLine(code);
            break;
    }
}

int main(int argc, char* argv[]){
    //opening infile stream
    std::ifstream file(argv[1], std::ios::binary);
    if(!file) {
        std::cerr << "Could not open " << argv[1] << "\n";
        return 1;
    }
    //setting vector with all the bytes that are in the program
    std::vector<uint8_t> bytes((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    
    for(size_t j = 0; j < bytes.size(); j+=2){
        uint16_t opcode = bytes.at(j) << 8 | bytes.at(j+1);

        std::cout << std::format("0x{:03X}: ", pc + j) + disassemble(opcode) << std::endl;
    }
    
}