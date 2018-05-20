#include <private/block.hpp>
#include <private/block_manager.hpp>

namespace Asdf {

void BlockManager::write_blocks(std::ostream &ostream) const
{
    for (auto b : blocks)
    {
        b->write(ostream);
    }
}

} /* end namespace Asdf */
