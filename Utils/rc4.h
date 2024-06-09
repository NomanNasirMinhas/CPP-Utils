#include <iostream>
#include <fstream>
#include <vector>
#include <string>

class RC4 {
public:
    RC4(const std::string& key) {
        KSA(key);
    }

    std::string encrypt(const std::string& data) {
        return PRGA(data);
    }

    std::string decrypt(const std::string& data) {
        return PRGA(data); // RC4 encryption and decryption are symmetric
    }

    void encryptFile(const std::string& inputFilePath, const std::string& outputFilePath) {
        std::string data = readFile(inputFilePath);
        std::string encryptedData = PRGA(data);
        writeFile(outputFilePath, encryptedData);
    }

    void decryptFile(const std::string& inputFilePath, const std::string& outputFilePath) {
        std::string data = readFile(inputFilePath);
        std::string decryptedData = PRGA(data);
        writeFile(outputFilePath, decryptedData);
    }

private:
    std::vector<int> S;
    int i = 0;
    int j = 0;

    void KSA(const std::string& key) {
        S.resize(256);
        for (int i = 0; i < 256; ++i) {
            S[i] = i;
        }

        int j = 0;
        for (int i = 0; i < 256; ++i) {
            j = (j + S[i] + key[i % key.size()]) % 256;
            std::swap(S[i], S[j]);
        }
    }

    std::string PRGA(const std::string& data) {
        std::string result;
        result.resize(data.size());

        for (size_t k = 0; k < data.size(); ++k) {
            i = (i + 1) % 256;
            j = (j + S[i]) % 256;
            std::swap(S[i], S[j]);
            int rnd = S[(S[i] + S[j]) % 256];
            result[k] = data[k] ^ rnd;
        }

        return result;
    }

    std::string readFile(const std::string& filePath) {
        std::ifstream file(filePath, std::ios::binary);
        if (!file) {
            throw std::runtime_error("Could not open file " + filePath);
        }
        std::string data((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        return data;
    }

    void writeFile(const std::string& filePath, const std::string& data) {
        std::ofstream file(filePath, std::ios::binary);
        if (!file) {
            throw std::runtime_error("Could not open file " + filePath);
        }
        file.write(data.c_str(), data.size());
    }
};