#include "nodebuilder.h"

namespace Asdf {

Node NodeBuilder::Root() {
  if (!m_pRoot)
    return Node();

  return Node(*m_pRoot, m_pMemory);
}
} /* end namespace Asdf */
