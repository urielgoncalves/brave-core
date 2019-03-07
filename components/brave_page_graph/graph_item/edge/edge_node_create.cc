/* Copyright (c) 2019 The Brave Software Team. Distributed under the MPL2
 * license. This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/components/brave_page_graph/graph_item/edge/edge_node_create.h"
#include <memory>
#include <string>
#include "brave/components/brave_page_graph/graph_item/edge/edge_node.h"
#include "brave/components/brave_page_graph/graph_item/node.h"
#include "brave/components/brave_page_graph/types.h"

using ::std::shared_ptr;
using ::std::string;

namespace brave_page_graph {

EdgeNodeCreate::EdgeNodeCreate(const PageGraphId id, shared_ptr<Node> in_node,
  shared_ptr<Node> out_node) :
    EdgeNode(id) {}

string EdgeNodeCreate::ItemName() const {
  return "EdgeNodeCreate#" + id_;
}

}  // brave_page_graph
