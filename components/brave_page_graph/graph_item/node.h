/* Copyright (c) 2019 The Brave Software Team. Distributed under the MPL2
 * license. This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_COMPONENTS_BRAVE_PAGE_GRAPH_GRAPH_ITEM_NODE_H_
#define BRAVE_COMPONENTS_BRAVE_PAGE_GRAPH_GRAPH_ITEM_NODE_H_

#include <memory>
#include <vector>
#include <string>
#include "brave/components/brave_page_graph/graph_item.h"
#include "brave/components/brave_page_graph/graph_item/edge.h"
#include "brave/components/brave_page_graph/types.h"

using ::std::string;
using ::std::shared_ptr;
using ::std::unique_ptr;
using ::std::vector;

namespace brave_page_graph {

class Edge;

class Node : public GraphItem {
 public:
  Node() = delete;
  explicit Node(const PageGraphId id);
  explicit Node(const Node& node) = default;
  ~Node() = default;

  string ItemName() const;
  void AddInEdge(shared_ptr<Edge> in_edge);
  void AddOutEdge(shared_ptr<Edge> out_edge);

 protected:
  string ToStringBody() const;
  string ToStringPrefix() const;
  string ToStringSuffix() const;
  unique_ptr<vector<weak_ptr<Edge>>> in_edges_ptr_;
  unique_ptr<vector<weak_ptr<Edge>>> out_edges_ptr_;
};

}  // namespace brave_page_graph

#endif BRAVE_COMPONENTS_BRAVE_PAGE_GRAPH_GRAPH_ITEM_NODE_H_
