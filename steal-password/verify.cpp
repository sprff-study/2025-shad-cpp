#include "verify.h"
#include <cstdlib>

namespace {
constexpr int kPassword = PPASSWORD;
constexpr int kSecureCode = PSECURECODE;

size_t RateLimitCheck() {
    static size_t wild_guess_limit = 3;
    wild_guess_limit = wild_guess_limit ? --wild_guess_limit : 0uz;
    return wild_guess_limit;
}
}  // namespace

int GetPassword(int secure_code) {
    int password = kPassword;
    return RateLimitCheck() && secure_code == kSecureCode ? password : std::rand();
}

bool DoSecretStuff(int password) {
    return RateLimitCheck() && password == kPassword;
}
