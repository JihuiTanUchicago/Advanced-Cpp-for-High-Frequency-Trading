#pragma once

namespace generic_algorithms {
    template<typename InputIterator, typename Predicate>
    InputIterator find_if(InputIterator first, InputIterator last, Predicate pred) {
        for (; first != last; ++first) {
            if (pred(*first)) return first;
        }
        return last;
    }
}