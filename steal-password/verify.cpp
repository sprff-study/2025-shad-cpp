#include "verify.h"
#include <cstdlib>
#include <ctime>

namespace {
const int kSecureCode = (std::srand(std::time({})), std::rand());
const int kPassword = std::rand();
}  // namespace

size_t RateLimitCheck();

int GetPassword(int secure_code) {
    int password = kPassword;
    return RateLimitCheck() && secure_code == kSecureCode ? password : std::rand();
}

bool DoSecretStuff(int password) {
    return RateLimitCheck() && password == kPassword;
}
