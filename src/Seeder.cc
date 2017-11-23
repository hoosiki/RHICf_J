#include "Seeder.hh"
#include <random>
#include <iostream>
#include <functional>
#include <typeinfo>

Seeder::Seeder()
{
}

Seeder::~Seeder()
{
}

long Seeder::GetSeedForG4()
{
    std::random_device Seed;
    std::mt19937_64 Engine(Seed());
    std::uniform_int_distribution<int> Distribution(0,900000000);
    auto GetRMDForG4 = bind(Distribution, Engine);
    return GetRMDForG4();
}

int Seeder::GetSeedForPythia()
{
    std::random_device Seed;
    std::mt19937_64 Engine(Seed());
    std::uniform_int_distribution<int> Distribution(0,1000000000);
    auto GetRMDForPythia = bind(Distribution, Engine);
    return GetRMDForPythia();
}

int Seeder::GetSeedForCRMC()
{
    std::random_device Seed;
    std::mt19937_64 Engine(Seed());
    std::uniform_int_distribution<int> Distribution(1,900000000);
    auto GetRMDForCRMC = bind(Distribution, Engine);
    return GetRMDForCRMC();
}
