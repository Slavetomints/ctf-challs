#include <iostream>
#include <fstream>
#include <vector>
#include <thread>

int main() {
    std::cout << "Hi! Welcome back to the Bank of Mints" << std::endl;
    std::cout << "We have this cool newsletter to show you, but only members can view it" << std::endl;
    std::cout << "All members got a key, what is your key?" << std::endl;
    // 98 114 130
    unsigned int k1, k2, k3;

    std::cout << "Enter key (3 integers): ";
    std::cin >> k1 >> k2 >> k3;

    unsigned char key[3] = {
        static_cast<unsigned char>(k1),
        static_cast<unsigned char>(k2),
        static_cast<unsigned char>(k3)
    };

    std::ifstream file("bee-movie.txt", std::ios::binary);
    if (!file) {
        std::cerr << "Failed to open file\n";
        return 1;
    }

    std::vector<unsigned char> data(
        (std::istreambuf_iterator<char>(file)),
        std::istreambuf_iterator<char>()
    );

    file.close();

    for (size_t i = 0; i < data.size(); i++) {
        data[i] ^= key[i % 3];
    }

    size_t i = 0;
    while (i < data.size()) {
        std::cout << data[i] << std::flush;

        std::this_thread::sleep_for(
            std::chrono::milliseconds(100) // 1/10 second
        );
        i++;
    }

    std::cout << "\n";
    return 0;
}