#include <iostream>
#include <map>
#include <functional>

std::string LimitedGetline() {
    constexpr int MaxSize = 80;
    std::string line;
    while (std::cin && line.size() < MaxSize) {
        int symbol = std::cin.get();
        if (symbol == '\n') break;
        line.push_back(symbol);
    }
    return line;
}

class ExitError {};
class RootError : public ExitError {};
class LError : public RootError {};
class RError : public RootError {};

static int64_t StepsLeft = 6;
static int64_t LeafsVisited = 0;

void Throw() {
    std::map<std::string, std::function<void()>> errors = {
        {"throw Root", [] { throw RootError(); }},
        {"throw L", [] { throw LError(); }},
        {"throw R", [] { throw RError(); }},
    };

    auto errorName = LimitedGetline();
    auto it = errors.find(errorName);
    if (it != errors.end()) {
        it->second();
    }

    std::cerr << "Exiting" << std::endl;
    throw ExitError();
}

void LLFn() {
    static bool Visited = false;
    if (!Visited) {
        Visited = true;
        ++LeafsVisited;
    }
    Throw();
}

void LRFn() {
    static bool Visited = false;
    if (!Visited) {
        Visited = true;
        ++LeafsVisited;
    }
    Throw();
}

void RLFn() {
    static bool Visited = false;
    if (!Visited) {
        Visited = true;
        ++LeafsVisited;
    }
    Throw();
}

void RRFn() {
    static bool Visited = false;
    if (!Visited) {
        Visited = true;
        ++LeafsVisited;
    }
    Throw();
}

void LFn() {
    while (true) {
        --StepsLeft;
        try {
            if (LimitedGetline() == "L") {
                LLFn();
            } else {
                LRFn();
            }
        } catch(const LError&) { }
    }
}

void RFn() {
    while (true) {
        --StepsLeft;
        try {
            if (LimitedGetline() == "L") {
                RLFn();
            } else {
                RRFn();
            }
        } catch(const RError&) { }
    }
}

void RootFn() {
    while (true) {
        --StepsLeft;
        try {
            if (LimitedGetline() == "L") {
                LFn();
            } else {
                RFn();
            }
        } catch(const RootError&) { }
    }
}

int main() {
    try {
        RootFn();
    } catch(const ExitError& ) {
        std::cerr << StepsLeft << " " << LeafsVisited << std::endl;
        if (StepsLeft >= 0 && LeafsVisited == 4) {
            std::cerr << "Bingo" << std::endl;
            return 1;
        }
    }

    return 0;
}
