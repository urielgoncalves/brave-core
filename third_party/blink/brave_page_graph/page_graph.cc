/* Copyright (c) 2019 The Brave Software Team. Distributed under the MPL2
 * license. This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/third_party/blink/brave_page_graph/page_graph.h"
#include <iostream>
#include <sstream>
#include "base/logging.h"
#include "brave/third_party/blink/brave_page_graph/graphml.h"
#include "brave/third_party/blink/brave_page_graph/graph_item/edge/edge_attribute_set.h"
#include "brave/third_party/blink/brave_page_graph/graph_item/edge/edge_attribute_delete.h"
#include "brave/third_party/blink/brave_page_graph/graph_item/edge/edge_node_create.h"
#include "brave/third_party/blink/brave_page_graph/graph_item/edge/edge_node_delete.h"
#include "brave/third_party/blink/brave_page_graph/graph_item/edge/edge_node_insert.h"
#include "brave/third_party/blink/brave_page_graph/graph_item/edge/edge_node_remove.h"
#include "brave/third_party/blink/brave_page_graph/graph_item/node/node_actor.h"
#include "brave/third_party/blink/brave_page_graph/graph_item/node/node_html.h"
#include "brave/third_party/blink/brave_page_graph/graph_item/node/node_html_element.h"
#include "brave/third_party/blink/brave_page_graph/graph_item/node/node_html_text.h"
#include "brave/third_party/blink/brave_page_graph/graph_item/node/node_parser.h"
#include "brave/third_party/blink/brave_page_graph/graph_item/node/node_script.h"
#include "brave/third_party/blink/brave_page_graph/graph_item/node/node_shields.h"
#include "brave/third_party/blink/brave_page_graph/graph_item/node/node_storage_cookiejar.h"
#include "brave/third_party/blink/brave_page_graph/graph_item/node/node_storage_localstorage.h"
#include "brave/third_party/blink/brave_page_graph/types.h"

namespace brave_page_graph {

PageGraph::PageGraph() {
  std::cout << "Allocating PageGraph\n";
  graph_id_counter_ = 0;
  parser_node_ = new NodeParser(this, graph_id_counter_++);
  graph_nodes_.push_back(parser_node_);

  shields_node_ = new NodeShields(this, graph_id_counter_++);
  graph_nodes_.push_back(shields_node_);

  cookie_jar_node_ = new NodeStorageCookieJar(this, graph_id_counter_++);
  graph_nodes_.push_back(cookie_jar_node_);

  local_storage_node_ = new NodeStorageLocalStorage(this, graph_id_counter_++);
  graph_nodes_.push_back(local_storage_node_);
}

PageGraph::~PageGraph() {
  std::cout << "Deallocating PageGraph\n";
  // Since graph_items_ is the one place that owns the items in the graph,
  // so go ahead and delete them here.
  for (Node* elm : graph_nodes_) {
    delete elm;
  }
  for (const Edge* elm : graph_edges_) {
    delete elm;
  }
}

NodeHTML* PageGraph::GetHTMLNode(const DOMNodeId node_id) const {
  LOG_ASSERT(html_element_nodes_.count(node_id) +
    html_text_nodes_.count(node_id) == 1);

  if (html_element_nodes_.count(node_id) == 1) {
    return html_element_nodes_.at(node_id);
  }

  return html_text_nodes_.at(node_id);
}

NodeHTMLElement* PageGraph::GetHTMLElementNode(const DOMNodeId node_id) const {
  LOG_ASSERT(html_element_nodes_.find(node_id) != html_element_nodes_.end());
  return html_element_nodes_.at(node_id);
}

NodeHTMLText* PageGraph::GetHTMLTextNode(const DOMNodeId node_id) const {
  LOG_ASSERT(html_text_nodes_.find(node_id) != html_text_nodes_.end());
  return html_text_nodes_.at(node_id);
}

void PageGraph::RegisterHTMLElementNodeCreated(const DOMNodeId node_id,
    const string& tag_name) {
  NodeHTMLElement* new_node = new NodeHTMLElement(this, graph_id_counter_++,
    node_id, tag_name);
  graph_nodes_.push_back(new_node);
  html_element_nodes_.emplace(node_id, new_node);

  if (tag_name.compare("html") == 0) {
    LOG_ASSERT(html_root_node_ == nullptr);
    html_root_node_ = new_node;
  }

  NodeActor* acting_node = GetCurrentActingNode();

  EdgeNodeCreate* edge = new EdgeNodeCreate(this, graph_id_counter_++,
    acting_node, new_node);
  graph_edges_.push_back(edge);

  new_node->AddInEdge(edge);
  acting_node->AddOutEdge(edge);

  std::cout << "RegisterHTMLElementNodeCreated: " << tag_name << "\n";
}

void PageGraph::RegisterHTMLTextNodeCreated(const DOMNodeId node_id,
    const string& text) {
  NodeHTMLText* new_node = new NodeHTMLText(this, graph_id_counter_++,
    node_id, text);
  graph_nodes_.push_back(new_node);
  html_text_nodes_.emplace(node_id, new_node);

  NodeActor* acting_node = GetCurrentActingNode();

  EdgeNodeCreate* edge = new EdgeNodeCreate(this, graph_id_counter_++,
    acting_node, new_node);
  graph_edges_.push_back(edge);

  new_node->AddInEdge(edge);
  acting_node->AddOutEdge(edge);

  std::cout << "RegisterHTMLTextNodeCreated: " << text << "\n";
}

void PageGraph::RegisterHTMLElementNodeInserted(const DOMNodeId node_id,
    const DOMNodeId parent_node_id, const DOMNodeId before_sibling_id) {
  LOG_ASSERT(html_element_nodes_.count(node_id) == 1);
  NodeHTMLElement* inserted_node = html_element_nodes_[node_id];

  NodeActor* acting_node = GetCurrentActingNode();

  EdgeNodeInsert* edge = new EdgeNodeInsert(this, graph_id_counter_++,
    acting_node, inserted_node, parent_node_id, before_sibling_id);
  graph_edges_.push_back(edge);

  inserted_node->AddInEdge(edge);
  acting_node->AddOutEdge(edge);
}

void PageGraph::RegisterHTMLElementNodeRemoved(const DOMNodeId node_id) {
  LOG_ASSERT(html_element_nodes_.count(node_id) == 1);
  NodeHTMLElement* removed_node = html_element_nodes_[node_id];

  NodeActor* acting_node = GetCurrentActingNode();
  LOG_ASSERT(GetCurrentActingNode()->IsScript());

  EdgeNodeRemove* edge = new EdgeNodeRemove(this, graph_id_counter_++,
    static_cast<NodeScript*>(acting_node), removed_node);
  graph_edges_.push_back(edge);

  acting_node->AddOutEdge(edge);
  removed_node->AddInEdge(edge);
}

void PageGraph::RegisterAttributeSet(const DOMNodeId node_id,
    const string& attr_name, const string& attr_value) {
  LOG_ASSERT(html_element_nodes_.count(node_id) == 1);
  NodeHTMLElement* target_node = html_element_nodes_[node_id];

  NodeActor* acting_node = GetCurrentActingNode();

  EdgeAttributeSet* edge = new EdgeAttributeSet(this, graph_id_counter_++,
    acting_node, target_node, attr_name, attr_value);
  graph_edges_.push_back(edge);

  acting_node->AddOutEdge(edge);
  target_node->AddInEdge(edge);
}

void PageGraph::RegisterAttributeDelete(const DOMNodeId node_id,
    const string& attr_name) {
  LOG_ASSERT(html_element_nodes_.count(node_id) == 1);
  NodeHTMLElement* target_node = html_element_nodes_[node_id];

  LOG_ASSERT(GetCurrentActingNode()->IsScript());
  NodeActor* acting_node = GetCurrentActingNode();
  EdgeAttributeDelete* edge = new EdgeAttributeDelete(this,
    graph_id_counter_++, static_cast<NodeScript*>(acting_node),
    target_node, attr_name);

  graph_edges_.push_back(edge);
  acting_node->AddOutEdge(edge);
  target_node->AddInEdge(edge);
}

string PageGraph::ToGraphML() const {
  return graphml_for_page_graph(this);
}

NodeActor* PageGraph::GetCurrentActingNode() const {
  return parser_node_;
}

vector<Node*> PageGraph::Nodes() const {
  return graph_nodes_;
}

vector<const Edge*> PageGraph::Edges() const {
  return graph_edges_;
}

vector<const NodeHTMLElement*> PageGraph::HTMLElementNodes() const {
  vector<const NodeHTMLElement*> html_nodes;
  for (const auto& elm : html_element_nodes_) {
    html_nodes.push_back(elm.second);
  }
  return html_nodes;
}

vector<const GraphItem*> PageGraph::GraphItems() const {
  vector<const GraphItem*> graph_items;
  for (const GraphItem* elm : Nodes()) {
    graph_items.push_back(elm);
  }
  for (const GraphItem* elm : Edges()) {
    graph_items.push_back(elm);
  }
  return graph_items; 
}

}  // namespace brave_page_graph
