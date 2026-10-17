# AVL Tree in C++

![build](https://github.com/YusufUtkuBeygo/AVL/actions/workflows/build.yml/badge.svg)

A self-balancing binary search tree written from scratch in C++ for my Data Structures course.
After every insert and delete the tree checks the balance factor of each node on the path and fixes it with rotations, so the height stays `O(log n)`.

## Features

- **Insert** with all four rebalancing cases: left-left, left-right, right-right, right-left
- **Delete** for leaf, single-child and two-children nodes (in-order successor), with rebalancing on the way back up
- **Search**, **height** and **pre-order traversal**
- Memory is released in the destructor

## Example

Inserting `10 20 30 40 50 25` in order would turn a plain BST into a linked list. The AVL tree rotates and stays balanced:

```
PreOrder: 30 20 10 25 40 50
Height: 2
Contains 25: yes
Contains 99: no
After removing 40: 30 20 10 25 50
Height: 2
```

## Run

```bash
g++ -std=c++17 AVL.cpp -o avl
./avl
```

Or open `AVL.sln` in Visual Studio.
