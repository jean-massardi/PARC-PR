#include "solver.h"
/*
        solver(extendedPlanLibrary* _epl);
        virtual ~solver();

        const string name;

        void addObservations(int symbol);
        void reset();
        int getMostProbableGoal();
        const unordered_map<int,float>& getCurrentSolution() const;
        const vector<int>& getObservations() const;

    private:
        void update();

        unordered_map<int,float> currentSolution;
        extendedPlanLibrary* epl;
        vector<int> observations;*/

solver::solver()
{
}

solver::solver(extendedPlanLibrary* _epl)
{    //ctor
}

solver::~solver()
{
}

randomSolver::randomSolver(extendedPlanLibrary* _epl):epl(_epl)
{
    name = "Random";
    int randGoal = epl->decisionModel.RNC(-1);
    currentSolution[randGoal] = 1.0;
}

particleSolver::particleSolver(extendedPlanLibrary* _epl, int _size):epl(_epl), size(_size)
{
    name = "Particle";
    //unordered_map<int, particle> particles;
}
