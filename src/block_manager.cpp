#include <private/block_manager.hpp>

namespace Asdf {

void BlockManager::write_blocks(std::ostream &ostream) const
{
    ostream << "HERE BE BLOCKS" << std::endl;
    for (auto b : blocks)
    {
        b->write(ostream);
    }
}

} /* end namespace Asdf */
