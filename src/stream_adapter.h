
/*
 * Copyright (c) 2011 runner.mei
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef __stream_adapter__h__
#define __stream_adapter__h__ 1

#include "snmp.h"

#define STREAM_SOCKET   2345

class Session;

class Stream {
private:
    netsnmp_transport* t_;
    Session* session_;
public:
    Stream();

    netsnmp_transport* transport() {
        return t_;
    }


    static void Initialize();

    static Stream* ToStream(netsnmp_transport *t);

    static int SetupSession(netsnmp_transport *t, snmp_session *s);

    static int Recv(netsnmp_transport* t, void *buf, int size, void **opaque, int *olength);

    static int Send(netsnmp_transport *t, void *buf, int size, void **opaque, int *olength);

    static int Close(netsnmp_transport *t);
};

#endif // __stream_adapter__h__