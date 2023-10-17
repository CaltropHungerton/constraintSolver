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

/*
pseudocode:

generically: we find the variable with the most constrained domain (while not being assigned)
    handle logic within a function/method
maybe we add other heuristics for assignment later
we backtrack on its possible assignments, if it satisfies, we prune domains of other variables as need be, call on new setup (what setup exactly?)




use hashmap as enum, populate it in problem setup
also initialize the constraints, put ptrs to them in the contraint vector
we do this by overriding the constraint class within the new thing

as we iterate through representation (sudoku ex)

overwrite load method as well i guess, maybe do initial domain prune as well

but in sudoku, we overwrite the csp with the board representation (9x9)
i guess we also have the constraints adapted to this as well
-row constraint: checks if all rows are consistent. makes hashmap, if assigned==true, puts in hashmap, makes sure no collisions.
does this for each row
-column constraint: same but for each column
-box constraint: does this for each 3x3 grid
do seperate for now.
also we're doing this on 1d array.

have to make sure i'm doing generics correctly

if all satisfied:
    if all assigned: print whatever

    else: we prune relative to the new thing that was assigned/called with (have position in bespoke representation like grid as arg?)
    this will potentially alter order of priority queue (of pointers probably). we select the variable at front, iterate over domain, calling
    recursively with edited assignment and priority queue

else:
    break

what this will involve:

satisfied bool
assigned bool/equivalent check (for all variables)
assigned bool for single variable (no need, public field)
prune: iterate over all unassigned variables (call with recently assigned thing, which we also know, was an arg)
    if it is within purview of constraint (and unassigned):
        remove from the var's hashmap
how to make generic prune: call with the location of recently assigned in representation (domain specific)
    iterate over all variables
    figure out how to check if it is in constraint purview of recent and whether it isn't assigned
    if so: remove recent.value from the domain of that thing

queue lambda:
    auto compare = [](Variable const& a, Variable const& b) {
        return a.domain.size() > b.domain.size();
    };






what do i do with the recurse?
so i initialize the fields, do initial prune
then i pick smallest domain
delete smallest from priority queue
for x in slst.domain:
    slst.value = x
    slst.assigned = true
    backtrackhelper() // this is the one that recursively calls itself
    slst.assigned = false
    

constructors for variables:
    we might override these depending on stuff
    but for now default constructor: isAssigned = false, domain is populated with full domain specific to the problem (only 1 argument)
    assignment constructor: isAssigned = true, domain is empty i guess, value is the value

i think i will overload





ok this looks great so far! unfortunately i can't debug anything until i have a child class set up. i'll do sudoku first.
*/