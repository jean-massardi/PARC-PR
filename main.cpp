
#include <iostream>
#include <fstream>
#include <ctime>
#include "extendedPlanLibrary.h"
#include "solver.h"
#include "testSolver.h"

using namespace std;

map<int,vector<int>> testFO()
{
    map<int,vector<int>> resu;

    for(int nbPart = 50; nbPart <= 500; nbPart+=50)
    {
        cout << nbPart << " ";
    }
    cout << endl;

    for(int t =0; t < 100; t++)
    {
        //cout << "TEST : " << t <<endl;
        extendedPlanLibrary ePL = extendedPlanLibrary();
        solver gP = solver(&ePL,1);
        pair<int,vector<int>> curPlan = gP.generatePlan();
        for(int nbPart = 50; nbPart <= 500; nbPart+=50)
        {
            resu[nbPart] = vector<int>();
            solver s = solver(&ePL,nbPart);
            bool found = false;
            for(int obs =0 ; obs < curPlan.second.size(); obs++)
            {
                if(s.getMaxGoal() == curPlan.first)
                {
                    resu[nbPart].push_back(obs);
                    found = true;
                    break;
                }
                s.addObservation(curPlan.second[obs]);

            }
            if(!found)
            {
                resu[nbPart].push_back(-1);
            }
            cout << resu[nbPart][resu[nbPart].size()-1] << " ";
        }
        cout << endl;
    }
    return resu;

}

map<int,vector<int>> testOver()
{
    map<int,vector<int>> resu;

    for(int t =0; t < 100; t++)
    {
            extendedPlanLibrary ePL = extendedPlanLibrary(true,20);
            solver gP = solver(&ePL,500);
            pair<int,vector<int>> curPlan = gP.generatePlanFO();
            solver s = solver(&ePL, 500);
            bool found = false;
            for(int obs =0 ; obs < curPlan.second.size(); obs++)
            {
                if(s.getMaxGoal() == curPlan.first)
                {
                    cout << obs << " ";
                    found = true;
                    break;
                }
                s.addObservation(curPlan.second[obs]);
            }
            if(!found)
            {
                cout << -1 << " ";
            }

        cout << endl;

    }

    return resu;

}

map<int,vector<int>> testUnder()
{
    map<int,vector<int>> resu;

    for(int t =0; t < 100; t++)
    {
            extendedPlanLibrary ePL = extendedPlanLibrary(true,10);
            solver gP = solver(&ePL,500);
            pair<int,vector<int>> curPlan = gP.generatePlan();
            int rnd = rand() % curPlan.second.size();
            curPlan.second[rnd] = rand() % 200 + 300;
            solver s = solver(&ePL, 500);
            bool found = false;
            for(int obs =0 ; obs < curPlan.second.size(); obs++)
            {
                if(s.getMaxGoal() == curPlan.first)
                {
                    cout << obs << " ";
                    found = true;
                    break;
                }
                s.addObservation(curPlan.second[obs]);
            }
            if(!found)
            {
                cout << -1 << " ";
            }

        cout << endl;

    }

    return resu;

}

map<int,vector<int>> testPO(int nbPart)
{
    map<int,vector<int>> resu;

    for(float p = 0.1; p < 0.4; p+=0.1)
    {
        cout << p << " ";
    }
    cout << endl;

    for(int t =0; t < 100; t++)
    {
        for(float p = 0.1; p < 0.4; p+=0.1)
        {
            extendedPlanLibrary ePL = extendedPlanLibrary(true,p);
            solver gP = solver(&ePL,500);
            pair<int,vector<int>> curPlan = gP.generatePlan();
            solver s = solver(&ePL, nbPart);
            bool found = false;
            for(int obs =0 ; obs < curPlan.second.size(); obs++)
            {
                if(s.getMaxGoal() == curPlan.first)
                {
                    cout << obs << " ";
                    found = true;
                    break;
                }
                s.addObservation(curPlan.second[obs]);
            }
            if(!found)
            {
                cout << -1 << " ";
            }
        }
        cout << endl;

    }

    return resu;
}

int main()
{
    /*clock_t begin = clock();

    map<int,vector<int>> TFO = testFO();
    clock_t end = clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;

    cout << "TIME :"<<elapsed_secs/100.0<< endl;*/
    /*extendedPlanLibrary ePL = extendedPlanLibrary();
    ofstream myfile ("example.txt");
    if (myfile.is_open())
    {
        myfile << ePL.ePlanLibrary->toString();
        myfile.close();
    }
    else cout << "Unable to open file";
    cout << ePL.ePlanLibrary->toString()<<endl;*/
    std::ifstream in("example.txt");
    std::string contents((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
    planLibrary PL = planLibrary(contents.c_str());

    extendedPlanLibrary ePL = extendedPlanLibrary(&PL);

    solver gP = solver(&ePL,1);
    pair<int,vector<int>> curPlan = gP.generatePlan();

     ofstream myfile ("list_plan.txt");
    if (myfile.is_open())
    {
        for(int i =0; i<200;i++)
        {
            myfile << curPlan.first << " : ";
            for(auto it: curPlan.second)
            {
                 myfile << it << " ";
            }
            myfile <<endl;
            curPlan = gP.generatePlan();
        }

        myfile.close();
    }
    else cout << "Unable to open file";


    //cout << PL.toString() << endl;
    //cout << "Hello World" << endl;
    return 0;
}
