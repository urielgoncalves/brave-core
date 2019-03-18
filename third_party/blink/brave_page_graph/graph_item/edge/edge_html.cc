/* Copyright (c) 2019 The Brave Software Team. Distributed under the MPL2
 * license. This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/third_party/blink/brave_page_graph/graph_item/edge/edge_html.h"
#include <ostream>
#include <string>
#include "brave/third_party/blink/brave_page_graph/graph_item/edge.h"
#include "brave/third_party/blink/brave_page_graph/graph_item/node/node_html.h"
#include "brave/third_party/blink/brave_page_graph/graph_item/node/node_html_element.h"
#include "brave/third_party/blink/brave_page_graph/graphml.h"
#include "brave/third_party/blink/brave_page_graph/page_graph.h"
#include "brave/third_party/blink/brave_page_graph/types.h"

using ::std::endl;
using ::std::string;
using ::std::stringstream;
using ::std::to_string;

namespace brave_page_graph {

EdgeHTML::EdgeHTML(const PageGraph* graph, const PageGraphId id,
    const NodeHTMLElement* const out_node, const NodeHTML* const in_node) :
      Edge(graph, id, out_node, in_node) {}

EdgeHTML::EdgeHTML(const NodeHTMLElement* const out_node,
    const NodeHTML* const in_node) :
      Edge(nullptr, 0, out_node, in_node) {}

EdgeHTML::~EdgeHTML() {}

string EdgeHTML::ItemName() const {
  return "EdgeHTML#" + to_string(id_);
}

GraphMLXML EdgeHTML::GraphMLTag() const {
  // graph_ will be null when EdgeHTML elements are being created only
  // for temporary GraphML export.  In all other cases graph_ will
  // point to the shared PageGraph instance/
  if (graph_ != nullptr) {
    return Edge::GraphMLTag();
  }

  // To ensure all tag ids are unique, dervie a graphml id based on
  // the parent and child DOMNodeIds, which will also make a unique combination. 
  const string graphml_id = to_string(out_node_->Id()) + "-" +
                            to_string(in_node_->Id());

  stringstream builder;
  builder << "<edge id\"t" + graphml_id << "\" " <<
                      "source=\"" << out_node_->GraphMLId() << "\" " <<
                      "target=\"" << in_node_->GraphMLId() << "\">" << endl;

  for (const GraphMLXML& elm : GraphMLAttributes()) {
    builder << "\t" << elm << endl;
  }
  builder << "</edge>" << endl;
  return builder.str();
}

GraphMLXMLGroup EdgeHTML::GraphMLAttributes() const {
  return {
    graphml_attr_def_for_type(GraphMLAttrDefEdgeType)
      ->ToValue("structure")
  };
}

}  // brave_page_graph
