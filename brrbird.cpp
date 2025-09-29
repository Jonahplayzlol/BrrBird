#include <iostream>
#include <fstream>
#include <vector>
#include <string>

int main() {
    std::string romPath, brrPath;
    unsigned int offsetHex; // store hex offset as an unsigned int

    std::cout << "Enter path to SMW ROM: ";
    std::getline(std::cin, romPath);

    std::cout << "Enter path to BRR file: ";
    std::getline(std::cin, brrPath);

    std::cout << "Enter ROM offset in hex (example 123456, no 0x needed): ";
    std::cin >> std::hex >> offsetHex;

    // Read BRR file into memory
    std::ifstream brrFile(brrPath, std::ios::binary);
    if (!brrFile.is_open()) {
        std::cerr << "Error: could not open BRR file.\n";
        return 1;
    }
    std::vector<char> brrData((std::istreambuf_iterator<char>(brrFile)),
                               std::istreambuf_iterator<char>());
    brrFile.close();

    // Open ROM for read/write
    std::fstream romFile(romPath, std::ios::in | std::ios::out | std::ios::binary);
    if (!romFile.is_open()) {
        std::cerr << "Error: could not open ROM file.\n";
        return 1;
    }

    // Backup ROM if no .bak exists
    std::ifstream backupCheck(romPath + ".bak", std::ios::binary);
    if (!backupCheck.is_open()) {
        std::ifstream src(romPath, std::ios::binary);
        std::ofstream dst(romPath + ".bak", std::ios::binary);
        dst << src.rdbuf();
        std::cout << "Backup created: " << romPath << ".bak\n";
    }

    // Move write pointer to offset
    romFile.seekp(static_cast<std::streamoff>(offsetHex), std::ios::beg);
    if (!romFile.good()) {
        std::cerr << "Error: failed to seek to offset.\n";
        return 1;
    }

    // Write BRR data
    romFile.write(brrData.data(), brrData.size());
    if (!romFile.good()) {
        std::cerr << "Error: failed to write BRR data.\n";
        return 1;
    }

    romFile.close();
    std::cout << "✅ BRR file successfully injected at 0x" 
              << std::hex << offsetHex << "\n";

    return 0;
}
