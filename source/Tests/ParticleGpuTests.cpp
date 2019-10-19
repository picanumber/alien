#include "IntegrationGpuTestFramework.h"

class ParticleGpuTests
	: public IntegrationGpuTestFramework
{
public:
    ParticleGpuTests(
        IntVector2D const& universeSize = { 900, 600 },
        optional<ModelGpuData> const& modelData = boost::none)
        : IntegrationGpuTestFramework(universeSize, modelData)
    {}

	virtual ~ParticleGpuTests() = default;
};

namespace
{
    ModelGpuData getModelGpuDataWithOneBlock()
    {
        ModelGpuData result;
        result.setNumThreadsPerBlock(16);
        result.setNumBlocks(1);
        result.setNumClusterPointerArrays(1);
        result.setMaxClusters(100000);
        result.setMaxCells(500000);
        result.setMaxParticles(500000);
        result.setMaxTokens(50000);
        result.setMaxCellPointers(500000 * 10);
        result.setMaxClusterPointers(100000 * 10);
        result.setMaxParticlePointers(500000 * 10);
        result.setMaxTokenPointers(50000 * 10);
        result.setDynamicMemorySize(100000000);
        return result;
    }
}

class ParticleGpuWithOneBlockTests : public ParticleGpuTests
{
public:
    ParticleGpuWithOneBlockTests()
        : ParticleGpuTests({ 100, 100}, getModelGpuDataWithOneBlock())
    { }

    virtual ~ParticleGpuWithOneBlockTests() = default;
};

                                                                    
/**
* Situation: fusion of two particles
* Expected result: one particle remains with average velocity
*/
TEST_F(ParticleGpuTests, testFusionOfTwoParticles)
{
	DataDescription origData;
	auto particleEnergy = _parameters.cellMinEnergy / 3.0;

	auto particleId1 = _numberGen->getId();
	auto particle1 = ParticleDescription().setId(particleId1).setEnergy(particleEnergy).setPos({ 100, 100 }).setVel({ 0.5, 0.0 });
	origData.addParticle(particle1);

	auto particleId2 = _numberGen->getId();
	auto particle2 = ParticleDescription().setId(particleId2).setEnergy(particleEnergy).setPos({ 110, 100 }).setVel({ -0.5, 0.0 });
	origData.addParticle(particle2);

	IntegrationTestHelper::updateData(_access, origData);
	IntegrationTestHelper::runSimulation(30, _controller);

	IntRect rect = { { 0, 0 },{ _universeSize.x, _universeSize.y } };
	DataDescription newData = IntegrationTestHelper::getContent(_access, rect);

	ASSERT_FALSE(newData.clusters);
	ASSERT_EQ(1, newData.particles->size());
	auto newParticle = newData.particles->front();
	EXPECT_TRUE(isCompatible(QVector2D(0, 0), *newParticle.vel));

	checkEnergy(origData, newData);
}

/**
* Situation: fusion of many particles
* Expected result: energy balance is fulfilled
*/
TEST_F(ParticleGpuTests, testFusionOfManyParticles)
{
	auto particleEnergy = _parameters.cellMinEnergy / 120.0;

	DataDescription origData;
	for (int i = 0; i < 100; ++i) {
		origData.addParticle(
			ParticleDescription().setId(_numberGen->getId()).setEnergy(particleEnergy).setPos({ 100, 100 }).setVel({ 0.5, 0.0 }));
	}

	IntegrationTestHelper::updateData(_access, origData);
	IntegrationTestHelper::runSimulation(300, _controller);
	DataDescription newData = IntegrationTestHelper::getContent(_access, { { 0, 0 },{ _universeSize.x, _universeSize.y } });

	checkEnergy(origData, newData);
}

/**
* Situation: particle with high energy
* Expected result: particle transforms to cell
*/
TEST_F(ParticleGpuTests, testTransformationParticleToCell)
{
    auto size = _spaceProp->getSize();
    DataDescription origData;
    float cellMinEnergy = static_cast<float>(_parameters.cellMinEnergy);
    origData.addParticle(ParticleDescription().setId(_numberGen->getId()).setPos({ 0, 0 }).setVel({ 0.5, 0 }).setEnergy(cellMinEnergy * 2));

    IntegrationTestHelper::updateData(_access, origData);
    IntegrationTestHelper::runSimulation(100, _controller);
    DataDescription newData = IntegrationTestHelper::getContent(_access, { { 0, 0 },{ _universeSize.x, _universeSize.y } });

    ASSERT_EQ(1, newData.clusters->size());
    EXPECT_EQ(1, newData.clusters->at(0).cells->size());

    checkEnergy(origData, newData);
}

/**
* Situation: many random particles and one 10x10 cluster
* Fixed error: wrong block partition in ParticleProcessor::updateMap_gridCall led to dead cells in particle map
* Expected result: energy balance fulfilled
*/
TEST_F(ParticleGpuWithOneBlockTests, regressionTestFusionOfManyParticles_oneThreadBlock)
{
    _parameters.radiationProb = 0;  //exclude radiation
    _parameters.cellTransformationProb = 0;
    _context->setSimulationParameters(_parameters);

    DataDescription origData;
    for (int i = 0; i < 10000; ++i) {
        auto const particle = ParticleDescription()
                                  .setId(_numberGen->getId())
                                  .setEnergy(3)
                                  .setPos(
                                      {static_cast<float>(_numberGen->getRandomReal(0, _universeSize.x)),
                                       static_cast<float>(_numberGen->getRandomReal(0, _universeSize.y))})
                                  .setVel(
                                      {static_cast<float>(_numberGen->getRandomReal(-1, 1)),
                                       static_cast<float>(_numberGen->getRandomReal(-1, 1))});
        origData.addParticle(particle);
    }
    origData.addCluster(createRectangularCluster({ 10, 10 }, QVector2D{ 0, 0 }, QVector2D{}));

    IntegrationTestHelper::updateData(_access, origData);
    IntegrationTestHelper::runSimulation(1000, _controller);

    IntRect rect = {{0, 0}, {_universeSize.x, _universeSize.y}};
    DataDescription newData = IntegrationTestHelper::getContent(_access, rect);

    checkEnergy(origData, newData);
}


