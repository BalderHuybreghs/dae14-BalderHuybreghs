#include "pch.h"
#include "AlgoUtils.h"

// A very simple compression implementation
std::vector<char> AlgoUtils::Compress(const std::vector<char>& input)
{
  std::vector<char> compressed;

  for (size_t i = 0; i < input.size(); ++i) {
    char current = input[i];
    size_t count = 1;

    // Count consecutive occurrences of the same byte
    while (i + 1 < input.size() && input[i + 1] == current) {
      ++count;
      ++i;
    }

    // Write count and byte to compressed vector
    compressed.push_back(count);
    compressed.push_back(current);
  }

  return compressed;
}

std::vector<char> AlgoUtils::Decompress(const std::vector<char>& input)
{
  std::vector<char> decompressed;

  for (size_t i = 0; i < input.size(); i += 2) {
    char count = input[i];
    char byte = input[i + 1];

    // Repeat byte count times
    for (int j = 0; j < count; ++j) {
      decompressed.push_back(byte);
    }
  }

  return decompressed;
}