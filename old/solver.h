#include "extendedPlanLibrary.h"
#include "rule.h"
#include <vector>
#include <utility>
#include <unordered_map>
#include <string>

using namespace std;

#ifndef SOLVER_H
#define SOLVER_H


class solver
{
    public:
        solver();
        solver(extendedPlanLibrary* _epl);
        virtual ~solver();

        string name;

        virtual void addObservation(int symbol);
        virtual void reset();
        virtual vector<int> generatePlan(int goal);
        int getMostProbableGoal();
        const unordered_map<int,float>& getCurrentSolution() const;
        const vector<int>& getObservations() const;

    private:
        virtual void update();
        unordered_map<int,float> currentSolution;
        extendedPlanLibrary* epl;
        vector<int> observations;

};

class randomSolver: public solver
{
    public:
        randomSolver(extendedPlanLibrary* _epl);
    private:
        virtual void update();
        unordered_map<int,float> currentSolution;
        extendedPlanLibrary* epl;
        vector<int> observations;
};

class particleSolver: public solver
{
    public:
        particleSolver(extendedPlanLibrary* _epl, int _size);
        vector<int> generatePlan(int goal);
    private:
        class particle
        {
            public:
               bool finish;
               rule* r;
               vector<particle> children;
               pair<int, bool> expand();
        };
        int size;
        unordered_map<int, particle> particles;

        virtual void update();
        unordered_map<int,float> currentSolution;
        extendedPlanLibrary* epl;
        vector<int> observations;
};

class TDPHATT: public solver
{

};

class BUPHATT: public solver
{

};

class SLIM: public solver
{

};

class DOPLAR: public solver
{

};
#endif // SOLVER_H
