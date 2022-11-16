#include <cctype>
#include <cstddef>
#include <iostream>
#include <stdio.h>
#include <string>
#include <chrono>
#include <ctime>

#include "Text.hpp"

int main() {
    Text UI;
    std::string text;// = "testing testing, are you there. I dont think so;";
    std::string werds[] = {"death", "which", "dog", "through", "ur mum", "how", "then", "when", "cat", "broom", "phone", "under", "top", "before", "same", "great", "because", "plan", "but"};
    int index = 0;
    int aindex = 0;
    double wpm;
    double accuracy;
    int correct_characters = 0;
    int correct_words = 0;
    bool mistake;
    int line = 1;
    char c;
    UI.maxLines = 3;

    
    srand(time(0));
    
    for (int x = 0; x < 20; x++) {
        text.append(werds[rand() % 15] + " ");
    }

    //printf("\n%s\r", text.c_str());
    //UI.printText(text, index, 0, true);
    UI.printLines(text, index, line, UI.maxLines, 0);


    // Set the terminal to raw mode
    system("stty raw -echo");

    auto start = std::chrono::high_resolution_clock::now();
    while(1) {
        c = getchar();

        if (c == text[index]) { //  Correct character pressed
            if (c == 32) {
                if (!mistake) { correct_words++; }
                mistake = false;
            }
            if (index >= aindex) { correct_characters++; }
            UI.correctChar(index, text);

        } else if (c == 32) { //  Skip word if spacebar is pressed
            while (text[index] != 32) {
                UI.invalidChar(index, text);
            }
            UI.invalidChar(index, text);
            UI.failedWord(index, text);
            
        } else if (c == 127) { //  Backspace if not at beginning of text (shit gets weird otherwise)
            if (!(index > 0)) { continue; }
            if (UI.overflows[index] > 0) { 
                UI.wordOverflowDelete(index, line, c, text);
               } else {
                if (!(index < aindex)) { aindex = index; } //  Dont count correctly typed characters before the furthest delete towards accuracy
                UI.deleteChar(index, text);
            }

        } else if (c == 3) { //  ctrl-c exits
            system("stty cooked");
            exit(0);

        } else { //  Incorrect character pressed
            if (text[index] == 32) { //  Overflow characters
                UI.wordOverflow(index, line, c, text);
            } else {
                UI.invalidChar(index, text);
                mistake = true;
            }
        }

        if (index == UI.edges[line] + 1) {
            printf("\r\n\x1b[10C");
            line++;
        }

        if (index >= text.length() - 1) {
            if (index == -1 || index == -2) { continue; }
            break;
        }
    }
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(stop - start);

    wpm = double(correct_words) / (duration.count() / 100.0);
    accuracy = (double(correct_characters) / double(text.length())) * 100;


    system("stty cooked");
    printf("\x1b[0m\nwpm: %f  accuracy: %f%%\n", wpm, accuracy);

}
