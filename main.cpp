#include <iostream>
#include <limits>
#include <fstream>
#include <filesystem>
#include <windows.h>

using namespace std;

int main() {
    string alphabet = "abcdefghijklmnopqrstuvwxyz0123456789-{}()";
    string password;
    string plaintext;
    string ciphertext;
    string outName;
    char passChar;
    cout << "please, choose 1 for encryption process otherwise for decryption: ";
    if (cin.peek() == '1') {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "please, enter input file name: ";
        getline(cin, plaintext);
        ifstream input(plaintext + ".txt");
        stringstream buffer;
        buffer << input.rdbuf();
        plaintext = buffer.str();
        input.close();
        cout << "enter output file name: ";
        getline(cin, outName);
        cout << "enter a password: ";
        cin >> password;
        for (char plainTxtChar : plaintext) {
            if (isalpha(plainTxtChar) || isdigit(plainTxtChar)) {
                short alphaPostion = alphabet.find(tolower(plainTxtChar));
                if (alphaPostion != string::npos) {
                    int password_limits = alphaPostion % password.size();
                    passChar = password[password_limits];
                    ciphertext += passChar;
                    if (password_limits != 0 &&
                        (password.substr(password_limits)).find(passChar) != string::npos) {
                        if (isdigit(ciphertext[ciphertext.size() - 1])) ciphertext += ',';
                        ciphertext += to_string(password_limits) + '#';
                    }
                    ciphertext += to_string(alphaPostion - password_limits) + '$';
                }
            } else ciphertext += plainTxtChar;
        }
        if (ciphertext[ciphertext.size() - 1] != '$')
            ciphertext += '$';
        ofstream output(outName + ".txt");
        output << ciphertext;
        output.close();
        filesystem::path filePath = filesystem::current_path() / (outName + ".txt");
        ShellExecute(nullptr, "open", filePath.string().c_str(), nullptr, nullptr, SW_SHOWNORMAL);

    } else {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "please, enter input file name: ";
        getline(cin, ciphertext);
        cout << "enter output file name: ";
        getline(cin, outName);
        ifstream input(ciphertext + ".txt");
        stringstream buffer;
        buffer << input.rdbuf();
        ciphertext = buffer.str();
        input.close();
        cout << "enter password: ";
        cin >> password;
        for (int i = 0; i < ciphertext.size(); i++) {
            if (ciphertext[i] == ' ') {
                plaintext += ciphertext[i];
                continue;
            }
            int passwordPosition = password.find(ciphertext[i]);
            if (passwordPosition != string::npos) {
                string num;
                if (ciphertext[i + 1] == ',')
                    i++;
                int repeated = 0;
                if (isdigit(ciphertext[i + 1])) {
                    while (ciphertext[++i] != '$') {
                        if (ciphertext[i] == '#') {
                            repeated = stoi(num);
                            num.clear();
                        } else { num += ciphertext[i]; }
                    }
                } else continue;
                int number = num.empty() ? 0 : stoi(num);
                if (repeated != 0)passwordPosition = 0;
                if (isupper(ciphertext[i]))
                    passChar = toupper(alphabet[number + passwordPosition + repeated]);
                else passChar = alphabet[number + passwordPosition + repeated];
                plaintext += passChar;
            }
        }
        ofstream output(outName + ".txt");
        output << plaintext;
        output.close();
        filesystem::path pathFile = filesystem::current_path() / (outName + ".txt");
        cout << pathFile;
        ShellExecute(nullptr, "open", pathFile.string().c_str(), nullptr, nullptr, SW_SHOWNORMAL);
    }

    return 0;
}
