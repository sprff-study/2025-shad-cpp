#include "verify.h"
#include <cstdlib>

namespace {
constexpr int kPassword = PPASSWORD;
constexpr int kSecureCode = PSECURECODE;
}  // namespace

int GetPassword(int secure_code) {
    int password = kPassword;
    return (secure_code == kSecureCode) ? password : std::rand();
}

bool DoSecretStuff(int password) {
    return password == kPassword;
}
