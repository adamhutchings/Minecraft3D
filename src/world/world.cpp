#include "world.hpp"

#include <chrono>
#include <cmath>

#include <application.hpp>
#include <world/gen/generator.hpp>

void chunk_queue_update_function(World* world) {

	// Check and update loader status
	if (world->chunk_list_update_state != World::ChunkQueueUpdateState::S_FREE) return;
	world->chunk_list_update_state = World::ChunkQueueUpdateState::S_LOADER;

	// We'll be starting over from scratch in the list.
	world->chunks_to_load  .clear();
	world->chunks_to_unload.clear();

	auto& cam = global_app->player->camera;

	int x = ( (float) cam.x() ) / CHUNK_SIZE,
		y = ( (float) cam.y() ) / CHUNK_SIZE,
		z = ( (float) cam.z() ) / CHUNK_SIZE;

	int worldx = x - World::RENDER_DISTANCE / 2;
	int worldy = y - World::RENDER_DISTANCE / 2;
	int worldz = z - World::RENDER_DISTANCE / 2;

	// Calculating chunks to load
	// Only chunks in this cube have a chance of being loaded.
	for (int x = 0; x < World::RENDER_DISTANCE; ++x) {
		for (int y = 0; y < World::RENDER_DISTANCE; ++y) {
			for (int z = 0; z < World::RENDER_DISTANCE; ++z) {

				auto chunkpos = glm::vec3(worldx + x, worldy + y, worldz + z);
				
				// If this chunk is not in the list of loaded chunks, add to queue
				if ( world->loaded_chunks.find(chunkpos) == world->loaded_chunks.end() ) {
					world->chunks_to_load.push_back(chunkpos);
				}

			}
		}
	}

	// Calculating chunks to unload
	for (auto chunk : world->loaded_chunks) {

		bool should_be_loaded = true;

		should_be_loaded &= abs(x - chunk.first.x) < World::RENDER_DISTANCE / 2;
		should_be_loaded &= abs(y - chunk.first.y) < World::RENDER_DISTANCE / 2;
		should_be_loaded &= abs(z - chunk.first.z) < World::RENDER_DISTANCE / 2;

		if (!should_be_loaded) {
			world->chunks_to_unload.push_back(glm::vec3(x, y, z));
		}

	}

	world->chunk_list_update_state = World::ChunkQueueUpdateState::S_FREE;

}

World::World()
: generator           {}
, chunk_queue_updater {
	[this](){
		while (this->chunk_updater_running) {
			chunk_queue_update_function(this);
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
		}
	}
} {

}

Chunk* World::get_chunk_containing_coords(int x, int y, int z) {

	return get_chunk_at(

		std::floor( (float) x / CHUNK_SIZE ),
		std::floor( (float) y / CHUNK_SIZE ),
		std::floor( (float) z / CHUNK_SIZE )

	);

}

Chunk* World::get_chunk_at(int x, int y, int z) {

	glm::vec3 vec;

	if (x == cc_x && y == cc_y && z == cc_z && cached_chunk_exists) {
		goto end;
	}

	vec = glm::vec3(x, y, z);
	cc_x = x, cc_y = y, cc_z = z;
	cached_chunk_exists = true;

	if (loaded_chunks.find(vec) != loaded_chunks.end()) {
		cached_chunk = loaded_chunks[vec];
	} else {
		cached_chunk = nullptr;
	}

end:
	return cached_chunk;

}

bool World::get_block_at(int x, int y, int z, BlockType& block) {

	auto chunk = get_chunk_containing_coords(x, y, z);
	if (chunk == nullptr) return false;

	block = chunk->at(
		((x < 0) * 16 + x % CHUNK_SIZE) % CHUNK_SIZE,
		((y < 0) * 16 + y % CHUNK_SIZE) % CHUNK_SIZE,
		((z < 0) * 16 + z % CHUNK_SIZE) % CHUNK_SIZE
	);
	return true;

}

