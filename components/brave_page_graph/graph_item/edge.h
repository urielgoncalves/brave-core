/* Copyright (c) 2019 The Brave Software Team. Distributed under the MPL2
 * license. This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_COMPONENTS_BRAVE_PAGE_GRAPH_GRAPH_ITEM_EDGE_H_
#define BRAVE_COMPONENTS_BRAVE_PAGE_GRAPH_GRAPH_ITEM_EDGE_H_

#include <memory>
#include <vector>
#include "brave/components/brave_page_graph/graph_item.h"
#include "brave/components/brave_page_graph/graph_item/node.h"
#include "brave/components/brave_page_graph/types.h"

using ::std::shared_ptr;
using ::std::weak_ptr;

namespace brave_page_graph {

class Node;

class Edge : public GraphItem {
 public:
  Edge() = delete;
  Edge(const PageGraphId id, shared_ptr<Node> in_node,
    shared_ptr<Node> out_node);
  explicit Edge(const Edge& edge) = default;
  ~Edge() = default;

  string ItemName() const;

 protected:
  string ToStringBody() const;
  string ToStringPrefix() const;
  string ToStringSuffix() const;
  const weak_ptr<Node> in_node_ptr_;
  const weak_ptr<Node> out_node_ptr_;
};

}  // namespace brave_page_graph

#endif BRAVE_COMPONENTS_BRAVE_PAGE_GRAPH_GRAPH_ITEM_EDGE_H_
