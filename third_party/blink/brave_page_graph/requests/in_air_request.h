/* Copyright (c) 2019 The Brave Software Team. Distributed under the MPL2
 * license. This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_COMPONENTS_BRAVE_PAGE_GRAPH_REQUESTS_H_
#define BRAVE_COMPONENTS_BRAVE_PAGE_GRAPH_REQUESTS_H_

#include "brave/third_party/blink/brave_page_graph/types.h"

namespace brave_page_graph {

class NodeHTMLElement;
class NodeScript;

class InAirRequest {
 public:
  InAirRequest(const RequestUrl url, const RequestType type,
    NodeScript* const script_node);
  InAirRequest(const RequestUrl url, const RequestType type,
    NodeHTMLElement* const html_elm_node);
  ~InAirRequest();
  RequestUrl GetRequestUrl() const;
  RequestType GetRequestType() const;

  bool IsScriptInitiated() const;
  NodeScript* GetScriptNode() const;
  bool IsElementInitiated() const;
  NodeHTMLElement* GetElementNode() const;

 protected:
  const RequestUrl url_;
  const RequestType type_;
  NodeScript* const script_node_;
  NodeHTMLElement* const elm_node_;
};

}  // namespace brave_page_graph

#endif  // BRAVE_COMPONENTS_BRAVE_PAGE_GRAPH_REQUESTS_H_
