#ifndef MAPCOMPARTMENTIMPL_H
#define MAPCOMPARTMENTIMPL_H

#include "model/context/mapcompartment.h"

class MapCompartmentImpl
	: public MapCompartment
{
public:
	virtual ~MapCompartmentImpl() {}

	virtual void init(Topology* topology, IntRect mapRect) override;

	virtual void registerNeighborContext(RelativeLocation location, SimulationUnitContext* context) override;
	virtual SimulationUnitContext* getNeighborContext(IntVector2D pos) const override;

private:
};

#endif // MAPCOMPARTMENTIMPL_H
