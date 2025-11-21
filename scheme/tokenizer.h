#pragma once

#include <cstdio>
#include <variant>
#include <istream>
#include <string>

struct SymbolToken {
    std::string name;

    bool operator==(const SymbolToken& other) const=default;
};

struct QuoteToken {
    bool operator==(const QuoteToken&) const=default;
};

struct DotToken {
    bool operator==(const DotToken&) const=default;
};

enum class BracketToken { OPEN, CLOSE };

struct ConstantToken {
    int value;

    bool operator==(const ConstantToken& other) const=default;
};

using Token = std::variant<ConstantToken, BracketToken, SymbolToken, QuoteToken, DotToken>;

// Интерфейс, позволяющий читать токены по одному из потока.
class Tokenizer {
public:
    Tokenizer(std::istream* in) : reader_(in), finished_(false) {
        Next();
    }

    bool IsEnd() {
        return finished_;
    }

    void Next() {
        while (reader_->peek() == ' ' || reader_->peek() == '\n' ) {
            reader_->get();
        }

        auto nxt = reader_->peek();
        if (nxt == EOF) {
            finished_ = true;
            return;
        }

        if (nxt == '(') {
            reader_->get();
            current_ = BracketToken::OPEN;
            return;
        } else if (nxt == ')') {
            reader_->get();
            current_ = BracketToken::CLOSE;
            return;
        } else if (nxt == '.') {
            reader_->get();
            current_ = DotToken{};
            return;
        } else if (nxt == '\'') {
            reader_->get();
            current_ = QuoteToken{};
            return;
        } else if (nxt == '/') {
            reader_->get();
            current_ = SymbolToken{"/"};
            return;
        } else if (IsSymbolStart(nxt)) {
            std::string name;
            while (IsSymbol(reader_->peek())) {
                name += reader_->get();
            }
            current_ = SymbolToken{name};
            return;
        } else if (IsDigit(nxt)) {
            current_ = ReadDigit();
            return;
        } else if (nxt == '+') {
            reader_->get();
            if (!IsDigit(reader_->peek())) {
                current_ = SymbolToken{"+"};
                return;
            }
            current_ = ReadDigit();
            return;
        } else if (nxt == '-') {
            reader_->get();
            if (!IsDigit(reader_->peek())) {
                current_ = SymbolToken{"-"};
                return;
            }
            current_ = ReadDigit(true);
            return;
        }
    }

    Token GetToken() {
        return current_;
    }

private:
    std::istream* reader_;
    bool finished_;
    Token current_;

    bool IsDigit(int nxt) {
        return ('0' <= nxt && nxt <= '9');
    }
    bool IsSymbolStart(int nxt) {
        return ('a' <= nxt && nxt <= 'z') || ('A' <= nxt && nxt <= 'Z') || nxt == '<' ||
               nxt == '=' || nxt == '>' || nxt == '*' || nxt == '#';
    }
    bool IsSymbol(int nxt) {
        return IsSymbolStart(nxt) || IsDigit(nxt) || nxt == '?' || nxt == '!' || nxt == '-';
    }
    Token ReadDigit(bool negative = false) {
        int res = 0;
        while (IsDigit(reader_->peek())) {
            res *= 10;
            res += reader_->get() - '0';
        }
        if (negative ) {
            res = -res;
        }
        return ConstantToken{res};
    }
};
