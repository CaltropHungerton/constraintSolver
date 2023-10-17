#pragma once
#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <utility>
#include <fstream>
#include <string>
#include <cassert>
#include <memory>
#include <climits>

class Variable
{
public:
    std::unordered_set<int> domain;
    int value;
    bool isAssigned;

    Variable(std::unordered_set<int> d);
    Variable(int v);
};

class Constraint
{
public:
    virtual bool isSatisfied(std::vector<Variable>) = 0;
};

class CSP
{
public:
    std::vector<Variable> variables;
    std::vector<std::shared_ptr<Constraint>> constraints;

    virtual void load(std::string path) = 0;
    void print();
    virtual void prettyprint() = 0;
    bool assigned();
    bool satisfied();
    int minDomain(std::vector<Variable>);
    virtual bool impinges(int recent, int i);
    std::vector<int> prune(int recent);
    void backtrack(int recent);
    void backtrackBegin();
};
