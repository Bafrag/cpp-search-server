// -------- Начало модульных тестов поисковой системы ----------

// Тест проверяет, что поисковая система исключает стоп-слова при добавлении документов
void TestExcludeStopWordsFromAddedDocumentContent() {
    const int doc_id = 42;
    const string content = "cat in the city"s;
    const vector<int> ratings = {1, 2, 3};
    {
        SearchServer server;
        server.AddDocument(doc_id, content, DocumentStatus::ACTUAL, ratings);
        const auto found_docs = server.FindTopDocuments("in"s);
        ASSERT_EQUAL(found_docs.size(), 1u);
        const Document& doc0 = found_docs[0];
        ASSERT_EQUAL(doc0.id, doc_id);
    }

    {
        SearchServer server;
        server.SetStopWords("in the"s);
        server.AddDocument(doc_id, content, DocumentStatus::ACTUAL, ratings);
        ASSERT_HINT(server.FindTopDocuments("in"s).empty(), "Stop words must be excluded from documents"s);
    }
}

void TestMinusWordsCheck(){
    const int doc_id = 41;
    const string content = "dog in the city"s;
    const vector<int> ratings = {1, 2, 3};
    {
        SearchServer server;
        server.SetStopWords("in the"s);
        server.AddDocument(doc_id, content, DocumentStatus::ACTUAL, ratings);
        ASSERT(server.FindTopDocuments("dog -city"s).empty());
    }
}

void TestMatchDocuments(){
    const int doc_id = 0;
    const string content = "dog in the city"s;
    const vector<int> ratings = {1, 2, 3};
    const int doc_id1 = 1;
    const string content1 = "cat in the city"s;
    const vector<int> ratings1 = {5, 1, 3};
    {//one is minus
        SearchServer server;
        server.SetStopWords("in the"s);
        server.AddDocument(doc_id, content, DocumentStatus::ACTUAL, ratings);
        server.AddDocument(doc_id1, content1, DocumentStatus::ACTUAL, ratings1);
        vector<Document> documents = server.FindTopDocuments("-dog city"s);
        int q=1;
        double w = 0.0;
        int e=3;
        ASSERT_EQUAL(documents[0].id, q);
        ASSERT(documents[0].relevance-w <= 1e-6);
        ASSERT_EQUAL(documents[0].rating, e);
    }
    {//no document
        SearchServer server;
        server.SetStopWords("in the"s);
        server.AddDocument(doc_id, content, DocumentStatus::ACTUAL, ratings);
        //server.AddDocument(doc_id1, content1, DocumentStatus::ACTUAL, ratings1);
        vector<Document> documents = server.FindTopDocuments("-dog"s);
        ASSERT(documents.empty());
    }
}
void TestPredicant(){
    const int doc_id = 0;
    const string content = "dog in the city"s;
    const vector<int> ratings = {1, 2, 3};
    {
        SearchServer server;
        server.SetStopWords("in the"s);
        server.AddDocument(doc_id, content, DocumentStatus::IRRELEVANT, ratings);
        vector<Document> documents = server.FindTopDocuments("dog city"s, [](int document_id, DocumentStatus status, int rating)
                                                             { return status == DocumentStatus::IRRELEVANT;});
        ASSERT_EQUAL(documents.size(), 1);
        ASSERT_EQUAL(documents[0].id, doc_id);
    }
}
void TestStatus(){
    const int doc_id = 0;
    const string content = "dog in the city"s;
    const vector<int> ratings = {1, 2, 3};
    const int doc_id1 = 1;
    const string content1 = "cat in the city p"s;
    const vector<int> ratings1 = {5, 1, 3};
    const int doc_id2 = 2;
    const string content2 = "nr city efu ewu"s;
    const vector<int> ratings2 = {6, 8, 2};
    {
        SearchServer server;
        server.SetStopWords("in the"s);
        server.AddDocument(doc_id, content, DocumentStatus::ACTUAL, ratings);
        server.AddDocument(doc_id1, content1, DocumentStatus::BANNED, ratings1);
        server.AddDocument(doc_id2, content2, DocumentStatus::ACTUAL, ratings2);
        vector<Document> documents = server.FindTopDocuments("dog city"s);
        ASSERT_EQUAL(documents.size(), 2);
        ASSERT_EQUAL(documents[0].id, doc_id);
        ASSERT_EQUAL(documents[1].id, doc_id2);
    }
}
void TestRelevantnost(){
    const int doc_id0 = 0;
    const string content0 = "dog in the ui vg"s;
    const vector<int> ratings0 = {1, 2, 3};
    const int doc_id1 = 1;
    const string content1 = "cat p the city"s;
    const vector<int> ratings1 = {1, 2, 3};
    {
        SearchServer server;
        server.SetStopWords("in the"s);
        server.AddDocument(doc_id0, content0,   DocumentStatus::ACTUAL, ratings0);
        server.AddDocument(doc_id1, content1,   DocumentStatus::ACTUAL, ratings1);
        //server.AddDocument(2, "mko in the ufd vu ouyfr"s, DocumentStatus::ACTUAL, {3});
        double relevance0 = 0.462098;
        double relevance1 = 0.231049;
        //double relevance2 = 0.0675775;
        vector<Document> documents = server.FindTopDocuments("dog in the cat vg"s);
        Document document0 = documents[0];
        Document document1 = documents[1];
        ASSERT_EQUAL(documents.size(), 2);
        ASSERT(document0.relevance-relevance0 <= 1e-6);
        ASSERT(document1.relevance-relevance1 <= 1e-6);
    }
}
// Функция TestSearchServer является точкой входа для запуска тестов
void TestSearchServer() {
    TestExcludeStopWordsFromAddedDocumentContent();//добавление документа и проверка стоп слов
    TestMinusWordsCheck();//поддержка минус слов
    TestMatchDocuments();//Матчинг документов
    TestPredicant();//Предикант
    TestStatus();//Статус
    TestRelevantnost();//Релевантность
    // Не забудьте вызывать остальные тесты здесь
}

// --------- Окончание модульных тестов поисковой системы -----------

int main() {
    TestSearchServer();
    // Если вы видите эту строку, значит все тесты прошли успешно
    cout << "Search server testing finished"s << endl;
}
