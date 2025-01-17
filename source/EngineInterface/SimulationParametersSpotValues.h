#pragma once

struct SimulationParametersSpotValues
{
    float friction = 0.001f;
    float rigidity = 0.0f;
    float radiationFactor = 0.0002f;
    float cellMaxForce = 0.8f;
    float cellMinEnergy = 50.0f;

    float cellBindingForce = 1.0f;
    float cellFusionVelocity = 0.4f;
    float cellMaxBindingEnergy = 500000.0f;

    float tokenMutationRate = 0;
    float cellMutationRate = 0;
    int cellFunctionMinInvocations = 40000;
    float cellFunctionInvocationDecayProb = 0;
    float cellFunctionWeaponEnergyCost = 0.2f;
    float cellFunctionWeaponColorTargetMismatchPenalty = 0.0f;
    float cellFunctionWeaponColorDominance = 0.0f;
    float cellFunctionWeaponGeometryDeviationExponent = 0.0f;
    float cellFunctionWeaponConnectionsMismatchPenalty = 0.33f;
    float cellFunctionWeaponTokenPenalty = 0.0f;

    bool operator==(SimulationParametersSpotValues const& other) const
    {
        return friction == other.friction && rigidity == other.rigidity && radiationFactor == other.radiationFactor
            && cellMaxForce == other.cellMaxForce && cellMinEnergy == other.cellMinEnergy
            && cellBindingForce == other.cellBindingForce && cellFusionVelocity == other.cellFusionVelocity
            && tokenMutationRate == other.tokenMutationRate && cellMutationRate == other.cellMutationRate
            && cellFunctionWeaponEnergyCost == other.cellFunctionWeaponEnergyCost
            && cellFunctionWeaponColorDominance == other.cellFunctionWeaponColorDominance
            && cellFunctionWeaponColorTargetMismatchPenalty == other.cellFunctionWeaponColorTargetMismatchPenalty
            && cellFunctionWeaponGeometryDeviationExponent == other.cellFunctionWeaponGeometryDeviationExponent
            && cellMaxBindingEnergy == other.cellMaxBindingEnergy && cellFunctionMinInvocations == other.cellFunctionMinInvocations
            && cellFunctionInvocationDecayProb == other.cellFunctionInvocationDecayProb
            && cellFunctionWeaponTokenPenalty == other.cellFunctionWeaponTokenPenalty
            && cellFunctionWeaponConnectionsMismatchPenalty == other.cellFunctionWeaponConnectionsMismatchPenalty;
    }
};
