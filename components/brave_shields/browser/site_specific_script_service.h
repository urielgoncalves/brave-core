/* Copyright (c) 2019 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_COMPONENTS_BRAVE_SHIELDS_BROWSER_SITE_SPECIFIC_SCRIPT_SERVICE_H_
#define BRAVE_COMPONENTS_BRAVE_SHIELDS_BROWSER_SITE_SPECIFIC_SCRIPT_SERVICE_H_

#include <stdint.h>

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "base/files/file_path.h"
#include "base/memory/weak_ptr.h"
#include "base/sequence_checker.h"
#include "base/sequenced_task_runner.h"
#include "base/values.h"
#include "brave/components/brave_shields/browser/base_local_data_files_observer.h"
#include "brave/components/brave_shields/browser/dat_file_util.h"
#include "content/public/common/resource_type.h"
#include "extensions/common/url_pattern.h"
#include "url/gurl.h"

#define SITE_SPECIFIC_SCRIPT_CONFIG_FILE "SiteSpecificScript.json"
#define SITE_SPECIFIC_SCRIPT_CONFIG_FILE_VERSION "1"

namespace brave_shields {

// The brave shields service in charge of site-specific script injection
class SiteSpecificScriptService : public BaseLocalDataFilesObserver {
 public:
  SiteSpecificScriptService();
  ~SiteSpecificScriptService() override;

  scoped_refptr<base::SequencedTaskRunner> GetTaskRunner();

  // implementation of BaseLocalDataFilesObserver
  void OnComponentReady(const std::string& component_id,
                        const base::FilePath& install_dir,
                        const std::string& manifest) override;

 private:
  void OnDATFileDataReady();

  struct SiteSpecificScriptRule {
    std::vector<URLPattern> urls;
    std::vector<base::FilePath> scripts;
    SiteSpecificScriptRule();
    SiteSpecificScriptRule(const SiteSpecificScriptRule& other);
    ~SiteSpecificScriptRule();
  };

  std::string file_contents_;
  std::vector<SiteSpecificScriptRule> rules_;
  base::FilePath install_dir_;

  SEQUENCE_CHECKER(sequence_checker_);
  base::WeakPtrFactory<SiteSpecificScriptService> weak_factory_;
  DISALLOW_COPY_AND_ASSIGN(SiteSpecificScriptService);
};

// Creates the SiteSpecificScriptService
std::unique_ptr<SiteSpecificScriptService> SiteSpecificScriptServiceFactory();

}  // namespace brave_shields

#endif  // BRAVE_COMPONENTS_BRAVE_SHIELDS_BROWSER_SITE_SPECIFIC_SCRIPT_SERVICE_H_
