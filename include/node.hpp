#pragma once

#include <yaml-cpp/yaml.h>
#include "yaml-cpp/node/node.h"
#include "yaml-cpp/node/iterator.h"
#include "yaml-cpp/node/detail/memory.h"
#include "yaml-cpp/node/detail/node.h"
#include "yaml-cpp/exceptions.h"

#include <asdffile.hpp>

namespace Asdf {

class Node : YAML::Node
{
    public:
        friend class NodeBuilder;
        inline Node() : YAML::Node() {}

    private:
        bool m_isValid;
        mutable YAML::detail::shared_memory_holder m_pMemory;
        mutable YAML::detail::node* m_pNode;

        inline Node(YAML::detail::node& node, YAML::detail::shared_memory_holder pMemory)
            : m_isValid(true), m_pMemory(pMemory), m_pNode(&node) {}
};

} /* namespace Asdf */
