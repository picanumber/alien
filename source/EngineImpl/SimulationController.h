#pragma once

#include <thread>

#include "EngineInterface/Definitions.h"
#include "EngineInterface/GeneralSettings.h"
#include "EngineInterface/SymbolMap.h"
#include "EngineWorker.h"

#include "Definitions.h"

class _SimulationController
{
public:

    ENGINEIMPL_EXPORT void initCuda();

    ENGINEIMPL_EXPORT void newSimulation(
        int timestep,
        GeneralSettings const& generalSettings,
        SimulationParameters const& parameters,
        SymbolMap const& symbolMap);
    ENGINEIMPL_EXPORT void clear();

    ENGINEIMPL_EXPORT void registerImageResource(GLuint image);

    ENGINEIMPL_EXPORT void getVectorImage(
        RealVector2D const& rectUpperLeft,
        RealVector2D const& rectLowerRight,
        IntVector2D const& imageSize,
        double zoom);

    ENGINEIMPL_EXPORT DataDescription
    getSimulationData(IntVector2D const& rectUpperLeft, IntVector2D const& rectLowerRight);

    ENGINEIMPL_EXPORT void updateData(DataChangeDescription const& dataToUpdate);

    ENGINEIMPL_EXPORT void calcSingleTimestep();
    ENGINEIMPL_EXPORT void runSimulation();
    ENGINEIMPL_EXPORT void pauseSimulation();

    ENGINEIMPL_EXPORT bool isSimulationRunning() const;

    ENGINEIMPL_EXPORT void closeSimulation();

    ENGINEIMPL_EXPORT uint64_t getCurrentTimestep() const;
    ENGINEIMPL_EXPORT void setCurrentTimestep(uint64_t value);

    ENGINEIMPL_EXPORT SimulationParameters getSimulationParameters() const;
    ENGINEIMPL_EXPORT GeneralSettings getGeneralSettings() const;
    ENGINEIMPL_EXPORT IntVector2D getWorldSize() const;
    ENGINEIMPL_EXPORT SymbolMap getSymbolMap() const;

    ENGINEIMPL_EXPORT boost::optional<int> getTpsRestriction() const;
    ENGINEIMPL_EXPORT void setTpsRestriction(boost::optional<int> const& value);

    ENGINEIMPL_EXPORT int getTps() const;

private:
    GeneralSettings _generalSettings;
    SimulationParameters _parameters;
    SymbolMap _symbolMap;

    EngineWorker _worker;
    std::thread* _thread = nullptr;
};
