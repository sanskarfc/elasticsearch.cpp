#include "json.hpp"
#include <algorithm>
#include <cctype>
#include <iostream>
#include <set>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;
using json = nlohmann::json;

class SearchEngine {
public:
  void indexDocument(const string &index_name, const string &text) {
    vector<string> tokens = tokenize(text);
    for (const auto &token : tokens) {
      inverted_index_[token].push_back(index_name);
    }
  }

  vector<string> matchQuery(const string &query) {
    vector<string> tokens = tokenize(query);
    set<string> matching_indices;
    for (const auto &token : tokens) {
      if (inverted_index_.find(token) != inverted_index_.end()) {
        for (const auto &index : inverted_index_[token]) {
          matching_indices.insert(index);
        }
      }
    }
    return vector<string>(matching_indices.begin(), matching_indices.end());
  }

private:
  unordered_map<string, vector<string>> inverted_index_;

  vector<string> tokenize(const string &text) {
    vector<string> tokens;
    istringstream iss(text);
    string token;
    // whitespace punctuations and convert to lowercase
    while (iss >> token) {
      transform(token.begin(), token.end(), token.begin(),
                [](unsigned char c) { return tolower(c); });
      token.erase(remove_if(token.begin(), token.end(), ::ispunct),
                  token.end());
      tokens.push_back(token);
    }
    return tokens;
  }
};

int main() {
  SearchEngine search_engine;
  cout << "Enter 'index' to add a document, 'search' to perform a match query, "
          "or 'quit' to exit."
       << endl;
  string command;
  while (true) {
    cout << "\nEnter command: ";
    cin >> command;
    if (command == "quit") {
      break;
    } else if (command == "index") {
      string index_name, document_text;
      cout << "Enter index name: ";
      cin >> index_name;
      cin.ignore();
      cout << "Enter document text: ";
      getline(cin, document_text);
      search_engine.indexDocument(index_name, document_text);
      cout << "Document indexed successfully!" << endl;
    } else if (command == "search") {
      string query;
      cout << "Enter search query: ";
      cin.ignore();
      getline(cin, query);
      vector<string> match_results = search_engine.matchQuery(query);
      cout << "Match results:" << endl;
      for (const auto &index : match_results) {
        cout << index << endl;
      }
    } else {
      cout << "Unknown command. Please enter 'index', 'search', or 'quit'."
           << endl;
    }
  }
  return 0;
}
