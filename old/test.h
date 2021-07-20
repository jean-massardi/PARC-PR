#include <vector>
#include <string>
#include "solver.h"
#include "extendedPlanLibrary.h"

using namespace std;

#ifndef TEST_H
#define TEST_H


class test
{
    public:
        test();
        test(extendedPlanLibrary* _epl);
        virtual ~test();

        void addSolver(solver s);
        void newPlan();
        bool next();

    private:
        vector<solver> solvers;
        vector<int> accuracy;

        int goal;
        vector<int> plan;
        int planStep;

        extendedPlanLibrary* pl;
};

#endif // TEST_H
