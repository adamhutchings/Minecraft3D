#include <world/gen/gen_tree.hpp>

void place_tree(World * world, glm::vec3 b) {

    // Place a halo of leaves
    for (int x = -2; x <= 2; ++x) {
        for (int y = 3; y <= 5; ++y) {
            for (int z = -2; z <= 2; ++z) {
                world->set_block_at(
                    b.x + x, b.y + y, b.z + z, BlockType::OAK_LEAVES
                );
            }
        }
    }

    // Add the very top leaves - which are slightly narrower than other layers
    for (int x = -1; x <= 1; ++x) {
        for (int z = -1; z <= 1; ++z) {
            world->set_block_at(
                b.x + x, b.y + 6, b.z + z, BlockType::OAK_LEAVES
            );
        }
    }

    // Place the wood
    for (int i = 1; i < 5; ++i)
        world->set_block_at(b.x, b.y + i, b.z, BlockType::OAK_WOOD);

}
