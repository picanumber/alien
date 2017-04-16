#include "model/entities/token.h"
#include "model/entities/cell.h"
#include "model/features/cellfeatureconstants.h"
#include "model/config.h"
#include "model/simulationcontext.h"
#include "model/simulationparameters.h"

#include "energyguidanceimpl.h"


EnergyGuidanceImpl::EnergyGuidanceImpl (SimulationContext* context)
    : EnergyGuidance(context), _parameters(context->getSimulationParameters())
{

}

CellFeature::ProcessingResult EnergyGuidanceImpl::processImpl (Token* token, Cell* cell, Cell* previousCell)
{
    ProcessingResult processingResult {false, 0};
	auto& tokenMem = token->getMemoryRef();
	quint8 cmd = tokenMem[Enums::EnergyGuidance::IN] % 6;
    qreal valueCell = static_cast<quint8>(tokenMem[Enums::EnergyGuidance::IN_VALUE_CELL]);
    qreal valueToken = static_cast<quint8>(tokenMem[Enums::EnergyGuidance::IN_VALUE_TOKEN]);
    qreal amount = 10.0;

    if( cmd == Enums::EnergyGuidanceIn::BALANCE_CELL ) {
        if( cell->getEnergy() > (_parameters->CRIT_CELL_TRANSFORM_ENERGY+valueCell) ) {
            cell->setEnergy(cell->getEnergy()-amount);
            token->setEnergy(token->getEnergy() + amount);
        }
        if( cell->getEnergy() < (_parameters->CRIT_CELL_TRANSFORM_ENERGY+valueCell) ) {
            if( token->getEnergy() > (_parameters->MIN_TOKEN_ENERGY+valueToken+amount) ) {
                cell->setEnergy(cell->getEnergy()+amount);
				token->setEnergy(token->getEnergy() - amount);
            }
        }
    }
    if( cmd == Enums::EnergyGuidanceIn::BALANCE_TOKEN ) {
        if( token->getEnergy() > (_parameters->MIN_TOKEN_ENERGY+valueToken) ) {
            cell->setEnergy(cell->getEnergy()+amount);
			token->setEnergy(token->getEnergy() - amount);
		}
        if( token->getEnergy() < (_parameters->MIN_TOKEN_ENERGY+valueToken) ) {
            if( cell->getEnergy() > (_parameters->CRIT_CELL_TRANSFORM_ENERGY+valueCell+amount) ) {
                cell->setEnergy(cell->getEnergy()-amount);
				token->setEnergy(token->getEnergy() + amount);
			}
        }
    }
    if( cmd == Enums::EnergyGuidanceIn::BALANCE_BOTH ) {
        if( (token->getEnergy() > (_parameters->MIN_TOKEN_ENERGY+valueToken+amount))
                && (cell->getEnergy() < (_parameters->CRIT_CELL_TRANSFORM_ENERGY+valueCell)) ) {
            cell->setEnergy(cell->getEnergy()+amount);
			token->setEnergy(token->getEnergy() - amount);
		}
        if( (token->getEnergy() < (_parameters->MIN_TOKEN_ENERGY+valueToken))
                && (cell->getEnergy() > (_parameters->CRIT_CELL_TRANSFORM_ENERGY+valueCell+amount)) ) {
            cell->setEnergy(cell->getEnergy()-amount);
			token->setEnergy(token->getEnergy() + amount);
		}
    }
    if( cmd == Enums::EnergyGuidanceIn::HARVEST_CELL ) {
        if( cell->getEnergy() > (_parameters->CRIT_CELL_TRANSFORM_ENERGY+valueCell+amount) ) {
            cell->setEnergy(cell->getEnergy()-amount);
			token->setEnergy(token->getEnergy() + amount);
		}
    }
    if( cmd == Enums::EnergyGuidanceIn::HARVEST_TOKEN ) {
        if( token->getEnergy() > (_parameters->MIN_TOKEN_ENERGY+valueToken+amount) ) {
            cell->setEnergy(cell->getEnergy()+amount);
			token->setEnergy(token->getEnergy() - amount);
		}
    }
    return processingResult;
}