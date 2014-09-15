// Copyright � 2010-2014 The CefSharp Project. All rights reserved.
//
// Use of this source code is governed by a BSD-style license that can be found in the LICENSE file.
#pragma once

#include "Stdafx.h"

#include "CefAppWrapper.h"
#include "CefBrowserWrapper.h"
#include "CefAppUnmanagedWrapper.h"

using namespace System;
using namespace System::Diagnostics;
using namespace System::Collections::Generic;

namespace CefSharp
{
    CefRefPtr<CefRenderProcessHandler> CefAppUnmanagedWrapper::GetRenderProcessHandler()
    {
        return this;
    };

    // CefRenderProcessHandler
    void CefAppUnmanagedWrapper::OnBrowserCreated(CefRefPtr<CefBrowser> browser)
    {
        auto wrapper = gcnew CefBrowserWrapper(browser);
        _onBrowserCreated->Invoke(wrapper);

        //Multiple CefBrowserWrappers created when opening popups
        CefAppWrapper::Instance->browserWrappers->Add(browser->GetIdentifier(), wrapper);
    }

    void CefAppUnmanagedWrapper::OnBrowserDestroyed(CefRefPtr<CefBrowser> browser)
    {
        auto browserWrappers = CefAppWrapper::Instance->browserWrappers;
        auto browserId = browser->GetIdentifier();
        CefBrowserWrapper^ wrapper = nullptr;

        browserWrappers->TryGetValue(browserId, wrapper);

        if (wrapper != nullptr)
        {
            browserWrappers->Remove(browserId);
            _onBrowserDestroyed->Invoke(wrapper);
            delete wrapper;
        }
    };

    void CefAppUnmanagedWrapper::OnContextCreated(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context)
    {
        auto window = context->GetGlobal();

        JavascriptRootObjectWrapper^ jswindow = _windowObject;

        if (jswindow != nullptr)
        {
            jswindow->V8Value = window;
            jswindow->Bind();
        }
    };

    void CefAppUnmanagedWrapper::OnContextReleased(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context)
    {
        
    };

    void CefAppUnmanagedWrapper::Bind(JavascriptRootObject^ rootObject)
    {
        _windowObject = gcnew JavascriptRootObjectWrapper(rootObject);
    };
}