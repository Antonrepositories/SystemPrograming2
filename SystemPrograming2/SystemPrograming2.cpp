#include <iostream>
#include<fstream>
#include<set>
#include<sstream>
#include<map>
#include<vector>
#include<string>
#include <regex>
#include <algorithm>

using namespace std;
struct Transition {
    int s;
    char a;
    int sn;
};

class FiniteAutomata {
public:
    FiniteAutomata(const string& filename);

    vector<string> findWords();
private:
    string characters = "abcdefghijklmnopqrstuvwxyz";
    //char eps = 'e';
    int alphabetSize;
    int statesAmount;
    vector<int> states;
    int startState;
    int finalStatesAmount;
    set<int> finalStates;
    vector<char> alphabet;
    map<pair<int, char>, int> transitions;
    vector<Transition> transitins;
    void Result(int currentState, string word, vector<string>&foundWords);
};
FiniteAutomata::FiniteAutomata(const string& filename) {
    ifstream file(filename);
    string line;

    getline(file, line);
    alphabetSize = stoi(line);
    cout << "||A|| = " << alphabetSize << endl;
    cout << "Alphabet: ";
    for (int i = 0; i < alphabetSize; i++) {
        alphabet.push_back(characters[i]);
        printf("%c ", alphabet[i]);
    }

    getline(file, line);
    statesAmount = stoi(line);

    getline(file, line);
    startState = stoi(line);

    getline(file, line);
    vector<int>numbers;

    stringstream ss;
    ss << line;
    int number;
    while (!ss.eof()) {
        ss >> number;
        numbers.push_back(number);
    }
    finalStatesAmount = numbers[0];
    cout << "\nFinal states: ";
    for (int i = 1; i < numbers.size(); i++) {
        finalStates.insert(numbers[i]);
        cout << numbers[i] << " ";
    }
    cout << endl;
    cout << "s" << " " << "a" << " " << "s`" << endl;
    while (getline(file, line)) {
        int s, sn;
        char a;
        istringstream iss(line);
        iss >> s >> a >> sn;
        Transition transition{ s, a, sn };
        transitins.push_back(transition);
        cout << s << " " << a << " " << sn << endl;
        transitions[make_pair(s, a)] = sn;
    }

}
vector<string> FiniteAutomata::findWords() {
    vector<string> foundWords;
    Result(startState, "", foundWords);
    return foundWords;
}
void FiniteAutomata::Result(int currentState, string word, vector<string>&foundWords) {
    if (finalStates.count(currentState) > 0) {
        foundWords.push_back(word);
    }
    vector<Transition> depTransitions;
    for (int i = 0; i < transitins.size(); i++) {
        if (transitins[i].s == currentState) {
            depTransitions.push_back(transitins[i]);
        }
    }
    int visited_itself_once = 0;
    for (int i = 0; i < depTransitions.size(); i++) {
        if (word.find(depTransitions[i].a) == string::npos) {
            Result(depTransitions[i].sn, word + depTransitions[i].a, foundWords);
        }
        else
        {
            continue;
        }
    }
}

int main()
{
    //FiniteAutomata automation("Autmt.txt");
    FiniteAutomata automation("Automata2.txt");
    //FiniteAutomata automation("Automata3.txt");
    vector<string>result = automation.findWords();
    cout << "Found words with empty word e:" << endl;
    for (int i = 0; i < result.size(); i++) {
        cout << result[i] << endl;
    }
    cout << "Found words without empty word e:" << endl;
    for (int i = 0; i < result.size(); i++) {
        result[i].erase(remove(result[i].begin(), result[i].end(), 'e'), result[i].end());
    }
    for (int i = 0; i < result.size(); i++) {
        cout << result[i] << endl;
    }
}

