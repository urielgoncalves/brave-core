/* Copyright (c) 2019 The Brave Software Team. Distributed under the MPL2
 * license. This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_COMPONENTS_BRAVE_PAGE_GRAPH_GRAPH_ITEM_EDGE_EDGE_NODE_CREATE_H_
#define BRAVE_COMPONENTS_BRAVE_PAGE_GRAPH_GRAPH_ITEM_EDGE_EDGE_NODE_CREATE_H_

#include <string>
#include "brave/components/brave_page_graph/graph_item/edge/edge_node.h"
#include "brave/components/brave_page_graph/types.h"

using ::std::string;

namespace brave_page_graph {

class NodeActor;
class NodeHTML;
class PageGraph;

class EdgeNodeCreate final : public EdgeNode {
friend class PageGraph;
 public:
  EdgeNodeCreate() = delete;
  ~EdgeNodeCreate() override;
  string ItemName() const override;

 protected:
  EdgeNodeCreate(const PageGraph* graph, const PageGraphId id,
    const NodeActor* out_node, const NodeHTML* in_node);
};

}  // namespace brave_page_graph

#endif  // BRAVE_COMPONENTS_BRAVE_PAGE_GRAPH_GRAPH_ITEM_EDGE_EDGE_NODE_CREATE_H_
