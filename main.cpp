#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <tuple>
#include <format>

int generateRandomNumber(int min, int max) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(min, max);
    return dis(gen);
}

struct Particle {
    int x;
    int y;
    int z;
    int mass;
};

enum class Type
{
    ArrayOfStructs,
    StructOfArrays,
};

template<Type type>
struct Particles {};

template<>
struct Particles<Type::ArrayOfStructs>
{
    std::vector<Particle> particles;
    
    Particles(int count)
    {
        particles.reserve(count);
        for (int i = 0; i < count; i++)
        {
            Particle p;
            p.x = generateRandomNumber(0, 100);
            p.y = generateRandomNumber(0, 100);
            p.z = generateRandomNumber(0, 100);
            p.mass = generateRandomNumber(0, 100);
            particles.push_back(p);
        }
    }

    long long total_mass()
    {
        long long total = 0;
        for (const auto& p : particles)
        {
            total += p.mass;
        }
        return total;
    }

    std::tuple<long long, long long, long long> average_position()
    {
        long long x = 0;
        long long y = 0;
        long long z = 0;
        for (const auto& p : particles)
        {
            x += p.x;
            y += p.y;
            z += p.z;
        }
        return {x / particles.size(), y / particles.size(), z / particles.size()};
    }

    std::tuple<long long, long long, long long> center_of_mass()
    {
        long long x = 0;
        long long y = 0;
        long long z = 0;
        long long total = 0;
        for (const auto& p : particles)
        {
            x += p.x * p.mass;
            y += p.y * p.mass;
            z += p.z * p.mass;
            total += p.mass;
        }
        return {x / total, y / total, z / total};
    }
};

template<>
struct Particles<Type::StructOfArrays>
{
    std::vector<int> x;
    std::vector<int> y;
    std::vector<int> z;
    std::vector<int> mass;

    Particles(int count)
    {
        x.reserve(count);
        y.reserve(count);
        z.reserve(count);
        mass.reserve(count);
        for (int i = 0; i < count; i++)
        {
            x.push_back(generateRandomNumber(0, 100));
            y.push_back(generateRandomNumber(0, 100));
            z.push_back(generateRandomNumber(0, 100));
            mass.push_back(generateRandomNumber(0, 100));
        }
    }

    long long total_mass()
    {
        long long total = 0;
        for (int i = 0; i < mass.size(); i++)
        {
            total += mass[i];
        }
        return total;
    }

    std::tuple<long long, long long, long long> average_position()
    {
        long long x = 0;
        long long y = 0;
        long long z = 0;
        for (int i = 0; i < mass.size(); i++) x += this->x[i];
        for (int i = 0; i < mass.size(); i++) y += this->y[i];
        for (int i = 0; i < mass.size(); i++) z += this->z[i];

        return {x / mass.size(), y / mass.size(), z / mass.size()};
    }

    std::tuple<long long, long long, long long> center_of_mass()
    {
        long long x = 0;
        long long y = 0;
        long long z = 0;
        long long total = 0;
        for (int i = 0; i < mass.size(); i++)
        {
            x += this->x[i] * mass[i];
            y += this->y[i] * mass[i];
            z += this->z[i] * mass[i];
            total += mass[i];
        }

        return {x / total, y / total, z / total};
    }
};

template<Type type>
void benchmark(int particle_count, int iteration_count)
{
    Particles<type> particles(particle_count);

    auto total_mass_time = std::chrono::nanoseconds(0);
    auto average_position_time = std::chrono::nanoseconds(0);
    auto center_of_mass_time = std::chrono::nanoseconds(0);

    volatile long long total_mass, x, y, z;
    
    for (int i = 0; i < iteration_count; i++)
    {
        auto start = std::chrono::high_resolution_clock::now();
        total_mass = particles.total_mass();
        total_mass_time += std::chrono::high_resolution_clock::now() - start;

        start = std::chrono::high_resolution_clock::now();
        auto average_position = particles.average_position();
        x = std::get<0>(average_position);
        y = std::get<1>(average_position);
        z = std::get<2>(average_position);
        average_position_time += std::chrono::high_resolution_clock::now() - start;

        start = std::chrono::high_resolution_clock::now();
        auto center_of_mass = particles.center_of_mass();
        x = std::get<0>(center_of_mass);
        y = std::get<1>(center_of_mass);
        z = std::get<2>(center_of_mass);
        center_of_mass_time += std::chrono::high_resolution_clock::now() - start;
    }

    total_mass_time /= iteration_count;
    average_position_time /= iteration_count;
    center_of_mass_time /= iteration_count;

    std::cout << std::format(
        "Total mass time:       {:>6}\n"
        "Average position time: {:>6}\n"
        "Center of mass time:   {:>6}\n\n",
        std::chrono::duration_cast<std::chrono::microseconds>(total_mass_time),
        std::chrono::duration_cast<std::chrono::microseconds>(average_position_time),
        std::chrono::duration_cast<std::chrono::microseconds>(center_of_mass_time)
    );
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <particle_count> <iteration_count>" << std::endl;
        return 1;
    }

    const int PARTICLE_COUNT  = std::stoi(argv[1]);
    const int ITERATION_COUNT = std::stoi(argv[2]);

    std::cout << "Array of structs\n\n";
    benchmark<Type::ArrayOfStructs>(PARTICLE_COUNT, ITERATION_COUNT);

    std::cout << "Struct of arrays\n\n";
    benchmark<Type::StructOfArrays>(PARTICLE_COUNT, ITERATION_COUNT);
    
    return 0;
}