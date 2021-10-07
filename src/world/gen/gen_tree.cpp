#include <world/gen/gen_tree.hpp>

void place_tree(World * world, glm::vec3 b) {

    // Place a halo of leaves
    for (int x = -2; x <= 2; ++x) {
        for (int y = 2; y <= 5; ++y) {
            for (int z = -2; z <= 2; ++z) {
                world->set_block_at(
                    b.x + x, b.y + y, b.z + z, BlockType::OAK_LEAVES
                );
            }
        }
    }

    // Place the wood
    for (int i = 1; i < 5; ++i)
        world->set_block_at(b.x, b.y + i, b.z, BlockType::OAK_WOOD);

}
