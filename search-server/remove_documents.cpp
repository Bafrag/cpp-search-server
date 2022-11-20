#include "remove_duplicates.h"

using namespace std;

void RemoveDuplicates(SearchServer& search_server) {
    set<int> duplicates;
    set<set<string>> unique;
    for (const int document_id : search_server) {
        map<string, double> test = search_server.GetWordFrequencies(document_id);
        set<string> h;
		for (auto [word, frec] : test) {
            h.insert(word);
        }
        if (unique.find(h) == unique.end()) {
            unique.insert(h);
        } else {
            duplicates.insert(document_id);
        }
    }
    for (int duplicat : duplicates) {
        search_server.RemoveDocument(duplicat);
        cout << "Found duplicate document id "s << duplicat << endl;
    }
}
