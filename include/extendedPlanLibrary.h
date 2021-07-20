#include "planLibrary.h"
#include "probabilityDistribution.h"
#include <utility>

#ifndef EXTENDEDPLANLIBRARY_H
#define EXTENDEDPLANLIBRARY_H


class extendedPlanLibrary
{
    public:
        //extendedPlanLibrary();
        extendedPlanLibrary(planLibrary* _ePlanLibrary);
        extendedPlanLibrary(const extendedPlanLibrary& epl);
        extendedPlanLibrary(bool rnd =true, float noisePRCT = 0.0, int goal = 5,int _size = 10, int depth = 2, int widthAND = 3, int widthOR = 2, bool DM = false);
        virtual ~extendedPlanLibrary();

        planLibrary* ePlanLibrary;
        probabilityDistribution decisionModel;
        probabilityDistribution noise;

        void Minimize();
};

#endif // EXTENDEDPLANLIBRARY_H
