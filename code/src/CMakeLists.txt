set(SRC_FILES
    analyzerwatchdog.cpp
    observablethread.cpp
    pcoconcurrencyanalyzer.cpp
    pcomodelchecker.cpp
    pcomodel.cpp
    scenariobuilder.cpp
    scenario.cpp
    main.cpp
    ReaderWriterLock.cpp
)

set(HEADER_FILES
    analyzerwatchdog.h
    observablethread.h
    pcoconcurrencyanalyzer.h
    pcomodelchecker.h
    pcomodel.h
    scenariobuilder.h
    scenario.h
    OurModel.h
    abstractreaderwriter.h
    ConcreteReaderWriter.h
    ReaderWriterChecker.h
)

add_library(modelchecking_lib ${SRC_FILES} ${HEADER_FILES})

target_include_directories(modelchecking_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
