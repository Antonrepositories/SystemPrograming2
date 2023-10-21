#include <iostream>
#include<fstream>
#include<set>
#include<sstream>
#include<map>
#include<vector>
#include<string>
#include <regex>
#include <algorithm>
#include <queue>
#include<regex>

using namespace std;
struct Transition {
    int s;
    char a;
    int sn;
};

class FiniteAutomata {
public:
    FiniteAutomata(const string& filename);
    set<string>possiblePeriod;
    vector<string> findWords();
private:
    string characters = "abcdefghijklmnopqrstuvwxyz";
    int periodSize;
    //char eps = 'e';
    int alphabetSize;
    int statesAmount;
    int transitionsAmount = 0;
    vector<int> states;
    int startState;
    int finalStatesAmount;
    set<int> finalStates;
    vector<char> alphabet;
    map<pair<int, char>, int> transitions;
    vector<Transition> transitins;
    vector<Transition>visited;
    void Result(int currentState, string word, vector<string>& foundWords, set<int>&visited);
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
        transitionsAmount++;
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

bool isEqual(Transition tr1, Transition tr2) {
    int s, s1, sn, sn1;
    char a, a1;
    s = tr1.s;
    s1 = tr2.s;
    sn = tr1.sn;
    sn1 = tr2.sn;
    a = tr1.a;
    a1 = tr2.a;
    if (s == s1) {
        if (sn == sn1) {
            if (a == a1) {
                return true;
            }
        }
    }
    else {
        return false;
    }
}
vector<string> FiniteAutomata::findWords() {
    vector<string> foundWords;
    set<int>visited;
    Result(startState, "", foundWords, visited);
    return foundWords;
}
void FiniteAutomata::Result(int currentState, string word, vector<string>&foundWords, set<int>&visited) {
    visited.insert(currentState);
    if (finalStates.count(currentState) > 0) {
        foundWords.push_back(word);
    }
    vector<Transition> depTransitions;
    for (int i = 0; i < transitins.size(); i++) {
        if (transitins[i].s == currentState) {
            depTransitions.push_back(transitins[i]);
        }
    }
    int max_plus = *finalStates.rbegin();
    for (int i = 0; i < depTransitions.size(); i++) {
        if (word.length() <= transitionsAmount + max_plus) {
            Result(depTransitions[i].sn, word + depTransitions[i].a, foundWords, visited);
            if (depTransitions[i].s > depTransitions[i].sn) {
                string period = word + depTransitions[i].a;
                period.erase(remove(period.begin(), period.end(), 'e'), period.end());
                while (period.size() > 1 && word.size() < alphabetSize) {
                    possiblePeriod.insert(period);
                    period.erase(period.begin());
                }
            }
        }
        else
        {
            continue;
        }
    }

}

int main()
{
    FiniteAutomata automation("Autmt.txt");
    //FiniteAutomata automation("Automata2.txt");
    vector<string>result = automation.findWords();
    for (int i = 0; i < result.size(); i++) {
        result[i].erase(remove(result[i].begin(), result[i].end(), 'e'), result[i].end());
    }
    //deleting words with periodic fragments
    vector<string>perFrag;
    for (auto it = automation.possiblePeriod.begin(); it != automation.possiblePeriod.end(); it++)
    {
        //cout << *it + *it << endl;
        perFrag.push_back(*it + *it);

    }
    cout << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << endl;
    vector<string>toDelete;
    for (int i = 0; i < result.size(); i++) {
        if (result[i].size() >= 2) {
            string word = result[i];
            
            for (int j = 0; j < word.length() - 1; j++) {
                char f = word[j];
                char n = word[j + 1];
                if (f == n) {
                    //cout << word << endl;
                    toDelete.push_back(result[i]);
                    break;
                }
            }
        }
        string word = result[i];
        for (int k = 0; k < perFrag.size(); k++) {
            string period = perFrag[k];
            if (word.length() > period.length()) {
                if (word.find(period) != string::npos) {
                    toDelete.push_back(word);
                }
            }
        }
    }
    for (int i = 0; i < toDelete.size(); i++) {
        result.erase(remove(result.begin(), result.end(), toDelete[i]), result.end());
    }
    //printing final result
    cout << "Found words without periodic fragments:" << endl;
    set<string>final_result;
    for (int i = 0; i < result.size(); i++) {
        //cout << result[i] << endl;
        final_result.insert(result[i]);
    }
    for (auto it = final_result.begin(); it != final_result.end(); it++)
    {
        cout << *it << endl;
    }


}

