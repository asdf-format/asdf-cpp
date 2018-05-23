#include <asdf-cpp/node.hpp>

namespace Asdf {

const AsdfFile *Node::get_asdf_file() const
{
    return file;
}

} /* end namespace Asdf */
