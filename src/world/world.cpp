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

	int camx = ( (float) cam.x() ) / CHUNK_SIZE,
		camy = ( (float) cam.y() ) / CHUNK_SIZE,
		camz = ( (float) cam.z() ) / CHUNK_SIZE;

	int worldx = camx - World::RENDER_DISTANCE / 2;
	int worldy = camy - World::RENDER_DISTANCE / 2;
	int worldz = camz - World::RENDER_DISTANCE / 2;

	// Calculating chunks to load
	// Only chunks in this cube have a chance of being loaded.
	for (int x = 0; x < World::RENDER_DISTANCE; ++x) {
		for (int y = 0; y < World::RENDER_DISTANCE; ++y) {
			for (int z = 0; z < World::RENDER_DISTANCE; ++z) {

				auto chunkpos = glm::vec3(worldx + x, worldy + y, worldz + z);
				
				// If this chunk is not in the list of loaded chunks, add to queue
				if ( world->loaded_chunks.count(chunkpos) == 0 ) {
					world->chunks_to_load.push_back(chunkpos);
				}

			}
		}
	}

	// Calculating chunks to unload
	for (auto chunk : world->loaded_chunks) {

		bool should_be_loaded = true;

		should_be_loaded &= abs(camx - chunk.first.x) < World::RENDER_DISTANCE / 2;
		should_be_loaded &= abs(camy - chunk.first.y) < World::RENDER_DISTANCE / 2;
		should_be_loaded &= abs(camz - chunk.first.z) < World::RENDER_DISTANCE / 2;

		if (!should_be_loaded) {
			world->chunks_to_unload.push_back(chunk.first);
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

	if (loaded_chunks.count(vec) > 0) {
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
		chunk->updated_since_last_frame = true;
	} else {
		this->bad_places[glm::vec3(x, y, z)] = block;
	}

	return chunk != nullptr;

}

void World::render() {
	
	for (auto cp : loaded_chunks) {
		if (cp.second->updated_since_last_frame) {
			cp.second->update_mesh(this,
				cp.first.x,
				cp.first.y,
				cp.first.z
			);
			cp.second->updated_since_last_frame = false;
		}
		if (cp.second->trees_generated_already)
			cp.second->render();
	}

}

void World::update_neighboring_chunks(glm::vec3 vec) {

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

}

bool World::unload_chunk(int x, int y, int z) {

	auto vec = glm::vec3(x, y, z);

	auto chunk = this->get_chunk_at(x, y, z);
	if (chunk == nullptr) return false;

	// Check if the chunk is already unloaded.
	if (unloaded_chunks.count(vec) > 0) {
		delete unloaded_chunks[vec];
	}

	unloaded_chunks[vec] = new CachedChunk(chunk);
	delete chunk;
	loaded_chunks.erase(vec);

	if (cached_chunk_exists) {
		if (cc_x == x && cc_y == y && cc_z == z) {
			cached_chunk_exists = false;
		}
	}

	update_neighboring_chunks(vec);

	return true;

}

bool World::load_chunk(glm::vec3 vec) {

	if (this->get_chunk_at(vec.x, vec.y, vec.z) != nullptr) {
		// Chunk is already loaded.
		return false;
	}

	if (unloaded_chunks.count(vec) == 0) {
		loaded_chunks[vec] = new Chunk(vec.x, vec.y, vec.z, generator, this);
	} else {
		loaded_chunks[vec] = new Chunk();
		unloaded_chunks[vec]->read(loaded_chunks[vec]);
		delete unloaded_chunks[vec];
		unloaded_chunks.erase(vec);
	}

	loaded_chunks[vec]->update_mesh(this, vec.x, vec.y, vec.z);

	// Check each side
	update_neighboring_chunks(vec);

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

#include <iostream>

void World::generate_trees() {

	for (auto cp : loaded_chunks) {
		// Check if chunks on all sides are loaded.
		// This doesn't matter for trees, but it will in the future.
		if (
			this->get_chunk_at(cp.first.x + 1, cp.first.y, cp.first.z) != nullptr &&
			this->get_chunk_at(cp.first.x - 1, cp.first.y, cp.first.z) != nullptr &&
			this->get_chunk_at(cp.first.x, cp.first.y + 1, cp.first.z) != nullptr &&
			this->get_chunk_at(cp.first.x, cp.first.y - 1, cp.first.z) != nullptr &&
			this->get_chunk_at(cp.first.x, cp.first.y, cp.first.z + 1) != nullptr &&
			this->get_chunk_at(cp.first.x, cp.first.y, cp.first.z - 1) != nullptr
		) {
			cp.second->generate_trees(this, cp.first);
		}
	}

}
