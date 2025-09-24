#pragma once

#include <string>
#include <stdexcept>
#include <map>
#include <iostream>
#include <vector>

std::map<char, std::vector<char>> buttons = {
    {'2', {'a', 'b', 'c', '2'}}, {'3', {'d', 'e', 'f', '3'}},      {'4', {'g', 'h', 'i', '4'}},
    {'5', {'j', 'k', 'l', '5'}}, {'6', {'m', 'n', 'o', '6'}},      {'7', {'p', 'q', 'r', 's', '7'}},
    {'8', {'t', 'u', 'v', '8'}}, {'9', {'w', 'x', 'y', 'z', '9'}}, {'0', {' ', '0'}},
    {'*', {'.', ',', '?', '!'}},
};

std::string NumberDecode(char c, int x, int& upper) {
    if (c == 0 || c == '.') {
        return "";
    }
    if (c == '1') {
        return std::string(x, '1');
    }
    x -= 1;
    x %= buttons[c].size();
    char res = buttons[c][x];
    if (upper == 1 && 'a' < res && res < 'z') {
        res = res - 'a' + 'A';
    }
    if (upper == 1) {
        upper = 0;
    }
    return std::string(1, res);
}

std::string Decode(const std::string& input) {
    char last = 0;
    int cnt = 0;
    int n = input.size();
    std::string res;
    int upper = 0;
    for (int i = 0; i < n; ++i) {
        if (input[i] == '#') {
            res += NumberDecode(last, cnt, upper);
            upper = upper ^ 1;
            last = 0;
            continue;
        }
        if (input[i] != last) {
            res += NumberDecode(last, cnt, upper);
            last = input[i];
            cnt = 1;
            continue;
        }
        cnt += 1;
    }
    res += NumberDecode(last, cnt, upper);
    return res;
}
