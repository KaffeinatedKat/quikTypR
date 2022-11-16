#pragma once
#include <string>
#include <map>
#include <sys/ioctl.h> //ioctl() and TIOCGWINSZ
#include <unistd.h> // for STDOUT_FILENO

class Text {
    public:
        std::map<int, int> overflows;
        std::map<int, int> edges;
        int maxLines;

    public: void printLines(std::string text, int index, int line, int count, int extra) {
        int end = ((screenSize() - 20) * line) - extra;
        bool buff = true;
        if (line == maxLines) { count += 1; }
        if (extra > 0) { buff = false; }

        for (int x = 1; x < count + 1; x++) {
            if (end > text.length()) {
                end = text.length();
            }
            while (text[end] != 32) {
                end--;
            }
            edges[x] = end;
            if (buff == true) { printf("%*c", 10, ' '); }
            printf("%s\r\x1b[10C\n", text.substr(index, end - index).c_str());
            end++;
            index = end;
            end += (screenSize() - 20);
        }
        printf("\r\x1b[%dA\x1b[10C", count);
    }

    public: int screenSize() {
        winsize win;
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &win);
        return win.ws_col;
    }

    public: void invalidChar(int &index, std::string text) {
        printf("\x1b[1;4m\x1b[31m%c", text[index]);
        index++;
    }

    public: void correctChar(int &index, std::string text) {
        printf("\x1b[0m\x1b[1;32m%c", text[index]);
        index++;
    }

    public: void deleteChar(int &index, std::string text) {
        index--;
        printf("\x1b[0m\x1b[D%c\x1b[D", text[index]);
    }

    public: void wordOverflow(int &index, int line, char c, std::string text) {
        overflows[index] += 1;
        printf("\x1b[0m\x1b[2m\x1b[1;31m%c\x1b[s\x1b[0J\x1b[0m", c);
        printLines(text, index, line, (3 - line) + 1, overflows[index]);
        printf("\x1b[u");
        
    }

    public: void wordOverflowDelete(int &index, int line, char c, std::string text) {
        printf("\x1b[D\x1b[0m\x1b[s\x1b[0J");
        printLines(text, index, line, (3 - line) + 1, overflows[index]);
        printf("\x1b[u");
        overflows[index] -= 1;
    }

    public: void failedWord(int index, std::string text) {
        int start = index;
        printf("\x1b[0m");
        do {
            index--;
            printf("\x1b[D");
        } while (text[index] != 32);
        printf("\x1b[1;31m\x1b[1;4m%s\x1b[0m ", text.substr(index, (start - index) - 1).c_str());
    }
};
