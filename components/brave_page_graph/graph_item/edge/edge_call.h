/* Copyright (c) 2019 The Brave Software Team. Distributed under the MPL2
 * license. This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_COMPONENTS_BRAVE_PAGE_GRAPH_GRAPH_ITEM_EDGE_EDGE_CALL_H_
#define BRAVE_COMPONENTS_BRAVE_PAGE_GRAPH_GRAPH_ITEM_EDGE_EDGE_CALL_H_

#include <memory>
#include <string>
#include <vector>
#include "brave/components/brave_page_graph/graph_item/edge.h"
#include "brave/components/brave_page_graph/graph_item/node.h"
#include "brave/components/brave_page_graph/types.h"

using ::std::shared_ptr;
using ::std::string;
using ::std::vector;

namespace brave_page_graph {

class EdgeCall : public Edge {
 public:
  EdgeCall() = delete;
  EdgeCall(const PageGraphId id, shared_ptr<Node> in_node,
    shared_ptr<Node> out_node, const string& method_name,
    const vector<string>& arguments);
  explicit EdgeCall(const EdgeCall& edge) = default;
  ~EdgeCall() = default;
  string ItemName() const;


 protected:
  string ToStringBody() const;
  const string method_name_;
  const vector<string> arguments_
};

}  // namespace brave_page_graph

#endif BRAVE_COMPONENTS_BRAVE_PAGE_GRAPH_GRAPH_ITEM_EDGE_EDGE_CALL_H_
