/* Copyright (c) 2019 The Brave Software Team. Distributed under the MPL2
 * license. This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_COMPONENTS_BRAVE_PAGE_GRAPH_GRAPH_ITEM_NODE_NODE_HTML_ELEMENT_H_
#define BRAVE_COMPONENTS_BRAVE_PAGE_GRAPH_GRAPH_ITEM_NODE_NODE_HTML_ELEMENT_H_

#include <map>
#include <string>
#include "brave/third_party/blink/brave_page_graph/graph_item/node.h"
#include "brave/third_party/blink/brave_page_graph/graph_item/node/node_html.h"
#include "brave/third_party/blink/brave_page_graph/types.h"

namespace brave_page_graph {

class PageGraph;
class EdgeNodeCreate;
class EdgeNodeDelete;
class EdgeNodeInsert;
class EdgeNodeRemove;
class EdgeAttributeDelete;
class EdgeAttributeSet;

class NodeHTMLElement final : public NodeHTML {
friend class PageGraph;
friend class NodeHTMLText;
friend class NodeHTML;
 public:
  NodeHTMLElement() = delete;
  ~NodeHTMLElement() override;
  ItemName GetItemName() const override;
  const std::string& TagName() const;

  using Node::AddInEdge;
  void AddInEdge(const EdgeNodeRemove* edge);
  void AddInEdge(const EdgeNodeInsert* edge);
  void AddInEdge(const EdgeNodeDelete* edge);
  void AddInEdge(const EdgeAttributeDelete* edge);
  void AddInEdge(const EdgeAttributeSet* edge);
  
  const HTMLNodeList& ChildNodes() const;
  GraphMLXML GetGraphMLTag() const override;

 protected:
  NodeHTMLElement(const PageGraph* graph, const PageGraphId id,
    const DOMNodeId node_id,  const std::string& tag_name);
  ItemDesc GetDescBody() const override;
  void MarkNodeDeleted() override;
  void PlaceChildNodeAfterSiblingNode(NodeHTML* child,
    NodeHTML* sibling);
  void RemoveChildNode(NodeHTML* child);
  GraphMLXMLList GraphMLAttributes() const override;

  const std::string tag_name_;
  std::map<const std::string, const std::string> current_attributes_;
  HTMLNodeList child_nodes_;
};

}  // namespace brave_page_graph

#endif  // BRAVE_COMPONENTS_BRAVE_PAGE_GRAPH_GRAPH_ITEM_NODE_NODE_HTML_ELEMENT_H_
