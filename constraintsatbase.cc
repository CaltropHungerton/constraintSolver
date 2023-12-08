#include "constraintsatbase.h"

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

// Variable class implementations

Variable::Variable(std::unordered_set<int> d) : domain(d), isAssigned(false) {}
Variable::Variable(int v) : isAssigned(true), value(v) {}

// CSP class implementations

void CSP::print()
{
    int count = 0;
    for (Variable v : variables)
    {
        if (v.isAssigned)
        {
            std::cout << count << ": " << v.value << "\n\n";
        }
        else
        {
            std::cout << count << ": " << "?" << "\n\n";
        }
        count++;
    }
}

bool CSP::assigned()
{
    for (Variable v : variables)
    {
        if (!v.isAssigned)
        {
            return false;
        }
    }
    return true;
}

bool CSP::satisfied()
{
    for (int i = 0; i < constraints.size(); i++)
    {
        if (!constraints[i]->isSatisfied(variables))
        {
            return false;
        }
    }
    return true;
}

int CSP::minDomain(std::vector<Variable>)
{
    int min = INT_MAX;
    int index;
    int count = 0;
    for (Variable v : variables)
    {
        if ((!v.isAssigned) && v.domain.size() < min)
        {
            min = v.domain.size();
            index = count;
        }
        count++;
    }
    return index;
}

bool CSP::impinges(int recent, int i) {return ((!variables[i].isAssigned) && (recent != i));} // redefine later. V IMPORTANT TODO

std::vector<int> CSP::prune(int recent)
{
    std::vector<int> pruned;
    for (int i = 0; i < variables.size(); i++)
    {
        if (impinges(recent, i))
        {
            variables[i].domain.erase(variables[recent].value);
            pruned.push_back(i);
        }
    }
    return pruned;
}

void CSP::backtrackBegin()
{
    if (satisfied())
    {
        if (assigned())
        {
            std::cout << "Assignment found!" << "\n";
            prettyprint();
        }
        else
        {
            int idx = minDomain(variables);
            for (int v : variables[idx].domain)
            {
                variables[idx].value = v;
                variables[idx].isAssigned = true;
                backtrack(idx);
                variables[idx].isAssigned = false;
            }
        }
    }
}

void CSP::backtrack(int recent)
{
    if (satisfied())
    {
        if (assigned())
        {
            std::cout << "Assignment found!" << "\n";
            prettyprint();
        }
        else
        {
            // prune first, then find min
            int val = variables[recent].value;
            std::vector<int> pruned = prune(recent);
            int idx = minDomain(variables);
            for (int v : variables[idx].domain)
            {
                variables[idx].value = v;
                variables[idx].isAssigned = true;
                backtrack(idx);
                variables[idx].isAssigned = false;
            }
            // unprune
            for (int i : pruned)
            {
                variables[i].domain.insert(val);
            }
        }
    }
}
