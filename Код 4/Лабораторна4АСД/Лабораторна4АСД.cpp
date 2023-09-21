#include <iostream>
#include <cstring>
#include <vector>
#include <set>
#include <Windows.h>

using namespace std;

struct Data // Структура Студент
{
    char name[10];
    int mark, desire, cheat;

    Data()
    {
        char nameArr[20][10] = { "Emma", "Liam", "Olivia", "Noah", "Ava", "Sophia", "Isabella", "Mia", "Jackson", "Aiden", "Lucas", "Caden", "Amelia", "Mason",
            "Charlotte", "Harper", "Evelyn", "Lily", "Michael", "Daniel" };
        strcpy_s(name, nameArr[rand() % 15]);
        mark = rand() % 51 + 50;
        desire = rand() % 11;
        cheat = rand() % 6;
    }

    bool operator<(const Data& second) const
    {
        if (mark != second.mark)
            return mark < second.mark;
        if (desire != second.desire)
            return desire < second.desire;
        return cheat < second.cheat;
    }

    bool operator==(const Data& second) const
    {
        return mark == second.mark && desire == second.desire && cheat == second.cheat;
    }

    bool operator!=(const Data& second) const
    {
        return mark != second.mark && desire != second.desire && cheat != second.cheat;
    }

    bool operator<=(const Data& second) const
    {
        return *this < second || *this == second;
    }

    bool operator>(const Data& second) const
    {
        return second < *this;
    }

    bool operator>=(const Data& second) const
    {
        return *this > second || *this == second;
    }
};

struct Node
{
    Data data;
    int balance;    // Поточна різниця збалансованості піддерев
    Node* left;
    Node* right;

    Node(Data value)
    {
        data = value;
        balance = 0;
        left = nullptr;
        right = nullptr;
    }
};

struct BinarySearchTree
{
    Node* root = nullptr;
    int treeSize = 0;

    ~BinarySearchTree()
    {
        destroyTree(root);
    }

    void insert(Data data)  // Вставка елементу
    {
        root = insertNode(root, data);
    }

    bool find(Data data)    // Перевірка наявності елемента
    {
        Node* node = root;
        while (node)
        {
            if (data == node->data)    // Якщо елемент знайдено
                return true;
            else if (data < node->data)    // Якщо елемент менше за значення вершини
                node = node->left;    // Пошук у лівому піддереві
            else
                node = node->right;    // В іншому випадку пошук у правому
        }

        return false;
    }

    void erase(Data data)   // Видалення елементу
    {
        root = eraseNode(root, data);
    }

    int size()   // Розмір дерева
    {
        return treeSize;
    }

    void print()   // Виведення всіх елементів
    {
        printInOrder(root);
    }

    int height()   // Висота дерева
    {
        return getHeight(root);
    }

    int findInRange(Data minObject, Data maxObject)   // Знайти кількість елементів у заданому діапазоні
    {
        return findInRange(root, minObject, maxObject);
    }

    Node* merge(Node* tree1, Node* tree2)   // Об'єднання дерев
    {
        if (!tree1)
            return tree2;
        if (!tree2)
            return tree1;

        Node* mergedTree = nullptr;
        if (tree1->data < tree2->data)   // Значення у кореня tree1 менше за значення у кореня tree2
        {
            mergedTree = tree1; // tree1 стає коренем об'єднаного дерева
            mergedTree->right = merge(tree1->right, tree2); // Рекурсивно об'єднуємо праве піддерево tree1 з tree2
        }
        else   // Значення у кореня tree2 менше або рівне значенню у кореня tree1
        {
            mergedTree = tree2; // tree2 стає коренем об'єднаного дерева
            mergedTree->left = merge(tree1, tree2->left);   // Рекурсивно об'єднуємо tree1 з лівим піддеревом tree2
        }

        updateBalance(mergedTree);    // Оновлення різниці збалансованості
        return balance(mergedTree);    // Зміна балансу (при необхідності)
    }

