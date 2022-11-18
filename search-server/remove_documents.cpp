#include "remove_duplicates.h"

using namespace std;

void RemoveDuplicates(SearchServer& search_server) {
    set<set<string>> non_duplicates;
    set<int> duplicates;
    for (const int document_id : search_server) {
        if (non_duplicates.count(search_server.GetIdDocumentsWords().at(document_id))) {
            duplicates.insert(document_id);
        } else {
            non_duplicates.insert(search_server.GetIdDocumentsWords().at(document_id));
        }
    }
    for (int duplicat : duplicates) {
        search_server.RemoveDocument(duplicat);
        cout << "Found duplicate document id "s << duplicat << endl;
    }
}
