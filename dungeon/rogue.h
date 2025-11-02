#pragma once

#include "dungeon.h"
#include "unordered_set"
#include "unordered_map"
#include <stdexcept>

const Room* FindFinalRoom(const Room* starting_room) {
    std::unordered_set<const Room*> unvisited;
    std::unordered_set<const Room*> visited;
    std::unordered_set<Door*> locked;
    std::unordered_set<std::string> keys;

    unvisited.insert(starting_room);
    while (unvisited.size() + locked.size() > 0) {
        if (unvisited.empty()) {
            for (auto to : locked) {
                for (auto key : keys) {
                    if (to->TryOpen(key)) {
                        unvisited.insert(to->GoThrough());
                        break;
                    }
                }
            }
        }
        if (unvisited.empty()) {
            return nullptr;
        }
        const Room* v = *unvisited.begin();
        if (v->IsFinal()) {
            return v;
        }
        unvisited.erase(v);
        visited.insert(v);
        for (size_t i = 0; i < v->NumKeys(); ++i) {
            keys.insert(v->GetKey(i));
        }
        for (size_t i = 0; i < v->NumDoors(); ++i) {
            auto d = v->GetDoor(i);
            if (d->IsOpen()) {
                if (visited.count(d->GoThrough()) == 0) {
                    unvisited.insert(d->GoThrough());
                }
            } else {
                locked.insert(d);
            }
        }
    }
    return nullptr;
}
