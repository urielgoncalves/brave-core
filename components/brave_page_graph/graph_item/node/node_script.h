/* Copyright (c) 2019 The Brave Software Team. Distributed under the MPL2
 * license. This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_COMPONENTS_BRAVE_PAGE_GRAPH_GRAPH_ITEM_NODE_NODE_SCRIPT_H_
#define BRAVE_COMPONENTS_BRAVE_PAGE_GRAPH_GRAPH_ITEM_NODE_NODE_SCRIPT_H_

#include <string>
#include "brave/components/brave_page_graph/graph_item/node/node_actor.h"
#include "brave/components/brave_page_graph/types.h"

using ::std::string;

namespace brave_page_graph {

class PageGraph;

class NodeScript final : public NodeActor {
friend class PageGraph;
 public:
  NodeScript() = delete;
  ~NodeScript() override;
  string ItemName() const override;
  bool IsScript() const override;

 protected:
   NodeScript(const PageGraph* graph, const PageGraphId id,
    const ScriptId script_id, const ScriptType type);
  string ToStringBody() const override;
  const ScriptId script_id_;
  const ScriptType type_;
};

}  // namespace brave_page_graph

#endif  // BRAVE_COMPONENTS_BRAVE_PAGE_GRAPH_GRAPH_ITEM_NODE_NODE_SCRIPT_H_
