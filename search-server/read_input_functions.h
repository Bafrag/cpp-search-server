#pragma once
#include "document.h"
#include "paginator.h"

std::string ReadLine();

int ReadLineWithNumber();

template <typename Iterator>
std::ostream& operator<<(std::ostream& out, const IteratorRange<Iterator>& range) {
    using namespace std::literals;
    for (Iterator it = range.begin(); it != range.end(); ++it) {
        out << *it;
    }
    return out;
}
