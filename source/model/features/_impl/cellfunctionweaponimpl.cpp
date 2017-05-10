#include "model/context/UnitContext.h"
#include "model/context/CellMap.h"
#include "model/entities/Cell.h"
#include "model/entities/CellCluster.h"
#include "model/entities/Token.h"
#include "model/context/SimulationParameters.h"
#include "model/Settings.h"

#include "CellFunctionWeaponImpl.h"

CellFunctionWeaponImpl::CellFunctionWeaponImpl (UnitContext* context)
    : CellFunction(context)
{
}

CellFeature::ProcessingResult CellFunctionWeaponImpl::processImpl (Token* token, Cell* cell, Cell* previousCell)
{
    ProcessingResult processingResult {false, 0};
	auto cellMap = _context->getCellMap();
	auto parameters = _context->getSimulationParameters();

	auto& tokenMem = token->getMemoryRef();
    tokenMem[Enums::Weapon::OUT] = Enums::WeaponOut::NO_TARGET;
    QVector2D pos = cell->getCluster()->calcPosition(cell);
    for(int x = -2; x < 3; ++x)
        for(int y = -2; y < 3; ++y) {
            QVector2D searchPos(pos.x()+x, pos.y()+y);
            Cell* otherCell = cellMap->getCell(searchPos);

            //other cell found?
            if( otherCell ) {
                if( otherCell->getCluster() != cell->getCluster() ) {
                    qreal energy = otherCell->getEnergy()*parameters->cellFunctionWeaponStrength+1.0;
                    if( otherCell->getEnergy() > energy ) {
                        otherCell->setEnergy(otherCell->getEnergy()-energy);
                        token->setEnergy(token->getEnergy() + energy/2.0);
                        cell->setEnergy(cell->getEnergy()+energy/2.0);
                        tokenMem[Enums::Weapon::OUT] = Enums::WeaponOut::STRIKE_SUCCESSFUL;
                    }
                }
            }
        }
    return processingResult;
}
