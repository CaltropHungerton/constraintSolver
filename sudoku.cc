#include "constraintsatbase.h"

class HorizontalConstraint : public Constraint
{
public:
    bool isSatisfied(std::vector<Variable> variables) override
    {
        for (int i = 0; i < 9; i++)
        {
            std::unordered_set<int> theset;
            for (int j = 0; j < 9; j++)
            {
                if (variables[(i*9)+j].isAssigned)
                {
                    if (theset.count(variables[(i*9)+j].value))
                    {
                        return false;
                    }
                    else
                    {
                        theset.insert(variables[(i*9)+j].value);
                    }
                }
            }
        }
        return true;
    }
};

class VerticalConstraint : public Constraint
{
public:
    bool isSatisfied(std::vector<Variable> variables) override
    {
        for (int i = 0; i < 9; i++)
        {
            std::unordered_set<int> theset;
            for (int j = 0; j < 9; j++)
            {
                if (variables[(j*9)+i].isAssigned)
                {
                    if (theset.count(variables[(j*9)+i].value))
                    {
                        return false;
                    }
                    else
                    {
                        theset.insert(variables[(j*9)+i].value);
                    }
                }
            }
        }
        return true;
    }
};

class BoxConstraint : public Constraint
{
public:
    bool isSatisfied(std::vector<Variable> variables) override
    {
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                std::unordered_set<int> theset;
                for (int k = 0; k < 3; k++)
                {
                    for (int w = 0; w < 3; w++)
                    {
                        if (variables[((((3*i)+k)*9)+((3*j)+w))].isAssigned) //check this
                        {
                            if (theset.count(variables[((((3*i)+k)*9)+((3*j)+w))].value))
                            {
                                return false;
                            }
                            else
                            {
                                theset.insert(variables[((((3*i)+k)*9)+((3*j)+w))].value);
                            }
                        }
                    }
                }
            }
        }
        return true;
    }
};

class SudokuCSP : public CSP
{
public:
    void prettyprint() override
    {
        for (int i = 0; i < 9; i++)
        {
            for (int j = 0; j < 9; j++)
            {
                if (variables[(i*9)+j].isAssigned)
                {
                    std::cout << variables[(i*9)+j].value << " ";
                }
                else
                {
                    std::cout << "_" << " ";
                }
            }
            std::cout << "\n\n";
        }
    }

    void load(std::string path) override
    {
        std::ifstream inFile(path);
        if (!inFile)
        {
            std::cerr << "Unable to open file: " << path << std::endl;
        }
        else
        {
            std::string line;
            for (int i = 0; i < 9; i++) 
            {
                if (!std::getline(inFile, line) || line.length() != 9) 
                {
                    std::cerr << "Invalid input in file: " << path << std::endl;
                }
                for (int j = 0; j < 9; j++)
                {
                    if (line[j] == '_')
                    {
                        variables.push_back(Variable(std::unordered_set<int>({1,2,3,4,5,6,7,8,9})));
                    }
                    else if (isdigit(line[j]) && line[j] != '0')
                    {
                        variables.push_back(Variable(line[j]-'0'));
                    }
                    else
                    {
                        std::cerr << "Invalid character in file: " << path << std::endl;
                    }
                }
            }

            assert(variables.size() == 81);

            auto horizontalConstraint = std::make_shared<HorizontalConstraint>();
            auto verticalConstraint = std::make_shared<VerticalConstraint>();
            auto boxConstraint = std::make_shared<BoxConstraint>();

            constraints.push_back(horizontalConstraint);
            constraints.push_back(verticalConstraint);
            constraints.push_back(boxConstraint);

            for (int i = 0; i < 81; i++)
            {
                if (variables[i].isAssigned)
                {
                    std::vector<int> _ = prune(i);
                }
            }
        }
    }

    bool impinges(int recent, int i) override
    {
        bool imp = ((recent % 9 == i % 9) &&
                    (recent / 9 == i / 9) &&
                    ((recent % 9) / 3 == (i % 9) / 3) &&
                    ((recent / 9) / 3 == (i / 9) / 3));
        
        return (imp && CSP::impinges(recent, i));
    }
};

int main()
{
    SudokuCSP sat = SudokuCSP();
    sat.load("sudokuweird.txt");
    sat.prettyprint();
    sat.backtrackBegin();
}