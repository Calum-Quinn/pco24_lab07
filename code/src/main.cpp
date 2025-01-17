#include "OurModel.h"
#include "pcomodelchecker.h"

int main(int /*argc*/, char */*argv*/[])
{
    {
        OurModel model;
        PcoModelChecker checker;
        checker.setModel(&model);
        checker.run();
    }


    return 0;
}
