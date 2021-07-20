#include "test.h"

test::test():goal(-1), planStep(-1), pl(nullptr)
{
    //ctor
}

test::test(extendedPlanLibrary* _epl):pl(_epl)
{
    goal = pl->decisionModel.RNC(-1);
    planStep = 0;
    solver tempSolver = particleSolver(pl, 0);
    plan = tempSolver.generatePlan(goal);
}

void test::addSolver(solver s)
{
    if(s.getMostProbableGoal() == goal)
    {
        accuracy.push_back(0);
    }
    else
    {
        for(int i = 0; i < planStep; i++)
        {
            s.addObservation(plan[planStep]);
            if(s.getMostProbableGoal() == goal)
            {
                accuracy.push_back(i);
                break;
            }
        }
    }
    solvers.push_back(s);
    if(solvers.size() != accuracy.size())
        accuracy.push_back(-1);
}

void test::newPlan()
{
    goal = pl->decisionModel.RNC(-1);
    planStep = 0;
    solver tempSolver = particleSolver(pl, 0);
    plan = tempSolver.generatePlan(goal);

    for(int i = 0; i < solvers.size(); i++)
    {
        solvers[i].reset();
        if(solvers[i].getMostProbableGoal() == goal)
        {
            accuracy[i] = 0;
        }
        else
        {
            accuracy[i] = -1;
        }
    }
}

bool test::next()
{
    planStep++;
    if(planStep == plan.size())
        return false;
    for(int i = 0; i < solvers.size(); i++)
    {
        if(accuracy[i] == -1)
        {
            solvers[i].addObservation(plan[planStep]);
            if(solvers[i].getMostProbableGoal() == goal)
                accuracy[i] = planStep;
        }
    }
    return true;
}

test::~test()
{
    //dtor
}
