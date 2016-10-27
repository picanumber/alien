#ifndef ALIENENERGY_H
#define ALIENENERGY_H

#include <QVector3D>

class AlienCellCluster;
class AlienGrid;
class AlienEnergy
{
public:
    AlienEnergy (qreal amount_, QVector3D pos_, QVector3D vel_, AlienGrid*& grid);
    AlienEnergy (QDataStream& stream, QMap< quint64, AlienEnergy* >& oldIdEnergyMap, AlienGrid*& grid);

    bool movement (AlienCellCluster*& cluster);

    void serialize (QDataStream& stream);

private:
    AlienGrid*& _grid;

public:
    qreal amount;
    QVector3D pos;
    QVector3D vel;
    quint64 id;
    quint8 color;
};

#endif // ALIENENERGY_H