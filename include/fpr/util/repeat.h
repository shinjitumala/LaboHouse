#pragma once

#include <functional>

namespace fpr {
using namespace std;
/// Execute 'action', 'count' times without the danger of accidentally touching
/// the index unlike for-loops.
/// @param count
/// @param action
void
repeat(uint count, function<void()> action)
{
    for (auto i{ 0 }; i < count; i++) {
        action();
    }
}
};