    pair<Node*, Node*> split(Node* tree, Data object)
    {
        if (!tree)   // Якщо вказане дерево пусте
            return make_pair(nullptr, nullptr);

        pair<Node*, Node*> splitTrees;
        if (object < tree->data)    // Об'єкт менший за значення вершини, розділяємо ліве піддерево
        {
            splitTrees = split(tree->left, object);
            tree->left = splitTrees.second; // Праве піддерево розділеного дерева буде новим лівим піддеревом поточної вершини
            splitTrees.second = tree; // Поточна вершина стає новим правим піддеревом розділеного дерева
        }
        else   // Об'єкт більший або рівний за значення вершини, розділяємо праве піддерево
        {
            splitTrees = split(tree->right, object);
            tree->right = splitTrees.first; // Ліве піддерево розділеного дерева буде новим правим піддеревом поточної вершини
            splitTrees.first = tree; // Поточна вершина стає новим лівим піддеревом розділеного дерева
        }

        return splitTrees;
    }

    void eraseRange(Data minObject, Data maxObject)
    {
        if (!root)
            return;

        pair<Node*, Node*> splitLeft = split(root, minObject);
        pair<Node*, Node*> splitRight = split(splitLeft.second, maxObject);

        // Видаляємо внутрішні дерева між minObject та maxObject
        destroyTree(splitLeft.second);
        destroyTree(splitRight.first);

        // Знову об'єднуємо ліве та праве дерева
        root = merge(splitLeft.first, splitRight.second);
    }

private:

    Node* insertNode(Node* node, Data data)    // Вставка елементу
    {
        if (!node)    // Місце для вставки знайдено
        {
            treeSize++;
            return new Node(data);
        }

        if (data < node->data)  // Елемент має знаходитись у лівому піддереві
            node->left = insertNode(node->left, data);
        else if (data > node->data)    // У правому піддереві
            node->right = insertNode(node->right, data);
        else    // Знайдено дублікат
            return node;

        updateBalance(node);    // Оновлення різниці збалансованості
        return balance(node);    // Зміна балансу (при необхідності)
    }

    Node* eraseNode(Node* node, Data data)    // Видалення елементу
    {
        if (!node)    // Бінарне дерево пошуку пусте
            return nullptr;

        if (data < node->data)    // Пошук елемента у лівому піддереві
            node->left = eraseNode(node->left, data);
        else if (data > node->data)
            node->right = eraseNode(node->right, data);    // У правому
        else    // Елемент знайдено
        {
            if (!node->left && !node->right)    // Дочірні елементи відсутні
            {
                treeSize--;
                delete node;
                return nullptr;
            }
            else if (!node->left)    // Немає лівого дочірнього елементу
            {
                treeSize--;
                Node* temp = node->right;
                delete node;
                return temp;
            }
            else if (!node->right)    // Немає правого
            {
                treeSize--;
                Node* temp = node->left;
                delete node;
                return temp;
            }
            else    // Обидва дочірні елементи присутні
            {
                Node* minRight = findMin(node->right);
                node->data = minRight->data;    // Заміна найменшим елементом правого піддерева
                node->right = eraseNode(node->right, minRight->data);
            }
        }

        updateBalance(node);    // Оновлення різниці збалансованості
        return balance(node);    // Зміна балансу (при необхідності)
    }

    void updateBalance(Node* node)  // Оновлення різниці збалансованості
    {
        node->balance = getHeight(node->right) - getHeight(node->left); // Від висоти правого піддерева віднімаєтьс висота лівого
    }

    Node* balance(Node* node)   // Зміна балансу
    {
        if (node->balance == -2)   // Висота лівого піддерева більша
        {
            if (node->left->balance <= 0)
                return rotateRight(node);   // Мале праве обертання
            else   // Велике праве обертання
            {
                node->left = rotateLeft(node->left);
                return rotateRight(node);
            }
        }
        else if (node->balance == 2)   // Висота правого піддерева більша
        {
            if (node->right->balance >= 0)   // Мале ліве обертання
                return rotateLeft(node);
            else   // Велике ліве обертання
            {
                node->right = rotateRight(node->right);
                return rotateLeft(node);
            }
        }

        return node;
    }

