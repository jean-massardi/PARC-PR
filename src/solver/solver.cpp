#include "solver.h"

solver::solver(extendedPlanLibrary* _epl, int _nbParticle):epl(_epl), nbParticles(_nbParticle)
{
    int currentNbParticle = 0;
    while(currentNbParticle < nbParticles)
    {
        solverParticle newParticle = solverParticle(epl);
//        assert(newParticle.expNextObs[0]);

        if(particles.count(newParticle.expNextObs[0]) > 0)
            particles[newParticle.expNextObs[0]].push_back(newParticle);
        else
        {
            particles[newParticle.expNextObs[0]] = vector<solverParticle>();
            particles[newParticle.expNextObs[0]].push_back(newParticle);
        }

        currentNbParticle++;
    }
}

solver::~solver()
{
    //dtor
}

bool solver::addObservation(int obs)
{
    previousObservations.push_back(obs);
    vector<solverParticle> validParticles = particles[obs];
    if(validParticles.size() == 0)
    {
        particles.clear();
        return false;
    }

    while(validParticles.size() < nbParticles)
    {
        solverParticle newParticle = particles[obs][rand() % particles[obs].size()];
        validParticles.push_back(newParticle);
    }

    particles.clear();
    for(auto it : validParticles)
    {

        if(it.update())
        {
            if(particles.count(it.expNextObs[0]) >0)
                particles[it.expNextObs[0]].push_back(it);
            else
            {
                particles[it.expNextObs[0]] = vector<solverParticle>();
                particles[it.expNextObs[0]].push_back(it);
            }
        }

    }
    return status();
}

bool solver::status()
{
    return (particles.size() != 0);
}

map<int,int> solver::getGoals()
{
    map<int,int> resu;
    for(auto itObs : particles)
    {
        for(auto itPar : itObs.second)
        {
            if(resu.count(itPar.goal) > 0)
                resu[itPar.goal]++;
            else
            {
                resu[itPar.goal] = 1;
            }
        }
    }
    //TODO
    return resu;
}

map<int, int> solver::getParticles()
{
    map<int, int> resu;
    for (auto itObs : particles)
    {
        resu[itObs.first] = itObs.second.size();
    }
    return resu;
}

const int& solver::getSize() const
{
    return nbParticles;
}

pair<int,vector<int>> solver::generatePlan()
{
    tree t = tree(epl,epl->ePlanLibrary->getRules().at(epl->decisionModel.RNC(-1)).getChildren().at(0));
    vector<int> resu;
    int nextAction = t.updateFO();
    while(nextAction != -1)
    {
        int PO = epl->noise.RNC(nextAction);
        if(PO > -1)
            resu.push_back(PO);
        nextAction = t.updateFO();
    }
    return make_pair(t.root.symbol,resu);
}

pair<int,vector<int>> solver::generatePlanFO()
{
    tree t = tree(epl,epl->ePlanLibrary->getRules().at(epl->decisionModel.RNC(-1)).getChildren().at(0));
    vector<int> resu;
    int nextAction = t.updateFO();
    while(nextAction != -1)
    {
        resu.push_back(nextAction);
        nextAction = t.updateFO();
    }
    return make_pair(t.root.symbol,resu);
}

int solver::getMaxGoal()
{
    map<int,int> goals = getGoals();
    if(goals.empty())
        return -1;
    int resu = -1;
    int value = 0;
    for(auto& it: goals)
    {
        if(value < it.second)
        {
            value = it.second;
            resu = it.first;
        }
    }
    return resu;
}

void solver::clear()
{
    previousObservations.clear();
    particles.clear();

    int currentNbParticle = 0;
    while(currentNbParticle < nbParticles)
    {
        solverParticle newParticle = solverParticle(epl);
//        assert(newParticle.expNextObs[0]);

        if(particles.count(newParticle.expNextObs[0]) > 0)
            particles[newParticle.expNextObs[0]].push_back(newParticle);
        else
        {
            particles[newParticle.expNextObs[0]] = vector<solverParticle>();
            particles[newParticle.expNextObs[0]].push_back(newParticle);
        }

        currentNbParticle++;
    }

}

solverParticle::solverParticle(extendedPlanLibrary* _epl):epl(_epl)
{
    int ruleGoal = epl->decisionModel.RNC(-1);
    goal = epl->ePlanLibrary->getRules().at(ruleGoal).getChildren().at(0);
    planTree = tree(epl,goal);
    expNextObs = planTree.update();
}

bool solverParticle::update()
{
    expNextObs.erase(expNextObs.begin());
    if(expNextObs.empty())
    {
        expNextObs = planTree.update();
    }
    return(!expNextObs.empty());
}

tree::tree()
{

}

tree::tree(extendedPlanLibrary* _epl, int goal):epl(_epl)
{
    root = treeNode(epl,goal);
}

//temp fix on extraneous action
vector<int> tree::update()
{
    if(root.status)
    {
         //cout << "t status "<<endl;
        return vector<int>();
    }

    int FO = updateFO();
    //cout << "t - FO :"<< FO<<endl;
    if(FO == -1)
    {
        //cout << "t status "<<endl;
        return vector<int>();
    }
    int PO = epl->noise.RNC(FO);

    if (PO == -1)
    {
        return update();
    }

    vector<int> resu;
    if (PO < -1)
    {
        while(resu.size() + PO < -1)
        {
            int temp = epl->decisionModel.RNC(FO);
            if(temp >= 0)
                resu.push_back(temp);
        }
    }
    resu.push_back(FO);
    return resu;
}

int tree::updateFO()
{
    return root.update();
}

treeNode::treeNode()
{
}

treeNode::treeNode(extendedPlanLibrary* _epl, int _symbol):epl(_epl), symbol(_symbol)
{
    if(!epl->ePlanLibrary->isTerminal(symbol))
    {
        _rule = epl->decisionModel.RNC(symbol);
        status = false;
    }
    else
    {
        _rule = -1;
        status = true;
    }
}

int treeNode::update()
{
    if(status)
    {
        return -1;
    }
    rule r = epl->ePlanLibrary->getRules().at(_rule);
    //trouver un element 'child' valable
    vector<int> list_candidate;
    for(int i =0;i < r.getChildren().size();i++)
    {
            bool ok = true;
           for(auto& itC : r.getConstraints())
            {
                if(itC.second == i)
                {
                    if(children.count(itC.first) == 0)
                    {
                        ok = false;
                        break;
                    }
                    else if(!children[itC.first].status)
                    {
                        ok = false;
                        break;
                    }
                }
            }
            if(children.count(i) > 0)
                if(children[i].status)
                    ok = false;
            if(ok)
                list_candidate.push_back(i);

    }
    int childId = list_candidate[rand() % list_candidate.size()];

    treeNode* child;
    if(children.count(childId) >0)
        child = &children[childId];
    else
    {
        children[childId] = treeNode(epl,r.getChildren()[childId]);
        child = &children[childId];
    }


    int resu;
    if(child->status)
    {
        resu = child->symbol;
    }
    else
        resu = child->update();

    status = children.size() == r.getChildren().size();
    if(status)
    {
        for(auto& it : children)
        {
            if(!it.second.status)
            {
                status = false;
                break;
            }
        }
    }
    return resu;
}

