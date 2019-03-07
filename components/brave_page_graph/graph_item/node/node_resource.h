/* Copyright (c) 2019 The Brave Software Team. Distributed under the MPL2
 * license. This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_COMPONENTS_BRAVE_PAGE_GRAPH_GRAPH_ITEM_NODE_NODE_RESOURCE_H_
#define BRAVE_COMPONENTS_BRAVE_PAGE_GRAPH_GRAPH_ITEM_NODE_NODE_RESOURCE_H_

#include <string>
#include "brave/components/brave_page_graph/graph_item/node.h"
#include "brave/components/brave_page_graph/types.h"

using ::std::string;

namespace brave_page_graph {

class NodeResource : public Node {
 public:
  NodeResource() = delete;
  NodeResource(const PageGraphId id, const RequestType type);
  explicit NodeResource(const NodeResource& node) = default;
  ~NodeResource() = default;
  string ItemName() const;

 protected:
  string ToStringBody() const;
  const RequestType type_;
};

}  // namespace brave_page_graph

#endif BRAVE_COMPONENTS_BRAVE_PAGE_GRAPH_GRAPH_ITEM_NODE_NODE_RESOURCE_H_
