#include "json.hpp"
#include <algorithm>
#include <cctype>
#include <iostream>
#include <set>
#include <sstream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

using namespace std;
using json = nlohmann::json;

class SearchEngine {
public:
  void indexDocument(const string &index_name, const string &text) {
    vector<string> tokens = tokenize(text);
    int loc = 0;
    for (const auto &token : tokens) {
      freq_loc[token][index_name].push_back(loc);
      loc++;
    }
  }

  void matchQuery(const string &query) {
    vector<string> tokens = tokenize(query);

    for (const auto &token : tokens) {
      if (freq_loc.find(token) != freq_loc.end()) { // token is present
        cout << "Token: " << token << endl;
        for (auto p : freq_loc[token]) {
          cout << "Document: " << p.first << endl;
          cout << "Location in Doument: ";
          for (auto elem : p.second) {
            cout << elem << ", ";
          }
          cout << endl;
        }
        cout << endl;
      }
    }
  }

private:
  map<string, map<string, vector<int>>> freq_loc;

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
      cout << "Enter doc name: ";
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
      search_engine.matchQuery(query);
    } else {
      cout << "Unknown command. Please enter 'index', 'search', or 'quit'."
           << endl;
    }
  }
  return 0;
}
