#ifndef CELLFEATURE_H
#define CELLFEATURE_H

#include <QDataStream>

#include "model/definitions.h"

class CellFeature
{
public:
    CellFeature (SimulationContext* context) : _context(context) {}
    virtual ~CellFeature ();

    void registerNextFeature (CellFeature* nextFeature);
    struct ProcessingResult {
        bool decompose;
        EnergyParticle* newEnergyParticle;
    };
    virtual ProcessingResult process (Token* token, Cell* cell, Cell* previousCell);

    virtual void serializePrimitives (QDataStream& stream) const;
    virtual void deserializePrimitives (QDataStream& stream);

protected:
    virtual ProcessingResult processImpl (Token* token, Cell* cell, Cell* previousCell) = 0;

    SimulationContext* _context = nullptr;
    CellFeature* _nextFeature = nullptr;

public:
    template< typename T >
    T* findObject ();
};


template< typename T >
T* CellFeature::findObject ()
{
    T* object = dynamic_cast< T* >(this);
    if( object )
        return object;
    else {
        if( _nextFeature )
            return _nextFeature->findObject<T>();
        else
            return 0;
    }
}

#endif // CELLFEATURE_H