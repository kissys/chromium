// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_CONTROLS_WEBVIEW_WEBVIEW_H_
#define UI_VIEWS_CONTROLS_WEBVIEW_WEBVIEW_H_
#pragma once

#include "base/memory/scoped_ptr.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "content/public/browser/web_contents.h"
#include "content/public/browser/web_contents_delegate.h"
#include "ui/views/view.h"

namespace views {

class NativeViewHost;

class VIEWS_EXPORT WebView
    : public View,
      public NON_EXPORTED_BASE(content::NotificationObserver),
      public NON_EXPORTED_BASE(content::WebContentsDelegate) {
 public:
  static const char kViewClassName[];

  explicit WebView(content::BrowserContext* browser_context);
  virtual ~WebView();

  // This creates a WebContents if none is yet associated with this WebView. The
  // WebView owns this implicitly created WebContents.
  content::WebContents* GetWebContents();

  // Creates a WebContents if none is yet assocaited with this WebView, with the
  // specified site instance. The WebView owns this WebContents.
  void CreateWebContentsWithSiteInstance(content::SiteInstance* site_instance);

  // WebView does not assume ownership of WebContents set via this method, only
  // those it implicitly creates via GetWebContents() above.
  void SetWebContents(content::WebContents* web_contents);

  content::WebContents* web_contents() { return web_contents_; }

  content::BrowserContext* browser_context() { return browser_context_; }

  // Loads the initial URL to display in the attached WebContents. Creates the
  // WebContents if none is attached yet. Note that this is intended as a
  // convenience for loading the initial URL, and so URLs are navigated with
  // PAGE_TRANSITION_START_PAGE, so this is not intended as a general purpose
  // navigation method - use WebContents' API directly.
  void LoadInitialURL(const GURL& url);

  // Controls how the attached WebContents is resized.
  // false = WebContents' views' bounds are updated continuously as the
  //         WebView's bounds change (default).
  // true  = WebContents' views' position is updated continuously but its size
  //         is not (which may result in some clipping or under-painting) until
  //         a continuous size operation completes. This allows for smoother
  //         resizing performance during interactive resizes and animations.
  void SetFastResize(bool fast_resize);

  // Called when the WebContents is focused.
  // TODO(beng): This view should become a WebContentsViewObserver when a
  //             WebContents is attached, and not rely on the delegate to
  //             forward this notification.
  void OnWebContentsFocused(content::WebContents* web_contents);

  // When used to host UI, we need to explicitly allow accelerators to be
  // processed. Default is false.
  void set_allow_accelerators(bool allow_accelerators) {
    allow_accelerators_ = allow_accelerators;
  }

  // Overridden from View:
  virtual std::string GetClassName() const OVERRIDE;

 private:
  // Overridden from View:
  virtual void OnBoundsChanged(const gfx::Rect& previous_bounds) OVERRIDE;
  virtual void ViewHierarchyChanged(bool is_add,
                                    View* parent,
                                    View* child) OVERRIDE;
  virtual bool SkipDefaultKeyEventProcessing(
      const views::KeyEvent& event) OVERRIDE;
  virtual bool IsFocusable() const OVERRIDE;
  virtual void OnFocus() OVERRIDE;
  virtual void AboutToRequestFocusFromTabTraversal(bool reverse) OVERRIDE;
  virtual void GetAccessibleState(ui::AccessibleViewState* state) OVERRIDE;
  virtual gfx::NativeViewAccessible GetNativeViewAccessible() OVERRIDE;

  // Overridden from content::NotificationObserver:
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  // Overridden from content::WebContentsDelegate:
  virtual void WebContentsFocused(content::WebContents* web_contents) OVERRIDE;

  void AttachWebContents();
  void DetachWebContents();

  void RenderViewHostChanged(content::RenderViewHost* old_host,
                             content::RenderViewHost* new_host);
  void WebContentsDestroyed(content::WebContents* web_contents);

  NativeViewHost* wcv_holder_;
  scoped_ptr<content::WebContents> wc_owner_;
  content::WebContents* web_contents_;
  content::BrowserContext* browser_context_;
  content::NotificationRegistrar registrar_;
  bool allow_accelerators_;

  DISALLOW_COPY_AND_ASSIGN(WebView);
};

}  // namespace views

#endif  // UI_VIEWS_CONTROLS_WEBVIEW_WEBVIEW_H_