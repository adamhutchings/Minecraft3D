#include "chunk_cache.hpp"

CachedChunk::CachedChunk(Chunk* chunk)
// Reasonable estimate for now
: data(100) {

	// Used in the inner loop
	int length;
	BlockType type;

	for (auto block : chunk->blocks) {

		if (block != type) {
			
			// Save the data in the buffer
			data.push_back( { length, type } );

			type = block;
			length = 0; // we always increment at the end of the loop

		}

		++length;

	}

	// These cached chunks can't be wasting memory.
	data.shrink_to_fit();

}

void CachedChunk::read(Chunk* into) {

	int index = 0; // index in the chunk

	for (auto run : this->data) {

		while (run.first--) {
			into->blocks[index++] = run.second;
		}

	}

}
