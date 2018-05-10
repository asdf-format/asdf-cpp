#pragma once

#include <vector>

#include "yaml-cpp/anchor.h"
#include "yaml-cpp/emitterstyle.h"
#include "yaml-cpp/eventhandler.h"
#include "yaml-cpp/node/ptr.h"
#include "yaml-cpp/nodebuilder.h"

#include <node.hpp>

namespace YAML {
namespace detail {
class node;
}  // namespace detail
struct Mark;
}  // namespace Asdf

namespace Asdf {
class Node;

using YAML::Mark;
using YAML::anchor_t;
using YAML::EmitterStyle;

class NodeBuilder : public YAML::NodeBuilder {
    public:
        Node Root();
};
}
