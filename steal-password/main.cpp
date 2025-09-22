#include "steal.h"
#include "verify.h"
#include <iostream>

int main() {
    int password = StealPassword();
    if (DoSecretStuff(password)) {
        std::cout << "welcome to the secret place...\n";
        return 0;
    } else {
        std::cout << "try again\n";
        return 1;
    }
}
