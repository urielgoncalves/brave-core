/* Copyright (c) 2019 The Brave Software Team. Distributed under the MPL2
 * license. This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/third_party/blink/brave_page_graph/page_graph.h"
#include <signal.h>
#include <iostream>
#include <fstream>
#include <map>
#include <memory>
#include <sstream>
#include <string>
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

using ::std::endl;
using ::std::map;
using ::std::string;
using ::std::to_string;
using ::std::unique_ptr;
using ::std::vector;

namespace brave_page_graph {

namespace {
  PageGraph* yuck = nullptr;
}

void log(const string& msg) {
  LOG(INFO) << "*PageGraph* " << msg;
}

void write_to_disk(int signal) {
  std::cout << "GOT THAT SIG" << endl;
  std::ofstream outfile("/tmp/pagegraph.log");
  string output = yuck->ToGraphML();
  std::cout << output;
  outfile.write(output.c_str(), output.size());
  outfile.close();
}

PageGraph::PageGraph() :
    id_counter_(0),
    parser_node_(new NodeParser(this, id_counter_++)),
    shields_node_(new NodeShields(this, id_counter_++)),
    cookie_jar_node_(new NodeStorageCookieJar(this, id_counter_++)),
    local_storage_node_(new NodeStorageLocalStorage(this, id_counter_++)),
    html_root_node_(nullptr) {
  AddNode(parser_node_);
  AddNode(shields_node_);
  AddNode(cookie_jar_node_);
  AddNode(local_storage_node_);
  yuck = this;
  signal(30, &write_to_disk);
}

PageGraph::~PageGraph() {}

NodeHTML* PageGraph::GetHTMLNode(const DOMNodeId node_id) const {
  LOG_ASSERT(element_nodes_.count(node_id) + text_nodes_.count(node_id) == 1);
  if (element_nodes_.count(node_id) == 1) {
    return element_nodes_.at(node_id);
  }
  return text_nodes_.at(node_id);
}

NodeHTMLElement* PageGraph::GetHTMLElementNode(const DOMNodeId node_id) const {
  log("GetHTMLElementNode: " + to_string(node_id));
  LOG_ASSERT(element_nodes_.count(node_id) == 1);
  return element_nodes_.at(node_id);
}

NodeHTMLText* PageGraph::GetHTMLTextNode(const DOMNodeId node_id) const {
  LOG_ASSERT(text_nodes_.count(node_id) == 1);
  return text_nodes_.at(node_id);
}

void PageGraph::RegisterHTMLElementNodeCreated(const DOMNodeId node_id,
    const string& tag_name) {
  log("RegisterHTMLElementNodeCreated: " + to_string(node_id) + " (" + tag_name + ")");
  LOG_ASSERT(element_nodes_.count(node_id) == 0);
  NodeHTMLElement* const new_node = new NodeHTMLElement(this, id_counter_++,
    node_id, tag_name);

  AddNode(new_node);
  element_nodes_.emplace(node_id, new_node);

  if (html_root_node_ == nullptr && tag_name == "html") {
    html_root_node_ = new_node;
  }

  NodeActor* const acting_node = GetCurrentActingNode();

  const EdgeNodeCreate* const edge = new EdgeNodeCreate(this, id_counter_++,
    acting_node, new_node);
  AddEdge(edge);

  new_node->AddInEdge(edge);
  acting_node->AddOutEdge(edge);
}

void PageGraph::RegisterHTMLTextNodeCreated(const DOMNodeId node_id,
    const string& text) {
  log("RegisterHTMLTextNodeCreated: " + to_string(node_id) + ", " + text);
  LOG_ASSERT(text_nodes_.count(node_id) == 0);
  NodeHTMLText* const new_node = new NodeHTMLText(this, id_counter_++,
    node_id, text);
  AddNode(new_node);
  text_nodes_.emplace(node_id, new_node);

  NodeActor* const acting_node = GetCurrentActingNode();

  const EdgeNodeCreate* const edge = new EdgeNodeCreate(this, id_counter_++,
    acting_node, new_node);
  AddEdge(edge);

  new_node->AddInEdge(edge);
  acting_node->AddOutEdge(edge);
}

void PageGraph::RegisterHTMLElementNodeInserted(const DOMNodeId node_id,
    const string& tag_name, const DOMNodeId parent_node_id,
    const DOMNodeId before_sibling_id) {
  log("RegisterHTMLElementNodeInserted: " + tag_name + " (" + to_string(node_id) + "), "
    + to_string(parent_node_id) + ", " + to_string(before_sibling_id));

  const DOMNodeId inserted_sibling_node = (tag_name == "html")
    ? 0 : before_sibling_id;

  LOG_ASSERT(element_nodes_.count(node_id) == 1);
  NodeHTMLElement* const inserted_node = element_nodes_.at(node_id);

  NodeActor* const acting_node = GetCurrentActingNode();

  const EdgeNodeInsert* const edge = new EdgeNodeInsert(this, id_counter_++,
    acting_node, inserted_node, parent_node_id, inserted_sibling_node);
  AddEdge(edge);

  inserted_node->AddInEdge(edge);
  acting_node->AddOutEdge(edge);
}

void PageGraph::RegisterHTMLTextNodeInserted(const DOMNodeId node_id,
    const DOMNodeId parent_node_id, const DOMNodeId before_sibling_id) {
  log("RegisterHTMLTextNodeInserted: (" + to_string(node_id) + "), " +
    to_string(parent_node_id) + ", " + to_string(before_sibling_id));
  LOG_ASSERT(text_nodes_.count(node_id) == 1);
  NodeHTMLText* const inserted_node = text_nodes_.at(node_id);

  NodeActor* const acting_node = GetCurrentActingNode();

  const EdgeNodeInsert* const edge = new EdgeNodeInsert(this, id_counter_++,
    acting_node, inserted_node, parent_node_id, before_sibling_id);
  AddEdge(edge);

  inserted_node->AddInEdge(edge);
  acting_node->AddOutEdge(edge);
}

void PageGraph::RegisterHTMLElementNodeRemoved(const DOMNodeId node_id) {
  log("RegisterHTMLElementNodeRemoved: " + to_string(node_id));
  LOG_ASSERT(element_nodes_.count(node_id) == 1);
  NodeHTMLElement* const removed_node = element_nodes_.at(node_id);

  NodeActor* const acting_node = GetCurrentActingNode();

  const EdgeNodeRemove* const edge = new EdgeNodeRemove(this, id_counter_++,
    static_cast<NodeScript*>(acting_node), removed_node);
  AddEdge(edge);

  acting_node->AddOutEdge(edge);
  removed_node->AddInEdge(edge);
}

void PageGraph::RegisterHTMLTextNodeRemoved(const DOMNodeId node_id) {
  log("RegisterHTMLTextNodeRemoved: " + to_string(node_id));
  LOG_ASSERT(text_nodes_.count(node_id) == 1);
  NodeHTMLText* const removed_node = text_nodes_.at(node_id);

  NodeActor* const acting_node = GetCurrentActingNode();

  const EdgeNodeRemove* const edge = new EdgeNodeRemove(this, id_counter_++,
    static_cast<NodeScript*>(acting_node), removed_node);
  AddEdge(edge);

  acting_node->AddOutEdge(edge);
  removed_node->AddInEdge(edge);
}

void PageGraph::RegisterAttributeSet(const DOMNodeId node_id,
    const string& attr_name, const string& attr_value) {
  LOG_ASSERT(element_nodes_.count(node_id) == 1);
  NodeHTMLElement* const target_node = element_nodes_.at(node_id);

  NodeActor* const acting_node = GetCurrentActingNode();

  const EdgeAttributeSet* const edge = new EdgeAttributeSet(this, id_counter_++,
    acting_node, target_node, attr_name, attr_value);
  AddEdge(edge);

  acting_node->AddOutEdge(edge);
  target_node->AddInEdge(edge);
}

void PageGraph::RegisterAttributeDelete(const DOMNodeId node_id,
    const string& attr_name) {
  LOG_ASSERT(element_nodes_.count(node_id) == 1);
  NodeHTMLElement* const target_node = element_nodes_.at(node_id);

  NodeActor* const acting_node = GetCurrentActingNode();
  const EdgeAttributeDelete* const edge = new EdgeAttributeDelete(this,
    id_counter_++, static_cast<NodeScript*>(acting_node),
    target_node, attr_name);
  AddEdge(edge);

  acting_node->AddOutEdge(edge);
  target_node->AddInEdge(edge);
}

GraphMLXML PageGraph::ToGraphML() const {
  return graphml_for_page_graph(this);
}

NodeActor* PageGraph::GetCurrentActingNode() const {
  return parser_node_;
}

const vector<unique_ptr<Node> >& PageGraph::Nodes() const {
  return nodes_;
}

const vector<unique_ptr<const Edge> >& PageGraph::Edges() const {
  return edges_;
}

const vector<const GraphItem*>& PageGraph::GraphItems() const {
  return graph_items_;
}

void PageGraph::AddNode(Node* const node) {
  nodes_.push_back(unique_ptr<Node>(node));
  graph_items_.push_back(node);
}

void PageGraph::AddEdge(const Edge* const edge) {
  edges_.push_back(unique_ptr<const Edge>(edge));
  graph_items_.push_back(edge);
}


}  // namespace brave_page_graph
