// -------- Начало модульных тестов поисковой системы ----------

// Тест проверяет, что поисковая система исключает стоп-слова при добавлении документов
void TestAddDocuments(){
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
}
void TestExcludeStopWords() {
    const int doc_id = 42;
    const string content = "cat in the city"s;
    const vector<int> ratings = {1, 2, 3};
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
    //нет минус слов
    {
        SearchServer server;
        server.SetStopWords("in the"s);
        server.AddDocument(doc_id, content, DocumentStatus::ACTUAL, ratings);
        ASSERT_EQUAL(server.FindTopDocuments("dog city"s).size(), 1);
    }
}
void TestMatchDocument(){
    const int doc_id = 0;
    const string content = "dog in the city p k ds p m"s;
    const vector<int> ratings = {1, 2, 3};
    const int doc_id1 = 1;
    const string content1 = "cat in the city kiy w d t"s;
    const vector<int> ratings1 = {5, 1, 3};
    {
        SearchServer server;
        server.SetStopWords("in the"s);
        server.AddDocument(doc_id, content, DocumentStatus::ACTUAL, ratings);
        server.AddDocument(doc_id1, content1, DocumentStatus::ACTUAL, ratings1);
        vector<string> matched_words_test={"cat"s, "city"s, "w"s};
        string raw_matched_words_query="cat city p w k m"s;
        tuple<vector<string>, DocumentStatus> documents_answer=server.MatchDocument(raw_matched_words_query, doc_id1);
        vector<string> test_vector_string   = get<0>(documents_answer);
        bool kostil = (get<1>(documents_answer)==DocumentStatus::ACTUAL);
        ASSERT_EQUAL(test_vector_string, matched_words_test);
        ASSERT(kostil);
    }
}
void TestUsePredicant(){
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
void TestRating(){
    const int doc_id0 = 0;
    const string content0 = "dog in the city"s;
    const vector<int> ratings0 = {1, 2, 3};
    const int doc_id1 = 1;
    const string content1 = "cat in the city p"s;
    const vector<int> ratings1 = {};
    const int doc_id2 = 2;
    const string content2 = "nr city efu ewu"s;
    const vector<int> ratings2 = {6, 8, 2};
    {
        SearchServer server;
        server.SetStopWords("in the"s);
        server.AddDocument(doc_id0, content0, DocumentStatus::ACTUAL, ratings0);
        server.AddDocument(doc_id1, content1, DocumentStatus::ACTUAL, ratings1);
        server.AddDocument(doc_id2, content2, DocumentStatus::ACTUAL, ratings2);
        vector<Document> documents = server.FindTopDocuments("dog city"s);
        ASSERT_EQUAL(documents.size(), 3);
        //--------------------------------------
        int rating_test0=0;
        if (ratings0.size()>0){
            for (int f:ratings0){
                rating_test0+=f;
            }
            rating_test0/=ratings0.size();
        }
        int rating_test1=0;
        if (ratings1.size()>0){
            for (int f:ratings1){
                rating_test1+=f;
            }
            rating_test1/=ratings1.size();
        }
        int rating_test2=0;
        if (ratings2.size()>0){
            for (int f:ratings2){
                rating_test2+=f;
            }
            rating_test2/=ratings2.size();
        }
        //--------------------------------------
        ASSERT_EQUAL(documents[0].rating, rating_test0);
        ASSERT_EQUAL(documents[1].rating, rating_test2);
        ASSERT_EQUAL(documents[2].rating, rating_test1);
    }
}
void TestStatus(){
    const int doc_id0 = 0;
    const string content0 = "dog in the city"s;
    const vector<int> ratings0 = {1, 2, 3};
    const int doc_id1 = 1;
    const string content1 = "cat in the city p"s;
    const vector<int> ratings1 = {5, 1, 3};
    const int doc_id2 = 2;
    const string content2 = "nr city efu ewu"s;
    const vector<int> ratings2 = {6, 8, 2};
    {
        SearchServer server;
        server.SetStopWords("in the"s);
        server.AddDocument(doc_id0, content0, DocumentStatus::IRRELEVANT, ratings0);
        server.AddDocument(doc_id1, content1, DocumentStatus::BANNED,     ratings1);
        server.AddDocument(doc_id2, content2, DocumentStatus::ACTUAL,     ratings2);
        vector<Document> documents = server.FindTopDocuments("dog city"s, DocumentStatus::IRRELEVANT);
        ASSERT_EQUAL(documents.size(), 1);
        ASSERT_EQUAL(documents[0].id, doc_id0);//только один документ с таким статусом
    }
    {
        SearchServer server;
        server.SetStopWords("in the"s);
        server.AddDocument(doc_id0, content0, DocumentStatus::IRRELEVANT, ratings0);
        server.AddDocument(doc_id1, content1, DocumentStatus::BANNED,     ratings1);
        server.AddDocument(doc_id2, content2, DocumentStatus::ACTUAL,     ratings2);
        vector<Document> documents = server.FindTopDocuments("dog city"s, DocumentStatus::REMOVED);
        ASSERT_EQUAL(documents.size(), 0);//нет документа с таким статусом
    }
}
void TestRelevantnost(){
    const int doc_id0 = 0;
    const string content0 = "dog in the ui vg"s;
    const vector<int> ratings0 = {1, 2, 3};
    const int doc_id1 = 1;
    const string content1 = "cat p the city"s;
    const vector<int> ratings1 = {1, 2, 3};
    string query="dog in the cat vg"s;
    {
        SearchServer server;
        server.SetStopWords("in the"s);
        server.AddDocument(doc_id0, content0,   DocumentStatus::ACTUAL, ratings0);
        server.AddDocument(doc_id1, content1,   DocumentStatus::ACTUAL, ratings1);
        //
        const vector<string> no_stop_content0={"dog"s, "ui"s, "vg"s};
        const vector<string> no_stop_content1={"cat"s, "p"s, "city"s};
        double relevance_dog=(1.0/no_stop_content0.size())*log(2.0/1.0);
        double relevance_cat=(1.0/no_stop_content1.size())*log(2.0/1.0);
        double relevance_vg =(1.0/no_stop_content0.size())*log(2.0/1.0);
        //
        double relevance0 = relevance_dog+relevance_vg;
        double relevance1 = relevance_cat;
        vector<Document> documents = server.FindTopDocuments("dog in the cat vg"s);
        ASSERT_EQUAL(documents.size(), 2);
        ASSERT(documents[0].relevance-relevance0 <= 1e-6);
        ASSERT(documents[1].relevance-relevance1 <= 1e-6);
    }
}
// Функция TestSearchServer является точкой входа для запуска тестов
void TestSearchServer() {
    TestAddDocuments();//добавление документа и проверка стоп слов
    TestExcludeStopWords();
    TestMinusWordsCheck();//поддержка минус слов
    TestMatchDocument();//Матчинг документов
    TestUsePredicant();//Предикант
    TestStatus();//Статус
    TestRelevantnost();//Релевантность
    TestRating();//Рейтинг
    // Не забудьте вызывать остальные тесты здесь
}

// --------- Окончание модульных тестов поисковой системы -----------

int main() {
    TestSearchServer();
    // Если вы видите эту строку, значит все тесты прошли успешно
    cout << "Search server testing finished"s << endl;
}
