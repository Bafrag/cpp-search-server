#include "remove_duplicates.h"

using namespace std;

void RemoveDuplicates(SearchServer& search_server) {
    set<int> duplicates;
    for (const int document_id : search_server) {
        if (!duplicates.count(document_id)) {
            auto test_content = search_server.GetDocumentsWordsWrequencies().at(document_id);
            for (const auto& [id, content]: search_server.GetDocumentsWordsWrequencies()) {
                if (content == test_content && id != document_id) {
                    duplicates.insert(id);
                }
            }
        }
        
    }
    for (int duplicat: duplicates) {
        search_server.RemoveDocument(duplicat);
        cout << "Found duplicate document id "s << duplicat << endl;
    }
}