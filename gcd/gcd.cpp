#include <numeric>
#include <iostream>

int Gcd(int a, int b) {
    while (a * b != 0) {
        int rem = a % b;
        a = b;
        b = rem;
    }
    return std::max(a, b);
}

int TryRead() {
    if (int x; std::cin >> x) {
        return x;
    }
    std::cout << "Fail on reading\n";
    std::exit(0);
}

int main() {
    int a = TryRead();
    int b = TryRead();
    if ((a <= 0) || (b <= 0)) {
        std::cout << "Numbers should be positive\n";
        return 0;
    }

    int gcd1 = Gcd(a, b);
    int gcd2 = std::gcd(a, b);
    if (gcd1 != gcd2) {
        std::cout << "Shit happens\n";
        return 1;
    }
    std::cout << "Try again...\n";
}
