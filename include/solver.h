#include "extendedPlanLibrary.h"
#include "rule.h";
#include <vector>
#include <map>
#include <iostream>

#ifndef SOLVER_H
#define SOLVER_H

using namespace std;

class treeNode
{
    public:
    treeNode();
    treeNode(extendedPlanLibrary* _epl, int _symbol);

    extendedPlanLibrary* epl;
    int symbol;
    int _rule;
    bool status;
    map<int,treeNode> children;

    int update();

};

class tree
{
    public:
        tree();
        tree(extendedPlanLibrary* _epl, int goal);

        extendedPlanLibrary* epl;
        treeNode root;
        vector<int> update();
        int updateFO();

};

class solverParticle
{
    public:
        solverParticle(extendedPlanLibrary* _epl);

        extendedPlanLibrary* epl;
        int goal;
        vector<int> expNextObs;
        tree planTree;
        bool update();
};

class solver
{
    public:
        solver(extendedPlanLibrary* _epl, int _nbParticle);
        virtual ~solver();

        bool addObservation(int obs);
        bool status();
        map<int, int> getGoals();
        map<int, int> getParticles();
        int getMaxGoal();
        const int& getSize() const;
        void clear();

        pair<int,vector<int>> generatePlan();
        pair<int,vector<int>> generatePlanFO();

    private:
        int nbParticles;
        extendedPlanLibrary* epl;
        vector<int> previousObservations;
        map<int, vector<solverParticle>> particles;

};

#endif // SOLVER_H
