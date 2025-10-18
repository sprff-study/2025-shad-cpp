#pragma once

#include <stdexcept>

template <class Iterator>
Iterator LocalMax(Iterator first, Iterator last) {
    Iterator cur = first;
    while (cur != last) {
        auto nxt = cur;
        ++nxt;
        bool l = (cur == first);
        bool r = (nxt == last);
        if (!l) {
            auto prv = cur;
            prv--;
            if (*prv < *cur) {
                l = true;
            }
        }
        if (!r) {
            if (*nxt < *cur) {
                r = true;
            }
        }
        if (l && r) {
            return cur;
        }
        ++cur;
    }
    return last;

    throw std::runtime_error{"Not implemented"};
}
