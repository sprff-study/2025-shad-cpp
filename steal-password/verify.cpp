#include "verify.h"
#include <cstdlib>

namespace {
const int kSecureCode = 12345;
const int kPassword = 6789;
}  // namespace

size_t RateLimitCheck();

int GetPassword(int secure_code) {
    int password = kPassword;
    return RateLimitCheck() && secure_code == kSecureCode ? password : std::rand();
}

bool DoSecretStuff(int password) {
    return RateLimitCheck() && password == kPassword;
}
