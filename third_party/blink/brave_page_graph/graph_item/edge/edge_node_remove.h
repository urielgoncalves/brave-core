/* Copyright (c) 2019 The Brave Software Team. Distributed under the MPL2
 * license. This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_COMPONENTS_BRAVE_PAGE_GRAPH_GRAPH_ITEM_EDGE_EDGE_NODE_REMOVE_H_
#define BRAVE_COMPONENTS_BRAVE_PAGE_GRAPH_GRAPH_ITEM_EDGE_EDGE_NODE_REMOVE_H_

#include <string>
#include "brave/third_party/blink/brave_page_graph/graphml.h"
#include "brave/third_party/blink/brave_page_graph/graph_item/edge/edge_node.h"
#include "brave/third_party/blink/brave_page_graph/types.h"

using ::std::string;

namespace brave_page_graph {

class Node;
class NodeScript;
class NodeHTML;
class PageGraph;

class EdgeNodeRemove final : public EdgeNode {
friend class PageGraph;
 public:
  EdgeNodeRemove() = delete;
  ~EdgeNodeRemove() override;
  string ItemName() const override;

 protected:
  EdgeNodeRemove(const PageGraph* graph, const PageGraphId id,
    const NodeScript* const out_node, const NodeHTML* const in_node);
  GraphMLXMLGroup GraphMLAttributes() const override;
};

}  // namespace brave_page_graph

#endif  // BRAVE_COMPONENTS_BRAVE_PAGE_GRAPH_GRAPH_ITEM_EDGE_EDGE_NODE_REMOVE_H_
