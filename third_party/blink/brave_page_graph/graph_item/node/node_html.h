/* Copyright (c) 2019 The Brave Software Team. Distributed under the MPL2
 * license. This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_COMPONENTS_BRAVE_PAGE_GRAPH_GRAPH_ITEM_NODE_NODE_HTML_H_
#define BRAVE_COMPONENTS_BRAVE_PAGE_GRAPH_GRAPH_ITEM_NODE_NODE_HTML_H_

#include <map>
#include <sstream>
#include <string>
#include <vector>
#include "brave/third_party/blink/brave_page_graph/graph_item/node.h"
#include "brave/third_party/blink/brave_page_graph/types.h"

using ::std::map;
using ::std::string;
using ::std::stringstream;
using ::std::vector;

namespace brave_page_graph {

class PageGraph;
class NodeHTMLElement;
class NodeHTMLText;

class NodeHTML : public Node {
friend class PageGraph;
friend class NodeHTMLElement;
friend class NodeHTMLText;
 public:
  NodeHTML() = delete;
  ~NodeHTML() override;
  virtual string ToHTMLString() const = 0;
  GraphMLFuncAttrMap GraphMLAttributeDefs() const override;

 protected:
  NodeHTML(const PageGraph* graph, const PageGraphId id,
    const DOMNodeId node_id);
  virtual void MarkNodeDeleted();
  virtual void ToHTMLString(const uint32_t indent,
    stringstream& builder) const = 0;

  const DOMNodeId node_id_;
  bool is_deleted_ = false;
  NodeHTMLElement* parent_node_ = nullptr;
};

void indent_for_html(const uint32_t indent, stringstream& builder);

}  // namespace brave_page_graph

#endif  // BRAVE_COMPONENTS_BRAVE_PAGE_GRAPH_GRAPH_ITEM_NODE_NODE_HTML_H_
