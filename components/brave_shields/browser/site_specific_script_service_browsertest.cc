/* Copyright (c) 2019 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "base/path_service.h"
#include "base/run_loop.h"
#include "base/task/post_task.h"
#include "base/test/thread_test_helper.h"
#include "brave/browser/brave_browser_process_impl.h"
#include "brave/common/brave_paths.h"
#include "brave/components/brave_shields/browser/local_data_files_service.h"
#include "brave/components/brave_shields/browser/site_specific_script_service.h"
#include "chrome/browser/extensions/extension_browsertest.h"

using extensions::ExtensionBrowserTest;

const char kLocalDataFilesComponentTestId[] =
    "eclbkhjphkhalklhipiicaldjbnhdfkc";

const char kLocalDataFilesComponentTestBase64PublicKey[] =
    "MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAsleoSxQ3DN+6xym2P1uX"
    "mN6ArIWd9Oru5CSjS0SRE5upM2EnAl/C20TP8JdIlPi/3tk/SN6Y92K3xIhAby5F"
    "0rbPDSTXEWGy72tv2qb/WySGwDdvYQu9/J5sEDneVcMrSHcC0VWgcZR0eof4BfOy"
    "fKMEnHX98tyA3z+vW5ndHspR/Xvo78B3+6HX6tyVm/pNlCNOm8W8feyfDfPpK2Lx"
    "qRLB7PumyhR625txxolkGC6aC8rrxtT3oymdMfDYhB4BZBrzqdriyvu1NdygoEiF"
    "WhIYw/5zv1NyIsfUiG8wIs5+OwS419z7dlMKsg1FuB2aQcDyjoXx1habFfHQfQwL"
    "qwIDAQAB";

class SiteSpecificScriptServiceTest : public ExtensionBrowserTest {
 public:
  SiteSpecificScriptServiceTest() {}

  void SetUp() override {
    InitService();
    brave::RegisterPathProvider();
    ExtensionBrowserTest::SetUp();
  }

  void PreRunTestOnMainThread() override {
    ExtensionBrowserTest::PreRunTestOnMainThread();
    WaitForSiteSpecificScriptServiceThread();
    ASSERT_TRUE(g_brave_browser_process->local_data_files_service()->
                IsInitialized());
  }

  void InitService() {
    brave_shields::LocalDataFilesService::
        SetComponentIdAndBase64PublicKeyForTest(
            kLocalDataFilesComponentTestId,
            kLocalDataFilesComponentTestBase64PublicKey);
  }

  void GetTestDataDir(base::FilePath* test_data_dir) {
    base::ScopedAllowBlockingForTesting allow_blocking;
    base::PathService::Get(brave::DIR_TEST_DATA, test_data_dir);
  }

  bool InstallSiteSpecificScriptExtension() {
    base::FilePath test_data_dir;
    GetTestDataDir(&test_data_dir);
    const extensions::Extension* mock_extension =
        InstallExtension(test_data_dir.AppendASCII("site-specific-script-data"),
                         1);
    if (!mock_extension)
      return false;

    g_brave_browser_process->site_specific_script_service()->OnComponentReady(
      mock_extension->id(), mock_extension->path(), "");
    WaitForSiteSpecificScriptServiceThread();

    return true;
  }

  void WaitForSiteSpecificScriptServiceThread() {
    scoped_refptr<base::ThreadTestHelper> io_helper(
        new base::ThreadTestHelper(
            g_brave_browser_process->site_specific_script_service()->
            GetTaskRunner()));
    ASSERT_TRUE(io_helper->Run());
    base::RunLoop().RunUntilIdle();
  }

  bool ScriptsFor(const GURL& primary_url, std::vector<std::string>* scripts) {
    base::ScopedAllowBlockingForTesting allow_blocking;
    return g_brave_browser_process->site_specific_script_service()->
      ScriptsFor(primary_url, scripts);
  }

  int GetRulesSize() {
    return g_brave_browser_process->site_specific_script_service()->
      rules_.size();
  }

  void ClearRules() {
    g_brave_browser_process->site_specific_script_service()->
      rules_.clear();
  }
};

IN_PROC_BROWSER_TEST_F(SiteSpecificScriptServiceTest, RuleParsing) {
  ASSERT_TRUE(InstallSiteSpecificScriptExtension());
  std::vector<std::string> scripts;

  // URL should match two rules, each with a different script
  // (first rule is an exact match, second rule is a TLD match)
  ASSERT_TRUE(ScriptsFor(GURL("https://www.example.com/"), &scripts));
  ASSERT_EQ(scripts.size(), 2UL);
  EXPECT_EQ(scripts[0].find("example.com"), 7UL);
  EXPECT_EQ(scripts[1].find("example.*"), 7UL);

  // URL should match two rules, each with a different script
  // (first rule is still an exact match because 80 is the default port,
  // second rule is a TLD match)
  ASSERT_TRUE(ScriptsFor(GURL("https://www.example.com:80/"), &scripts));
  ASSERT_EQ(scripts.size(), 2UL);
  EXPECT_EQ(scripts[0].find("example.com"), 7UL);
  EXPECT_EQ(scripts[1].find("example.*"), 7UL);

  // URL should match one rule with one script (because of TLD matching)
  ASSERT_TRUE(ScriptsFor(GURL("https://www.example.org/"), &scripts));
  ASSERT_EQ(scripts.size(), 1UL);
  EXPECT_EQ(scripts[0].find("example.*"), 7UL);

  // URL should match one rule with one script (because 80 is the default port)
  ASSERT_TRUE(ScriptsFor(GURL("https://www.example.org:80/"), &scripts));
  ASSERT_EQ(scripts.size(), 1UL);
  EXPECT_EQ(scripts[0].find("example.*"), 7UL);

  // URL should match one rule with one script (because TLD matching works on
  // multi-dotted TLDs)
  ASSERT_TRUE(ScriptsFor(GURL("https://www.example.co.uk/"), &scripts));
  ASSERT_EQ(scripts.size(), 1UL);
  EXPECT_EQ(scripts[0].find("example.*"), 7UL);

  // URL should not match any rules (because of scheme)
  ASSERT_FALSE(ScriptsFor(GURL("http://www.example.com/"), &scripts));
  EXPECT_EQ(scripts.size(), 0UL);

  // URL should not match any rules (because of non-default port)
  ASSERT_FALSE(ScriptsFor(GURL("http://www.example.com:8000/"), &scripts));
  EXPECT_EQ(scripts.size(), 0UL);

  // URL should not match any rules (because wildcard at end of pattern only
  // matches TLDs, not arbitrary domains)
  ASSERT_FALSE(ScriptsFor(GURL("https://www.example.evil.com/"), &scripts));
  EXPECT_EQ(scripts.size(), 0UL);
}

// Ensure the site specific script service properly clears its cache of
// precompiled URLPatterns if initialized twice. (This can happen if
// the parent component is updated while Brave is running.)
IN_PROC_BROWSER_TEST_F(SiteSpecificScriptServiceTest, ClearCache) {
  ASSERT_TRUE(InstallSiteSpecificScriptExtension());
  int size = GetRulesSize();
  // clear the cache manually to make sure we're actually
  // reinitializing it the second time
  ClearRules();
  ASSERT_TRUE(InstallSiteSpecificScriptExtension());
  EXPECT_EQ(size, GetRulesSize());
  // now reinitialize without manually clearing (simulates an in-place
  // component update)
  ASSERT_TRUE(InstallSiteSpecificScriptExtension());
  EXPECT_EQ(size, GetRulesSize());
}
