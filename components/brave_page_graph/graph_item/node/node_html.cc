/* Copyright (c) 2019 The Brave Software Team. Distributed under the MPL2
 * license. This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/components/brave_page_graph/graph_item/node/node_html.h"
#include <string>
#include "brave/components/brave_page_graph/graph_item/node.h"
#include "brave/components/brave_page_graph/types.h"

using ::std::string;

namespace brave_page_graph {

NodeHTML::NodeHTML(const PageGraphId id, const DOMNodeID node_id,
  const string& tag_name) :
    Node(id),
    node_id_(node_id),
    tag_name_(tag_name) {}

string NodeHTML::ItemName() const {
  return "NodeHTML#" + id_;
}

string NodeHTML::ToStringBody() const {
  return ItemName() + " [DOMNodeID:" + node_id_ + ", tag:" + tag_name_ + "]"; 
}

}  // brave_page_graph