    Node* rotateRight(Node* node)   // Мале праве обертання
    {
        Node* newRoot = node->left;
        node->left = newRoot->right;
        newRoot->right = node;

        updateBalance(node);
        updateBalance(newRoot);

        return newRoot;
    }

    Node* rotateLeft(Node* node)   // Мале ліве обертання
    {
        Node* newRoot = node->right;
        node->right = newRoot->left;
        newRoot->left = node;

        updateBalance(node);
        updateBalance(newRoot);

        return newRoot;
    }

    Node* findMin(Node* node)   // Знайде найменший елемент у піддереві
    {
        while (node->left)
            node = node->left;
        return node;
    }

    Node* findMax(Node* node)   // Знайде найбільший елемент у піддереві
    {
        while (node->right)
            node = node->right;
        return node;
    }

    void printInOrder(Node* node)   // Виведення елементів від найменшого до найбільшого
    {
        if (node)
        {
            printInOrder(node->left);
            cout << "Name: " << node->data.name << "; Mark: " << node->data.mark << "; Desire: " << node->data.desire << "; Cheat: " << node->data.cheat << endl;
            printInOrder(node->right);
        }
    }

    int getHeight(Node* node)   // Знайти висоту піддерева
    {
        if (!node)
            return -1;
        int leftHeight = getHeight(node->left);
        int rightHeight = getHeight(node->right);
        return 1 + max(leftHeight, rightHeight);
    }

    int findInRange(Node* node, Data minObject, Data maxObject)
    {
        if (!node)
            return 0;
        if (node->data >= minObject && node->data <= maxObject)   // Досліджувана вершина знаходиться у проміжку
            return 1 + findInRange(node->left, minObject, maxObject) + findInRange(node->right, minObject, maxObject);
        else if (node->data < minObject)   // Досліджувана вершина менша за проміжок пошуку
            return findInRange(node->right, minObject, maxObject);
        else   // Досліджувана вершина більша за проміжок пошуку
            return findInRange(node->left, minObject, maxObject);
    }

    void destroyTree(Node* node)   // Видалення дерева та очищення пам'яті 
    {
        if (node != nullptr)
        {
            destroyTree(node->left);
            destroyTree(node->right);
            delete node;
        }
    }
};

long long generateRandLong() // Функція генерації довгого числа
{
    long long num = 0;
    int maxLen = rand() % 12 + 1;

    for (int i = 0; i < maxLen; i++)
        num += (rand() % 10) * pow(10, i);
    return num;
}

bool isSameTree(Node* t1, Node* t2)
{
    if (!t1 && !t2)    // Базовий випадок: якщо обидва дерева порожні, вони однакові
        return true;
    if (!t1 || !t2)    // Якщо лише одне дерево порожнє, вони різні
        return false;
    if (t1->data != t2->data)   // Перевірка, чи значення вузлів однакові
        return false;

    // Рекурсивна перевірка для лівих та правих піддерев
    bool leftSame = isSameTree(t1->left, t2->left);
    bool rightSame = isSameTree(t1->right, t2->right);

    return leftSame && rightSame;    // Два дерева однакові тільки тоді, коли обидва піддерева однакові
}



