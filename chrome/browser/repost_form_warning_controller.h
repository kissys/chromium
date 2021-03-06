// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_REPOST_FORM_WARNING_CONTROLLER_H_
#define CHROME_BROWSER_REPOST_FORM_WARNING_CONTROLLER_H_
#pragma once

#include "base/compiler_specific.h"
#include "chrome/browser/ui/tab_modal_confirm_dialog_delegate.h"

namespace content {
class NavigationController;
}

// This class is used to continue or cancel a pending reload when the
// repost form warning is shown. It is owned by the platform-dependent
// |RepostFormWarning{Gtk,Mac,View}| classes.
class RepostFormWarningController : public TabModalConfirmDialogDelegate {
 public:
  explicit RepostFormWarningController(content::WebContents* web_contents);
  virtual ~RepostFormWarningController();

  // TabModalConfirmDialogDelegate methods:
  virtual string16 GetTitle() OVERRIDE;
  virtual string16 GetMessage() OVERRIDE;
  virtual string16 GetAcceptButtonTitle() OVERRIDE;
#if defined(TOOLKIT_GTK)
  virtual const char* GetAcceptButtonIcon() OVERRIDE;
  virtual const char* GetCancelButtonIcon() OVERRIDE;
#endif  // defined(TOOLKIT_GTK)
  virtual void OnAccepted() OVERRIDE;
  virtual void OnCanceled() OVERRIDE;

 private:
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  // Weak pointer; this dialog is cancelled when the WebContents is closed.
  content::NavigationController* navigation_controller_;

  DISALLOW_COPY_AND_ASSIGN(RepostFormWarningController);
};

#endif  // CHROME_BROWSER_REPOST_FORM_WARNING_CONTROLLER_H_
