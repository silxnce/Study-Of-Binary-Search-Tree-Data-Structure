# Study-Of-Binary-Search-Tree-Data-Structure
Computer Science. Year 1. Algorithms and Data structures. Lab 8

# Task
Translated via ChatGPT

The "Student" structure has the following fields: name, average grade, desire to learn, the number of plagiarized works, and so on. Create a sorted "database" of students in which you can quickly check the presence of a specific student and find all students within a specified range (between two students).  

1. Create a "Data" structure for storing objects with characteristics according to the chosen task. Choose a criterion for comparing two objects and overload the necessary operators.  

2. Implement a binary search tree (BST):  
2.1 Create a basic tree element "Node" that will contain data and pointers to its left and right descendants. Optionally, add a pointer to the parent node.  
2.2 Create a "BinarySearchTree" structure with basic methods for an unbalanced binary search tree:  
  insert(object) – add a new element to the tree (without duplicates).  
  find(object) – check the presence of an element in the tree.  
  erase(object) – remove an element from the tree.  
  size() – find the number of elements in the tree.  
2.3 Implement additional methods for working with the tree:  
  print() – print all tree elements (in sorted order).  
  height() – find the height of the tree.  
  findInRange(minObject, maxObject) – find all elements in the tree within the range [minObject, maxObject] and return their count or a dynamic array (vector) with these elements.  

3. Perform testing using the provided testBinarySearchTree() function. Verify correctness and measure the speed of operation, comparing it with the ready-made STL set library solution.  

# Additional Tasks
Expand the implementation of the binary search tree from the main task:  

1. Implement balancing logic in the BinarySearchTree, similar to an AVL tree or another balanced tree (red-black tree / splay tree).  

2. Solve the "Identical Trees" problem:  
  Implement the function bool isSameTree(Node* t1, Node* t2) to check if two input trees are identical in structure and values in nodes.  

3. Implement binary search tree methods for working with subtrees:  
  merge(tree1, tree2) – merge two trees into one.  
  split(tree, object) – split the binary search tree by key into two others: the first containing all elements < object and the second containing >= object.  
  eraseRange(minObject, maxObject) – delete all elements in the tree within the range [minObject, maxObject] using the methods created above.  

Also GitHub has some issues with Cyrillic symbols, therefore, some comments and lines of code may not be displayed correctly.
