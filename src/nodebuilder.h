#pragma once

#include <vector>

#include "yaml-cpp/anchor.h"
#include "yaml-cpp/emitterstyle.h"
#include "yaml-cpp/eventhandler.h"
#include "yaml-cpp/node/ptr.h"

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

class NodeBuilder : public YAML::EventHandler {
 public:
  NodeBuilder();
  virtual ~NodeBuilder();

  Node Root();

  virtual void OnDocumentStart(const Mark& mark);
  virtual void OnDocumentEnd();

  virtual void OnNull(const Mark& mark, anchor_t anchor);
  virtual void OnAlias(const Mark& mark, anchor_t anchor);
  virtual void OnScalar(const Mark& mark, const std::string& tag,
                        anchor_t anchor, const std::string& value);

  virtual void OnSequenceStart(const Mark& mark, const std::string& tag,
                               anchor_t anchor, EmitterStyle::value style);
  virtual void OnSequenceEnd();

  virtual void OnMapStart(const Mark& mark, const std::string& tag,
                          anchor_t anchor, EmitterStyle::value style);
  virtual void OnMapEnd();

 private:
  YAML::detail::node& Push(const Mark& mark, anchor_t anchor);
  void Push(YAML::detail::node& node);
  void Pop();
  void RegisterAnchor(anchor_t anchor, YAML::detail::node& node);

 private:
  YAML::detail::shared_memory_holder m_pMemory;
  YAML::detail::node* m_pRoot;

  typedef std::vector<YAML::detail::node*> Nodes;
  Nodes m_stack;
  Nodes m_anchors;

  typedef std::pair<YAML::detail::node*, bool> PushedKey;
  std::vector<PushedKey> m_keys;
  std::size_t m_mapDepth;
};
}
