#include "BloomFilter.h"
#include "MurmurHash3.h"

BloomFilter::BloomFilter(int b, int hashes_no)
  : hashes_number(hashes_no)
  , bits(b)
  , bitvector(b, 0) {
}

std::array<uint64_t, 2> hash(Key k) {
  std::array<uint64_t, 2> result;
  MurmurHash3_x64_128(&k, sizeof(k), 0, result.data());
  return result;
}

uint64_t get_hash(uint64_t hash1, uint64_t hash2, int i, int bits) {
  return (hash1 + i * hash2) % (1<<bits);
}

void BloomFilter::add(Key k) {
  std::array<uint64_t, 2> h = hash(k);
  for (int i=0; i<hashes_number; i++) {
    bitvector[get_hash(h[0], h[1], i, bits)] = true;
  }
}

bool BloomFilter::possibly_contains(Key k) {
  std::array<uint64_t, 2> h = hash(k);
  for (int i=0; i<hashes_number; i++) {
    if (!bitvector[get_hash(h[0], h[1], i, bits)]) {
      return false;
    }
  }
  return true;
}
