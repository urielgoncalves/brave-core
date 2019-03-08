/* Copyright (c) 2019 The Brave Software Team. Distributed under the MPL2
 * license. This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_COMPONENTS_BRAVE_PAGE_GRAPH_GRAPH_ITEM_NODE_NODE_HTML_H_
#define BRAVE_COMPONENTS_BRAVE_PAGE_GRAPH_GRAPH_ITEM_NODE_NODE_HTML_H_

#include <map>
#include <string>
#include <vector>
#include "brave/components/brave_page_graph/graph_item/node.h"
#include "brave/components/brave_page_graph/types.h"

using ::std::map;
using ::std::string;
using ::std::vector;

namespace brave_page_graph {

class PageGraph;

class NodeHTML final : public Node {
friend class PageGraph;
 public:
  NodeHTML() = delete;
  ~NodeHTML() override;
  string ItemName() const override;

  // weak_ptr<NodeHTML> GetHTMLParent() const;
  // void SetHTMLParent(weak_ptr<NodeHTML> parent,
  //   weak_ptr<NodeHTML> before_sibling);
  // vector<weak_ptr<Node>> GetHTMLChildren() const;

 protected:
  NodeHTML(const PageGraph* graph, const PageGraphId id,
    const DOMNodeId node_id,  const string& tag_name);
  string ToStringBody() const override;

  const DOMNodeId node_id_;
  const string tag_name_;
  bool is_deleted_ = false;
  map<string, string> current_attributes_;
  NodeHTML* parent_node_ = nullptr;
  vector<NodeHTML*> child_nodes_;
};

}  // namespace brave_page_graph

#endif  // BRAVE_COMPONENTS_BRAVE_PAGE_GRAPH_GRAPH_ITEM_NODE_NODE_HTML_H_
