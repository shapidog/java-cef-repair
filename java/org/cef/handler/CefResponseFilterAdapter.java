// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef.handler;

import java.nio.ByteBuffer;

public abstract class CefResponseFilterAdapter implements CefResponseFilter {
    @Override
    public boolean initFilter() {
        return true;
    }

    @Override
    public int filter(ByteBuffer dataIn, long dataInSize, long[] dataInRead,
                      ByteBuffer dataOut, long dataOutSize, long[] dataOutWritten) {
        return RESPONSE_FILTER_ERROR;
    }
}
