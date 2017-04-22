#ifndef CELLMAP_H
#define CELLMAP_H

#include "definitions.h"

class CellMap
{
public:
	CellMap(Topology* topo);
	virtual ~CellMap();

	void init();
	void clear();

	void setCell(QVector3D pos, Cell* cell);
	void removeCell(QVector3D pos);
	void removeCellIfPresent(QVector3D pos, Cell* cell);
	Cell* getCell(QVector3D pos) const;
	inline Cell* getCellFast(IntVector2D const& intPos) const;

	//advanced functions
	CellClusterSet getNearbyClusters(QVector3D const& pos, qreal r) const;
	CellCluster* getNearbyClusterFast(QVector3D const& pos, qreal r, qreal minMass, qreal maxMass, CellCluster* exclude) const;
	using CellSelectFunction = bool(*)(Cell*);
	QList< Cell* > getNearbySpecificCells(QVector3D const& pos, qreal r, CellSelectFunction selection) const;

	void serializePrimitives(QDataStream& stream) const;
	void deserializePrimitives(QDataStream& stream, QMap< quint64, Cell* > const& oldIdCellMap);

private:
	void deleteCellMap();
	inline void removeCellIfPresent(int const &x, int const &y, Cell* cell);

	Topology* _topo = nullptr;
	Cell*** _cellGrid = nullptr;
	int _gridSize = 0;
};

Cell * CellMap::getCellFast(IntVector2D const& intPos) const
{
	return _cellGrid[intPos.x][intPos.y];
}

void CellMap::removeCellIfPresent(int const &x, int const &y, Cell* cell)
{
	if (_cellGrid[x][y] == cell)
		_cellGrid[x][y] = nullptr;
}

#endif //CELLMAP_H