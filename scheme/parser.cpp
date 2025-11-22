#include "error.h"
#include "object.h"
#include "parser.h"
#include "tokenizer.h"

#include <stdexcept>
#include <memory>
#include <variant>
#include <vector>

bool IsBrace(const Token& t, BracketToken exp) {
    return std::holds_alternative<BracketToken>(t) && std::get<BracketToken>(t) == exp;
}

std::shared_ptr<Object> ReadObject(Tokenizer* tokenizer);
std::shared_ptr<Object> ReadList(Tokenizer* tokenizer) {
    Token current = tokenizer->GetToken();
    if (!IsBrace(current, BracketToken::OPEN)) {
        throw RuntimeError{"expected ( in the beggining of ReadList"};
    }
    tokenizer->Next();

    std::vector<std::shared_ptr<Object>> tokens;
    bool improper = false;
    while (true) {
        if (tokenizer->IsEnd()) {
            throw SyntaxError{"unpaired ("};
        }
        if (IsBrace(tokenizer->GetToken(), BracketToken::CLOSE)) {
            break;
        }
        if (std::holds_alternative<DotToken>(tokenizer->GetToken())) {
            if (tokens.empty()) {
                throw SyntaxError{"pair requeries first element"};
            }
            tokenizer->Next();
            if (tokenizer->IsEnd()) {
                throw SyntaxError{"unpaired ("};
            }
            tokens.push_back(ReadObject(tokenizer));
            improper = true;
            break;
        }
        tokens.push_back(ReadObject(tokenizer));
    }

    if (tokenizer->IsEnd()) {
        throw SyntaxError{"unpaired ("};
    }
    if (!IsBrace(tokenizer->GetToken(), BracketToken::CLOSE)) {
        throw SyntaxError{"expected ) in the end of ReadList"};
    }
    tokenizer->Next();

    if (tokens.empty()) {
        return nullptr;
    }
    if (tokens.size() == 1) {
        return tokens[0];
    }
    std::shared_ptr<Cell> root = std::make_shared<Cell>(nullptr, nullptr);
    std::shared_ptr<Cell> curr = root;
    for (int i = 0; i < tokens.size(); ++i) {
        curr->SetFirst(tokens[i]);
        if (i == tokens.size() - 2 && improper) {
            curr->SetSecond(tokens.back());
            break;
        }
        if (i != tokens.size() - 1) {
            auto nxt = std::make_shared<Cell>(nullptr, nullptr);
            curr->SetSecond(nxt);
            curr = nxt;
        }
    }
    return root;
}

std::shared_ptr<Object> ReadObject(Tokenizer* tokenizer) {
    Token current = tokenizer->GetToken();
    if (tokenizer->IsEnd()) {
        throw SyntaxError{"Read in ended tokenizer"};
    }

    if (const auto* p = std::get_if<ConstantToken>(&current)) {
        tokenizer->Next();
        return std::make_shared<Number>(p->value);
    }
    if (const auto* p = std::get_if<BracketToken>(&current)) {
        if (*p == BracketToken::CLOSE) {
            throw SyntaxError{"unpaired )"};
        }
        return ReadList(tokenizer);
    }
    if (const auto* p = std::get_if<SymbolToken>(&current)) {
        tokenizer->Next();
        return std::make_shared<Symbol>(p->name);
    }
    if (const auto* p = std::get_if<QuoteToken>(&current)) {
        tokenizer->Next();
        return std::make_shared<Quote>("UNKNOWN QUOTE");
    }
    throw SyntaxError{"unexpected token for read"};
}

std::shared_ptr<Object> Read(Tokenizer* tokenizer) {
    auto res = ReadObject(tokenizer);
    if (!tokenizer->IsEnd()) {
        throw SyntaxError{"unread tokens are left"};
    }
    return res;
}
