#include <iostream>
#include <fstream>
#include <vector>

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <input> <output>\n";
        return 1;
    }

    // fixed key
    unsigned char key[3] = {0x62, 0x72, 0x82};
    const size_t keyLen = 3;

    std::ifstream input(argv[1], std::ios::binary);
    if (!input) {
        std::cerr << "Failed to open input file\n";
        return 1;
    }

    std::vector<unsigned char> data(
        (std::istreambuf_iterator<char>(input)),
        std::istreambuf_iterator<char>()
    );

    input.close();

    // XOR encrypt
    for (size_t i = 0; i < data.size(); i++) {
        data[i] ^= key[i % keyLen];
    }

    std::ofstream output(argv[2], std::ios::binary);
    if (!output) {
        std::cerr << "Failed to open output file\n";
        return 1;
    }

    output.write(reinterpret_cast<char*>(data.data()), data.size());
    output.close();

    std::cout << "Done encrypting.\n";
    return 0;
}