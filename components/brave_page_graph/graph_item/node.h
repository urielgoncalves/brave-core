/* Copyright (c) 2019 The Brave Software Team. Distributed under the MPL2
 * license. This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_COMPONENTS_BRAVE_PAGE_GRAPH_GRAPH_ITEM_NODE_H_
#define BRAVE_COMPONENTS_BRAVE_PAGE_GRAPH_GRAPH_ITEM_NODE_H_

#include "brave/components/brave_page_graph/graph_item/node.h"
#include <vector>
#include <string>
#include "brave/components/brave_page_graph/graph_item.h"
#include "brave/components/brave_page_graph/types.h"

using ::std::string;
using ::std::vector;

namespace brave_page_graph {

class Edge;
class PageGraph;

class Node : public GraphItem {
friend class PageGraph;
 public:
  Node() = delete;
  ~Node() override;
  void AddInEdge(const Edge* in_edge);
  void AddOutEdge(const Edge* out_edge);

 protected:
  Node(const PageGraph* graph, const PageGraphId id);
  string ToStringPrefix() const override;
  string ToStringSuffix() const override;

  // Reminder to self:
  //   out_edge -> node -> in_edge
  // These vectors do not own their references.  All nodes in the entire
  // graph are owned by the PageGraph instance.
  vector<const Edge*> in_edges_ptr_;
  vector<const Edge*> out_edges_ptr_;
};

}  // namespace brave_page_graph

#endif  // BRAVE_COMPONENTS_BRAVE_PAGE_GRAPH_GRAPH_ITEM_NODE_H_
