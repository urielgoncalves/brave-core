/* Copyright (c) 2019 The Brave Software Team. Distributed under the MPL2
 * license. This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_COMPONENTS_BRAVE_PAGE_GRAPH_GRAPH_ITEM_NODE_NODE_HTML_TEXT_H_
#define BRAVE_COMPONENTS_BRAVE_PAGE_GRAPH_GRAPH_ITEM_NODE_NODE_HTML_TEXT_H_

#include <memory>
#include <sstream>
#include <string>
#include "brave/third_party/blink/brave_page_graph/graphml.h"
#include "brave/third_party/blink/brave_page_graph/graph_item/node.h"
#include "brave/third_party/blink/brave_page_graph/graph_item/node/node_html.h"
#include "brave/third_party/blink/brave_page_graph/types.h"

using ::std::string;
using ::std::stringstream;
using ::std::unique_ptr;

namespace brave_page_graph {

class EdgeNodeCreate;
class EdgeNodeDelete;
class EdgeNodeInsert;
class EdgeNodeRemove;
class EdgeAttributeDelete;
class EdgeAttributeSet;
class PageGraph;

class NodeHTMLText final : public NodeHTML {
friend class PageGraph;
 public:
  NodeHTMLText() = delete;
  ~NodeHTMLText() override;
  string ItemName() const override;
  string ToHTMLString() const override;
  const string& Text() const;

  using Node::AddInEdge;
  void AddInEdge(const EdgeNodeRemove* edge);
  void AddInEdge(const EdgeNodeInsert* edge);
  void AddInEdge(const EdgeNodeDelete* edge);

 protected:
  NodeHTMLText(const PageGraph* graph, const PageGraphId id,
    const DOMNodeId node_id,  const string& text);
  string ToStringBody() const override;
  void ToHTMLString(const uint32_t indent,
    stringstream& builder) const override;
  GraphMLXMLGroup GraphMLAttributes() const override;

  const string text_;
};

extern void indent_for_html(const uint32_t indent, stringstream& builder);

}  // namespace brave_page_graph

#endif  // BRAVE_COMPONENTS_BRAVE_PAGE_GRAPH_GRAPH_ITEM_NODE_NODE_HTML_TEXT_H_
