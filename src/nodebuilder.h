#pragma once

#include "yaml-cpp/nodebuilder.h"

#include <node.hpp>

namespace Asdf {
class Node;

class NodeBuilder : public YAML::NodeBuilder {
    public:
        Node Root(AsdfFile *file) {
          if (!m_pRoot)
            return Node();

          std::cout << "nodebuilder: " << file << std::endl;
          return Node(*m_pRoot, m_pMemory, file);
        };
};
}
