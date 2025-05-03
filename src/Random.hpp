#include <random>
#include <cstdint> //for unsigned or signed 64b int
#include <cmath> //for rounding


inline static std::mt19937_64 engine{std::random_device{}()}; //gets rand seed by creating a 64-bit Mersenne-Twister engine(i looked this up hehe - Kylie)

//gets rand int
inline uint64_t uniform_mod(uint64_t q) {
    std::uniform_int_distribution<uint64_t> dist(0, q-1); //make sure equal probability
    return dist(engine); //calls gen and returns rand 64 bit int
  }

  
//gets rand int with error
 inline int64_t sample_error(double sigma) { //sigmas(lol) the deviation
    std::normal_distribution<double> dist(0.0, sigma); //changes numbers by sigma(lol)
    return std::llround(dist(engine));//rounds and rets "integer Gaussian"
  }
  