bool World::set_block_at(int x, int y, int z, BlockType block) {

	auto chunk = get_chunk_containing_coords(x, y, z);

	if (chunk != nullptr) {
		chunk->at(
			((x < 0) * 16 + x % CHUNK_SIZE) % CHUNK_SIZE,
			((y < 0) * 16 + y % CHUNK_SIZE) % CHUNK_SIZE,
			((z < 0) * 16 + z % CHUNK_SIZE) % CHUNK_SIZE
		) = block;
		chunk->update_mesh(this,
			std::floor( (float) x / CHUNK_SIZE ),
			std::floor( (float) y / CHUNK_SIZE ),
			std::floor( (float) z / CHUNK_SIZE )
		);
	}

	return chunk != nullptr;

}

void World::render() {
	
	for (auto cp : loaded_chunks) {
		cp.second->render();
	}

}

bool World::unload_chunk(int x, int y, int z) {

	auto vec = glm::vec3(x, y, z);

	auto chunk = this->get_chunk_at(x, y, z);
	if (chunk == nullptr) return false;

	// Check if the chunk is already unloaded.
	if (unloaded_chunks.find(vec) != unloaded_chunks.end()) {
		return false;
	}

	unloaded_chunks[vec] = new CachedChunk(chunk);
	delete chunk;
	loaded_chunks.erase(vec);

	if (cached_chunk_exists) {
		if (cc_x == x && cc_y == y && cc_z == z) {
			cached_chunk_exists = false;
		}
	}

	return true;

}

bool World::load_chunk(glm::vec3 vec) {

	if (this->get_chunk_at(vec.x, vec.y, vec.z) != nullptr) {
		// Chunk is already loaded.
		return false;
	}

	if (unloaded_chunks.find(vec) == unloaded_chunks.end()) {
		loaded_chunks[vec] = new Chunk(vec.x, vec.y, vec.z, generator);
	} else {
		loaded_chunks[vec] = new Chunk(unloaded_chunks[vec]);
		delete unloaded_chunks[vec];
		unloaded_chunks.erase(vec);
	}

	loaded_chunks[vec]->update_mesh(this, vec.x, vec.y, vec.z);

	// Check each side
	Chunk* chunk;
	if ( (chunk = this->get_chunk_at(vec.x + 1, vec.y, vec.z) ) ) {
		  chunk->update_mesh(  this, vec.x + 1, vec.y, vec.z);
	}
	if ( (chunk = this->get_chunk_at(vec.x - 1, vec.y, vec.z) ) ) {
		  chunk->update_mesh(  this, vec.x - 1, vec.y, vec.z);
	}
	if ( (chunk = this->get_chunk_at(vec.x, vec.y + 1, vec.z) ) ) {
		  chunk->update_mesh(  this, vec.x, vec.y + 1, vec.z);
	}
	if ( (chunk = this->get_chunk_at(vec.x, vec.y - 1, vec.z) ) ) {
		  chunk->update_mesh(  this, vec.x, vec.y - 1, vec.z);
	}
	if ( (chunk = this->get_chunk_at(vec.x, vec.y, vec.z + 1) ) ) {
		  chunk->update_mesh(  this, vec.x, vec.y, vec.z + 1);
	}
	if ( (chunk = this->get_chunk_at(vec.x, vec.y, vec.z - 1) ) ) {
		  chunk->update_mesh(  this, vec.x, vec.y, vec.z - 1);
	}

	return true;

}

void World::shutdown_update_thread() {
	chunk_updater_running = false;
	chunk_queue_updater.join();
}

void World::load_unload_one() {

	if (chunk_list_update_state != ChunkQueueUpdateState::S_FREE) return;
	chunk_list_update_state = ChunkQueueUpdateState::S_WORLD;

	if (!chunks_to_load.empty()) {
		glm::vec3 chunk = chunks_to_load[chunks_to_load.size() - 1];
		this->load_chunk(chunk);
		chunks_to_load.pop_back();
	}

	if (!chunks_to_unload.empty()) {
		glm::vec3 chunk = chunks_to_unload[chunks_to_unload.size() - 1];
		this->unload_chunk(chunk.x, chunk.y, chunk.z);
		chunks_to_unload.pop_back();
	}

	chunk_list_update_state = ChunkQueueUpdateState::S_FREE;

}
