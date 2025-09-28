#include <iostream>
#include <fstream>
#include <vector>

int main() {
    std::string romPath, brrPath;
    std::streampos offset;

    std::cout << "Enter path to SMW ROM: ";
    std::getline(std::cin, romPath);

    std::cout << "Enter path to BRR file: ";
    std::getline(std::cin, brrPath);

    std::cout << "Enter ROM offset (hex): 0x";
    std::cin >> std::hex >> offset;

    // Read BRR file into memory
    std::ifstream brrFile(brrPath, std::ios::binary);
    if (!brrFile) {
        std::cerr << "Failed to open BRR file.\n";
        return 1;
    }
    std::vector<char> brrData((std::istreambuf_iterator<char>(brrFile)),
                               std::istreambuf_iterator<char>());
    brrFile.close();

    // Open ROM file
    std::fstream romFile(romPath, std::ios::in | std::ios::out | std::ios::binary);
    if (!romFile) {
        std::cerr << "Failed to open ROM file.\n";
        return 1;
    }

    // Optional: make backup
    std::ifstream romBackupCheck(romPath + ".bak", std::ios::binary);
    if (!romBackupCheck) {
        std::ofstream romBackup(romPath + ".bak", std::ios::binary);
        romBackup << romFile.rdbuf();
        romBackup.close();
        std::cout << "Backup created: " << romPath << ".bak\n";
    }

    // Seek to the offset
    romFile.seekp(offset);
    if (!romFile) {
        std::cerr << "Failed to seek to offset.\n";
        return 1;
    }

    // Write BRR data
    romFile.write(brrData.data(), brrData.size());
    if (!romFile) {
        std::cerr << "Failed to write BRR data.\n";
        return 1;
    }

    romFile.close();
    std::cout << "BRR file successfully injected into ROM.\n";

    return 0;
}
