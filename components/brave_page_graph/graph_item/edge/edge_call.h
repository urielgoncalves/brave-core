/* Copyright (c) 2019 The Brave Software Team. Distributed under the MPL2
 * license. This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_COMPONENTS_BRAVE_PAGE_GRAPH_GRAPH_ITEM_EDGE_EDGE_CALL_H_
#define BRAVE_COMPONENTS_BRAVE_PAGE_GRAPH_GRAPH_ITEM_EDGE_EDGE_CALL_H_

#include <string>
#include <vector>
#include "brave/components/brave_page_graph/graph_item/edge.h"
#include "brave/components/brave_page_graph/graph_item/node.h"
#include "brave/components/brave_page_graph/types.h"

using ::std::string;
using ::std::vector;

namespace brave_page_graph {

class PageGraph;

class EdgeCall final : public virtual Edge {
friend class PageGraph;
 public:
  EdgeCall() = delete;
  ~EdgeCall() override;
  string ItemName() const override;

 protected:
  EdgeCall(const PageGraphId id, const Node* in_node,
    const Node* out_node, const vector<string>& arguments);
  string ToStringBody() const override;

  const vector<string> arguments_;
};

}  // namespace brave_page_graph

#endif  // BRAVE_COMPONENTS_BRAVE_PAGE_GRAPH_GRAPH_ITEM_EDGE_EDGE_CALL_H_
