#include <iostream>
#include <unordered_map>
#include <fstream>
#include <vector>

using namespace std;

struct Node
{
    unordered_map<char, Node*> children; // Хеш-таблиця для збереження символів
    bool isWordEnd = false;
};

struct Trie {
    Node* root;

    Trie()
    {
        root = new Node();
    }

    ~Trie()
    {
        delete root;
    }

    void insert(string word)
    {
        Node* currentNode = root;

        for (char c : word)
        {
            if (currentNode->children.find(c) == currentNode->children.end()) // Перевіряємо, чи вже є вузол для даного символу
                currentNode->children[c] = new Node(); // Якщо немає, створюємо новий вузол
            currentNode = currentNode->children[c]; // Переходимо до дочірнього вузла з символом c
        }

        currentNode->isWordEnd = true; // Позначаємо кінець слова
    }

    vector<string> findByPrefix(string prefix)
    {
        vector<string> wordsArr;
        Node* currNode = root;

        for (char c : prefix)   // Цикл проходження по необхідним гілкам
        {
            if (currNode->children.find(c) == currNode->children.end()) // Перевіряємо, чи існує вузол з символом c
                return wordsArr; // Якщо ні, повертаємо порожній вектор слів
            currNode = currNode->children[c]; // Переходимо до наступного вузла з символом c
        }

        FBPInner(prefix, currNode, "", wordsArr); // Викликаємо рекурсивну функцію для знаходження слів за префіксом
        return wordsArr;
    }

    void addWordsFromFile(const string& filename) {
        ifstream file(filename);   // Відкриваємо файл зі словами

        if (file.is_open()) {
            string word;

            while (file >> word)
                insert(word); // Додаємо слова з файлу до префіксного дерева

            file.close();
            cout << "Words from file '" << filename << "' added to the trie." << endl;
        }
        else
            cout << "Unable to open file '" << filename << "'." << endl;
    }

private:
    void FBPInner(string prefix, Node* node, string currWord, vector<string>& wordsArr)   // Рекурсивна функція знаходження слів за префіксом
    {
        if (node->isWordEnd)   // Якщо знайдено кінець слова, додаємо у масив
            wordsArr.push_back(prefix + currWord);

        for (auto pair : node->children)   // Цикл проходження по всім гілкам досліджуваної вершини
        {
            char c = pair.first;
            Node* childNode = pair.second;

            FBPInner(prefix, childNode, currWord + c, wordsArr);    // Рекурсивний виклик для нащадка
        }
    }
};

int main() {
    Trie trie;

    trie.addWordsFromFile("words_alpha.txt");

    string prefix;
    cout << "Input: > ";
    cin >> prefix;

    vector<string> wordsArr = trie.findByPrefix(prefix);

    if (!wordsArr.empty()) {
        cout << "Output: > ";
        for (string word : wordsArr)
            cout << word << "; ";
        cout << endl;
    }
    else
        cout << "No autocomplete suggestions found for the prefix '" << prefix << "'." << endl;
}