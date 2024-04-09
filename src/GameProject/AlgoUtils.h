#pragma once
#include <vector>

// General utilities for algorithms
namespace AlgoUtils
{
  // Hashing of a pair, used in the tilemap implementation
  struct PairHash
  {
    template <class T1, class T2> // Template class telling t1 and t2 can be of any type, allowing for any pair to work
    std::size_t operator()(const std::pair<T1, T2>& p) const
    {
      size_t hash1 = std::hash<T1>{}(p.first);
      size_t hash2 = std::hash<T2>{}(p.second);
      return hash1 ^ hash2; // Combine the hash values
    }
  };

  // Compression of a binary vec
  std::vector<char> Compress(const std::vector<char>& input);
  std::vector<char> Decompress(const std::vector<char>& input);
};

