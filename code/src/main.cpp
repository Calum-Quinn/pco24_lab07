#include "OurModel.h"
#include "pcomodelchecker.h"

#include <pcosynchro/pcologger.h>

int main(int argc, char *argv[])
{
    {
        logger().initialize(argc, argv);
        OurModel model;
        PcoModelChecker checker;
        checker.setModel(&model);
        checker.run();
    }


    return 0;
}
