/**
 * Block types and whatnot.
 */

#pragma once

#include <graphics/texture.hpp>
#include <world/side.hpp>

enum BlockType {

	AIR_BLOCK,
	GRASS_BLOCK,
	DIRT_BLOCK,
	STONE_BLOCK,
	OAK_WOOD,
	OAK_LEAVES,

};

namespace blocks {

inline Texture get_texture(BlockType type, Side side) {
	switch (type) {
		case AIR_BLOCK:
			return INVALID_TEXTURE;
		case GRASS_BLOCK:
			switch (side) {
				case UP:
					return GRASS_TOP_TEXTURE;
				case DOWN:
					return DIRT_TEXTURE;
				default:
					return GRASS_SIDE_TEXTURE;
			}
		case DIRT_BLOCK:
			return DIRT_TEXTURE;
		case STONE_BLOCK:
			return STONE_TEXTURE;
		case OAK_WOOD:
			switch (side) {
				case UP:
				case DOWN:
					return OAK_WOOD_INNER_TEXTURE;
				default:
					return OAK_WOOD_SIDE_TEXTURE;
			}
		case OAK_LEAVES:
			return OAK_LEAVES_TEXTURE;
	}
	return INVALID_TEXTURE;
}

}
