/* Copyright (c) 2019 The Brave Software Team. Distributed under the MPL2
 * license. This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/third_party/blink/brave_page_graph/graph_item/node/node_html_element.h"
#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "base/logging.h"
#include "brave/third_party/blink/brave_page_graph/graph_item/edge/edge_attribute_delete.h"
#include "brave/third_party/blink/brave_page_graph/graph_item/edge/edge_attribute_set.h"
#include "brave/third_party/blink/brave_page_graph/graph_item/edge/edge_html.h"
#include "brave/third_party/blink/brave_page_graph/graph_item/edge/edge_node_create.h"
#include "brave/third_party/blink/brave_page_graph/graph_item/edge/edge_node_delete.h"
#include "brave/third_party/blink/brave_page_graph/graph_item/edge/edge_node_insert.h"
#include "brave/third_party/blink/brave_page_graph/graph_item/edge/edge_node_remove.h"
#include "brave/third_party/blink/brave_page_graph/graph_item/node.h"
#include "brave/third_party/blink/brave_page_graph/graph_item/node/node_html.h"
#include "brave/third_party/blink/brave_page_graph/graphml.h"
#include "brave/third_party/blink/brave_page_graph/page_graph.h"
#include "brave/third_party/blink/brave_page_graph/types.h"

using ::std::endl;
using ::std::find;
using ::std::string;
using ::std::stringstream;
using ::std::to_string;
using ::std::vector;

namespace brave_page_graph {

NodeHTMLElement::NodeHTMLElement(const PageGraph* graph, const PageGraphId id,
    const DOMNodeId node_id, const string& tag_name) :
      NodeHTML(graph, id, node_id),
      tag_name_(tag_name) {}

NodeHTMLElement::~NodeHTMLElement() {}

string NodeHTMLElement::ItemName() const {
  return "NodeHTMLElement#" + to_string(id_);
}

string NodeHTMLElement::ToHTMLString() const {
  stringstream builder;
  ToHTMLString(0, builder);
  return builder.str();
}

const string& NodeHTMLElement::TagName() const {
  return tag_name_;
}

// Special case for when something (script) is removing an HTML element
// from the DOM.  Update the parallel HTML graph by removing the pointer
// to the parent element.
void NodeHTMLElement::AddInEdge(const EdgeNodeRemove* edge) {
  if (parent_node_ != nullptr) {
    parent_node_->RemoveChildNode(this);
  }
  parent_node_ = nullptr;
  Node::AddInEdge(edge);
}

void NodeHTMLElement::AddInEdge(const EdgeNodeInsert* edge) {
  parent_node_ = edge->GetParentNode();
  // Parent node will be nullptr if this is the root of a document, or a
  // subtree.
  if (parent_node_ != nullptr) {
    parent_node_->PlaceChildNodeAfterSiblingNode(this,
      edge->GetPriorSiblingNode());
  }
  Node::AddInEdge(edge);
}

void NodeHTMLElement::AddInEdge(const EdgeNodeDelete* const edge) {
  MarkNodeDeleted();
  Node::AddInEdge(edge);
}

void NodeHTMLElement::AddInEdge(const EdgeAttributeDelete* const edge) {
  current_attributes_.erase(edge->AttributeName());
  Node::AddInEdge(edge);
}

void NodeHTMLElement::AddInEdge(const EdgeAttributeSet* const edge) {
  current_attributes_.emplace(edge->AttributeName(), edge->AttributeValue());
  Node::AddInEdge(edge);
}

const vector<NodeHTML*>& NodeHTMLElement::ChildNodes() const {
  return child_nodes_;
}

GraphMLXML NodeHTMLElement::GraphMLTag() const {
  stringstream builder;
  builder << Node::GraphMLTag();

  for (const NodeHTML* child_node : child_nodes_) {
    EdgeHTML html_edge(this, child_node);
    builder << html_edge.GraphMLTag();
  }
  return builder.str();
}

string NodeHTMLElement::ToStringBody() const {
  stringstream string_builder;
  string_builder << ItemName();
  string_builder << " [DOMNodeId:";
  string_builder << to_string(node_id_);
  string_builder << ", tag:";
  string_builder << tag_name_;
  string_builder << ", attributes=";
  for (const auto& attr : current_attributes_) {
    string key = attr.first;
    string value = attr.second;
    string_builder << "{";
    string_builder << key;
    string_builder << "='";
    string_builder << value;
    string_builder << "'} ";
  }
  string_builder << "]";
  return string_builder.str();
}

void NodeHTMLElement::MarkNodeDeleted() {
  LOG_ASSERT(is_deleted_ == false);
  is_deleted_ = true;
  for (NodeHTML* node : child_nodes_) {
    node->MarkNodeDeleted();
  }
}

void NodeHTMLElement::PlaceChildNodeAfterSiblingNode(NodeHTML* child,
    NodeHTML* sibling) {
  // If this node has no current children, then this is easy, just add
  // the provided child as the only child.
  if (child_nodes_.size() == 0) {
    LOG_ASSERT(sibling == nullptr);
    child_nodes_.push_back(child);
    return;
  }

  // Or, if sibling is null, then insert the child in the first position
  // in the child nodes.
  if (sibling == nullptr) {
    child_nodes_.insert(child_nodes_.begin(), child);
    return;
  }

  // Otherwise, figure out where the sibling is in the child node set.
  const auto sib_pos = find(child_nodes_.begin(), child_nodes_.end(), sibling);
  LOG_ASSERT(sib_pos != child_nodes_.end());
  child_nodes_.insert(sib_pos + 1, child);
}

void NodeHTMLElement::RemoveChildNode(NodeHTML* child) {
  const auto child_pos = find(child_nodes_.begin(), child_nodes_.end(), child);
  LOG_ASSERT(child_pos != child_nodes_.end());
  child_nodes_.erase(child_pos);
}

void NodeHTMLElement::ToHTMLString(const uint32_t indent,
    stringstream& builder) const {
  indent_for_html(indent, builder);

  builder << "<" << tag_name_;
  for (const auto& attr : current_attributes_) {
    string key = attr.first;
    string value = attr.second;
    builder << " " << key << "=\"" << value << "\"";
  }
  builder << ">" << endl;

  const uint32_t new_indent = indent + 1;
  for (const NodeHTML* child : child_nodes_) {
    child->ToHTMLString(new_indent, builder);
  }

  indent_for_html(indent, builder);
  builder << "<" << tag_name_ << ">" << endl;
}

GraphMLXMLGroup NodeHTMLElement::GraphMLAttributes() const {
  GraphMLXMLGroup attrs = NodeHTML::GraphMLAttributes();
  attrs.push_back(graphml_attr_def_for_type(GraphMLAttrDefNodeType)
      ->ToValue("html node"));
  attrs.push_back(graphml_attr_def_for_type(GraphMLAttrDefNodeTag)
      ->ToValue(TagName()));
  return attrs;
}

}  // brave_page_graph
