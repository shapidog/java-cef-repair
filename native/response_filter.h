// Copyright (c) 2019 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef JCEF_NATIVE_RESPONSE_FILTER_H_
#define JCEF_NATIVE_RESPONSE_FILTER_H_
#pragma once

#include <jni.h>
#include "include/cef_response_filter.h"
#include "jni_scoped_helpers.h"

// ResponseFilter implementation.
class ResponseFilter : public CefResponseFilter {
 public:
  ResponseFilter(JNIEnv* env, jobject handler);
    virtual bool InitFilter() override;

    CefResponseFilter::FilterStatus Filter(void* data_in,
                    size_t data_in_size,
                    size_t &data_in_read,
                    void* data_out,
                    size_t data_out_size,
                    size_t &data_out_written) override;
 protected:
  ScopedJNIObjectGlobal handle_;

  // Include the default reference counting implementation.
  IMPLEMENT_REFCOUNTING(ResponseFilter);
};

#endif  // JCEF_NATIVE_RESPONSE_FILTER_H_
