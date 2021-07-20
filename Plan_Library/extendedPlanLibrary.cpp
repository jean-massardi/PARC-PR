#include "extendedPlanLibrary.h"

/*extendedPlanLibrary::extendedPlanLibrary()
{
    ePlanLibrary = new planLibrary();
}*/

extendedPlanLibrary::extendedPlanLibrary(planLibrary* _ePlanLibrary)
{
    ePlanLibrary = _ePlanLibrary;
        //add DM
    decisionModel = probabilityDistribution(*ePlanLibrary);

    //add noise
    noise = probabilityDistribution(ePlanLibrary->getTerminals(), 0.0);
}


extendedPlanLibrary::extendedPlanLibrary(const extendedPlanLibrary& epl)
{
    //if((*this) != epl)
    //{
        delete ePlanLibrary;
        ePlanLibrary = epl.ePlanLibrary;
        decisionModel = epl.decisionModel;
        noise = epl.noise;
    //}
}

extendedPlanLibrary::extendedPlanLibrary(bool rnd, float noisePRCT , int goal,int _size, int depth, int widthAND , int widthOR  , bool DM )
{
    ePlanLibrary = new planLibrary();
    //keep a track of ids
    int idPrim = 0;
    int idRule = 0;
    //assert(rnd);
    //add the control symbol for goals
    ePlanLibrary->addSymbol(-1,false,false);

    //add goals
    for(idPrim; idPrim < goal;idPrim++)
    {
        ePlanLibrary->addSymbol(idPrim,false,true);

        rule g = rule(-1,idRule);
        g.addChild(idPrim);
        idRule++;
        ePlanLibrary->addRule(g);

        for(int j = 0; j < widthOR; j++)
        {
            rule r = rule(idPrim,idRule);
            idRule++;

            for(int k =0; k < widthAND; k++)
            {
                r.addChild(rand() % _size + _size);
                for(int rSym = 0; rSym < k; rSym++)
                {
                    if(rand() % 3 == 2)
                    {
                        r.addConstraint(std::make_pair(rSym,k));
                    }
                }
            }

            ePlanLibrary->addRule(r);
        }

    }
    //add non terminal symbols
    idPrim = _size;
    for(int i =1; i<depth;i++)
    {
        for(idPrim; idPrim<_size*(i+1);idPrim++)
        {
            ePlanLibrary->addSymbol(idPrim,false,false);

            for(int j = 0; j < widthOR; j++)
            {
                rule r = rule(idPrim,idRule);
                idRule++;

                for(int k=0; k < widthAND; k++)
                {
                    r.addChild(rand() % _size + _size*(i+1));
                    for(int rSym = 0; rSym < k; rSym++)
                    {
                        if(rand() % 3 == 2)
                        {
                            r.addConstraint(std::make_pair(rSym,k));
                        }
                    }
                }

                ePlanLibrary->addRule(r);
            }

        }
    }


    //add terminal symbols
    for(idPrim;idPrim<_size*(depth+1);idPrim++)
    {
        ePlanLibrary->addSymbol(idPrim,true,false);
    }

    //add DM
    decisionModel = probabilityDistribution(*ePlanLibrary);

    //add noise
    noise = probabilityDistribution(ePlanLibrary->getTerminals(), noisePRCT);
}

extendedPlanLibrary::~extendedPlanLibrary()
{
    //delete ePlanLibrary;
}