bool testBinarySearchTree()
{
    srand(time(NULL));
    const int iters = 80000;
    const int keysAmount = iters * 2;
    const int itersToRangeQueries = 1000;
    vector<Data> data(keysAmount);
    vector<Data> dataToInsert(iters);
    vector<Data> dataToErase(iters);
    vector<Data> dataToFind(iters);
    vector<pair<Data, Data>> dataToRangeQueries;
    for (int i = 0; i < iters; i++)
    {
        dataToInsert[i] = data[generateRandLong() % keysAmount];
        dataToErase[i] = data[generateRandLong() % keysAmount];
        dataToFind[i] = data[generateRandLong() % keysAmount];
    }
    for (int i = 0; i < itersToRangeQueries; i++)
    {
        Data minData = Data();
        Data maxData = Data();
        if (maxData < minData)
        {
            swap(minData, maxData);
        }
        dataToRangeQueries.push_back({ minData, maxData });
    }
    BinarySearchTree myTree;
    clock_t myStart = clock();
    for (int i = 0; i < iters; i++)
    {
        myTree.insert(dataToInsert[i]);
    }
    int myInsertSize = myTree.size();
    int myTreeHeight = myTree.height();
    int optimalTreeHeight = log2(myInsertSize) + 1;
    for (int i = 0; i < iters; i++)
    {
        myTree.erase(dataToErase[i]);
    }
    int myEraseSize = myInsertSize - myTree.size();
    int myFoundAmount = 0;
    for (int i = 0; i < iters; i++)
    {
        if (myTree.find(dataToFind[i]))
        {
            myFoundAmount++;
        }
    }
    clock_t myEnd = clock();
    float myTime = (float(myEnd - myStart)) / CLOCKS_PER_SEC;
    set<Data> stlTree;
    clock_t stlStart = clock();
    for (int i = 0; i < iters; i++)
    {
        stlTree.insert(dataToInsert[i]);
    }
    int stlInsertSize = stlTree.size();
    for (int i = 0; i < iters; i++)
    {
        stlTree.erase(dataToErase[i]);
    }
    int stlEraseSize = stlInsertSize - stlTree.size();
    int stlFoundAmount = 0;
    for (int i = 0; i < iters; i++)
    {
        if (stlTree.find(dataToFind[i]) != stlTree.end())
        {
            stlFoundAmount++;
        }
    }
    clock_t stlEnd = clock();
    float stlTime = (float(stlEnd - stlStart)) / CLOCKS_PER_SEC;
    clock_t myRangeStart = clock();
    int myRangeFoundAmount = 0;
    for (int i = 0; i < itersToRangeQueries; i++)
    {
        myRangeFoundAmount += myTree.findInRange(dataToRangeQueries[i].first, dataToRangeQueries[i].second);
    }
    clock_t myRangeEnd = clock();
    float myRangeTime = (float(myRangeEnd - myRangeStart)) / CLOCKS_PER_SEC;
    clock_t stlRangeStart = clock();
    int stlRangeFoundAmount = 0;
    for (int i = 0; i < itersToRangeQueries; i++)
    {
        const auto& low = stlTree.lower_bound(dataToRangeQueries[i].first);
        const auto& up = stlTree.upper_bound(dataToRangeQueries[i].second);
        stlRangeFoundAmount += distance(low, up);
    }
    clock_t stlRangeEnd = clock();
    float stlRangeTime = (float(stlRangeEnd - stlRangeStart)) / CLOCKS_PER_SEC;
    cout << "My BinaryTree: height = " << myTreeHeight << ", optimal height = " << optimalTreeHeight << endl;
    cout << "Time: " << myTime << ", size: " << myInsertSize << " - " << myEraseSize << ", found amount : " << myFoundAmount << endl;
    cout << "Range time: " << myRangeTime << ", range found amount: " << myRangeFoundAmount << endl << endl;
    cout << "STL Tree:" << endl;
    cout << "Time: " << stlTime << ", size: " << stlInsertSize << " - " << stlEraseSize << ", found amount: " << stlFoundAmount << endl;
    cout << "Range time: " << stlRangeTime << ", range found amount: " << stlRangeFoundAmount << endl << endl;
    if (myInsertSize == stlInsertSize && myEraseSize == stlEraseSize && myFoundAmount == stlFoundAmount && myRangeFoundAmount == stlRangeFoundAmount)
    {
        cout << "The lab is completed" << endl;
        return true;
    }
    cerr << ":(" << endl;
    return false;
}

int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    testBinarySearchTree();

    BinarySearchTree first, second;

    Data d1, d2, d3, d4;

    if (isSameTree(first.root, second.root))
        cout << "\nFunction isSameTree works correctly when both trees are empty.\n";

    first.insert(d1);
    first.insert(d2);
    first.insert(d3);

    second.insert(d1);
    second.insert(d2);
    second.insert(d4);

    if (!isSameTree(first.root, second.root))
        cout << "Function isSameTree works correctly when different objects are inserted.\n";

    second.erase(d4);
    second.insert(d3);

    if (isSameTree(first.root, second.root))
        cout << "Function isSameTree works correctly when they consist of the same objects.\n";
}