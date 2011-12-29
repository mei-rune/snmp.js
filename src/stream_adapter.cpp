
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

#include <memory>
#include "stream_adapter.h"
#include "session.h"

void transport_udp_ctor(void);
void transport_udp6_ctor(void);

#define TO_STREAM(t) (Stream*)t->base_transport;

Stream::Stream() {
    session_ = NULL;
    t_ = (netsnmp_transport*)calloc(1, sizeof(netsnmp_transport));

    t_->f_recv     = &Recv;
    t_->f_send     = &Send;
    t_->f_close    = &Close;
    t_->f_setup_session  = &SetupSession;

    // NOTE: this is hack.
    t_->base_transport = (netsnmp_transport*)this;
    t_->sock       = STREAM_SOCKET;
}

//bool Stream::init(struct sockaddr_storage* addr, int local) {
//	v8::HandleScope scope;
//	v8::Handle<v8::Value> args1[1];
//	v8::Handle<v8::Value> args2[2];
//
//	// get process from global scope.
//	v8::Local<v8::Object> global = v8::Context::GetCurrent()->Global();
//	v8::Local<v8::Object> process = global->Get(process_symbol)->ToObject();
//	v8::Local<v8::Object> require = global->Get(require_symbol)->ToObject();
//
//	// dgram = require('dgram');
//	args1[0] = dgram_symbol;
//	v8::Local<v8::Object> dgram = require->CallAsFunction(global, 1, args1)->ToObject();
//
//	// sock = dgram.createSocket('udp4');
//	args2[0] = (addr->ss_family == AF_INET)?udp4_symbol:udp6_symbol;
//	args2[1] = this->on_message_callback;
//	v8::Local<v8::Value> sock = v8::Local<v8::Function>::Cast(dgram->Get(createSocket_symbol))->Call(dgram, 2, args1);
//	if(sock->IsNull() || sock->IsUndefined()) {
//		return false;
//	}
//
//	stream = v8::Persistent<v8::Object>::New(sock->ToObject());
//
//	// sock.bind(port)
//	args2[0] = from_uint16(ntohs((addr->ss_family == AF_INET)?((struct sockaddr_in*)addr)->sin_port:((struct sockaddr_in6*)addr)->sin6_port));
//	args2[1] = on_binding_callback;
//	v8::Local<v8::Function>::Cast(stream->Get(bind_symbol)->ToObject())->Call(sock->ToObject(), 2, args2);
//}

void Stream::Initialize() {
    transport_udp_ctor();
    transport_udp6_ctor();
}

Stream* Stream::ToStream(netsnmp_transport *t) {
    return TO_STREAM(t);
}

int Stream::SetupSession(netsnmp_transport *t, netsnmp_session *s) {
    Stream* stream = TO_STREAM(t);
    Session* session = Session::ToSession(s);
    session->on_open(stream);
    stream->session_ = session;
    return SNMPERR_SUCCESS;
}

int Stream::Recv(netsnmp_transport* t, void *buf, int size,
                 void **opaque, int *olength) {

    Stream* stream = TO_STREAM(t);
    Session* session = stream->session_;
    // TODO
    struct sockaddr *from;
    socklen_t       fromlen = sizeof(struct sockaddr);

    netsnmp_indexed_addr_pair* addr_pair = (netsnmp_indexed_addr_pair *) malloc(sizeof(netsnmp_indexed_addr_pair));
    memset(addr_pair, 0, sizeof(netsnmp_indexed_addr_pair));
    from = &addr_pair->remote_addr.sa;

    session->on_recv(stream, buf, &size, from, &fromlen);
    if (size < 0) {
        DEBUGMSGTL(("netsnmp_udp", "recvfrom fd %d err %d (\"%s\")\n",
                    t->sock, errno, strerror(errno)));
    }

    *opaque = (void *)addr_pair;
    *olength = sizeof(netsnmp_indexed_addr_pair);
    return size;
}

int Stream::Send(netsnmp_transport *t, void *buf, int size, void **opaque, int *olength) {

    Stream* stream = TO_STREAM(t);
    Session* session = stream->session_;

    netsnmp_indexed_addr_pair *addr_pair = NULL;

    if (opaque != NULL && *opaque != NULL &&
            ((*olength == sizeof(netsnmp_indexed_addr_pair) ||
              (*olength >= sizeof(struct sockaddr_in))))) {
        addr_pair = (netsnmp_indexed_addr_pair *) (*opaque);
    } else if (t->data != NULL &&
               t->data_length == sizeof(netsnmp_indexed_addr_pair)) {
        addr_pair = (netsnmp_indexed_addr_pair *) (t->data);
    }


    session->on_send(stream, buf, size, &addr_pair->remote_addr.sa, sizeof(struct sockaddr));

    return 1;
}

int Stream::Close(netsnmp_transport *t) {
    Stream* stream = TO_STREAM(t);
    Session* session = stream->session_;
    session->on_close(stream);
    stream->t_->base_transport = NULL;
    delete stream;
    return SNMPERR_SUCCESS;
}