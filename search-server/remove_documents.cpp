#include "remove_duplicates.h"

using namespace std;

void RemoveDuplicates(SearchServer& search_server) {
    set<int> duplicates;
    set<set<string>> unique;
    for (const int document_id : search_server) {
        map<string, double> words_and_frec_in_document = search_server.GetWordFrequencies(document_id);
        set<string> set_of_unique_words_in_document;
		for (auto [word, frec] : words_and_frec_in_document) {
            set_of_unique_words_in_document.insert(word);
        }
        if (unique.find(set_of_unique_words_in_document) == unique.end()) {
            unique.insert(set_of_unique_words_in_document);
        } else {
            duplicates.insert(document_id);
        }
    }
    for (int duplicat : duplicates) {
        search_server.RemoveDocument(duplicat);
        cout << "Found duplicate document id "s << duplicat << endl;
    }
}
