/* Copyright (c) 2019 The Brave Software Team. Distributed under the MPL2
 * license. This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_COMPONENTS_BRAVE_PAGE_GRAPH_GRAPH_ITEM_EDGE_EDGE_STORAGE_SET_H_
#define BRAVE_COMPONENTS_BRAVE_PAGE_GRAPH_GRAPH_ITEM_EDGE_EDGE_STORAGE_SET_H_

#include <string>
#include "brave/third_party/blink/brave_page_graph/graphml.h"
#include "brave/third_party/blink/brave_page_graph/graph_item/edge.h"
#include "brave/third_party/blink/brave_page_graph/graph_item/edge/edge_storage.h"
#include "brave/third_party/blink/brave_page_graph/types.h"

using ::std::string;

namespace brave_page_graph {

class Node;
class NodeActor;
class NodeStorage;
class PageGraph;

class EdgeStorageSet final : public EdgeStorage {
friend class PageGraph;
 public:
  EdgeStorageSet() = delete;
  ~EdgeStorageSet() override;
  string ItemName() const override;

 protected:
  EdgeStorageSet(const PageGraph* graph, const PageGraphId id,
    const NodeActor* const out_node, const NodeStorage* const in_node,
    const string& key, const string& value);
  string ToStringBody() const override;
  GraphMLXMLGroup GraphMLAttributes() const override;

  const string value_;
};

}  // namespace brave_page_graph

#endif  // BRAVE_COMPONENTS_BRAVE_PAGE_GRAPH_GRAPH_ITEM_EDGE_EDGE_STORAGE_SET_H_
