#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <map>
#include "document.h"
#include "paginator.h"

std::string ReadLine();

int ReadLineWithNumber();

void PrintDocument(const Document& document);

void PrintMatchDocumentResult(int document_id, const std::vector<std::string>& words, DocumentStatus status);

template <typename Iterator>
std::ostream& operator<<(std::ostream& out, const IteratorRange<Iterator>& range) {
    using namespace std::literals;
    for (Iterator it = range.begin(); it != range.end(); ++it) {
        out << *it;
    }
    return out;
}

std::ostream& operator<<(std::ostream& out, const Document& document);