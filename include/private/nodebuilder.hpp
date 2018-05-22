#pragma once

#include "yaml-cpp/nodebuilder.h"

#include <node.hpp>

namespace Asdf {
class Node;

/*
 * Override YAML::NodeBuilder in order to allow construction of Asdf Node
 * objects that contain references to a top-level AsdfFile object. This is
 * necessary in order to enable block allocation for NDArrays while supporting
 * a reasonable API for creating new nodes.
 */
class NodeBuilder : public YAML::NodeBuilder {
    public:
        Node Root(AsdfFile *file) {
          if (!m_pRoot)
            return Node();

          return Node(*m_pRoot, m_pMemory, file);
        };
};
}
