/**
 * This represents a single chunk saved in memory.
 * It can be written to a file, and loaded and unloaded
 * from the world.
 */

#pragma once

#include <vector>
#include <utility>

#include <world/blocks.hpp>
#include <world/chunk.hpp>

class CachedChunk {

private:

	// This is the block data stored in a single chunk.
	// It is compressed using run-length encoding, so
	// there are 5 grass blocks in a row, then 3 dirt,
	// etc.
	std::vector< std::pair<
		int, // the number of blocks
		BlockType // the block type
	> > data;

public:

	// Read a chunk's data. This does NOT affect the chunk
	// itself.
	CachedChunk(Chunk&);

	// Write data into a chunk.
	void read(Chunk* into);

};
