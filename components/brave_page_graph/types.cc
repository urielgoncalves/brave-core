/* Copyright (c) 2019 The Brave Software Team. Distributed under the MPL2
 * license. This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/components/brave_page_graph/types.h"
#include <string>

using ::std::string;

namespace brave_page_graph {

string request_type_to_string(const RequestType type) noexcept {
  switch (type) {
    case RequestTypeImage:
      return "Image";
    case RequestTypeScriptClassic:
      return "ScriptClassic";
    case RequestTypeScriptModule:
      return "ScriptModule";
    case RequestTypeCSS:
      return "CSS";
    case RequestTypeVideo:
      return "Video";
    case RequestTypeAudio:
      return "Audio";
    case RequestTypeSVG:
      return "SVG";
    case RequestTypeFont:
      return "Font";
    case RequestTypeDocument:
      return "Document";
    case RequestTypeUnknown:
    default:
      return "Unknown";
  }
}

string script_type_to_string(const ScriptType type) noexcept {
  switch (type) {
    case ScriptTypeClassic:
      return "Classic";
    case ScriptTypeModule:
      return "Module";
    case ScriptTypeExtension:
      return "Extension";
    case ScriptTypeUnknown:
    default:
      return "Unknown";
  }
}

}  // namespace brave_page_graph