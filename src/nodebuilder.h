#pragma once

#include "yaml-cpp/nodebuilder.h"

#include <node.hpp>

namespace Asdf {
class Node;

class NodeBuilder : public YAML::NodeBuilder {
    public:
        Node Root() {
          if (!m_pRoot)
            return Node();

          return Node(*m_pRoot, m_pMemory);
        };
};
}
