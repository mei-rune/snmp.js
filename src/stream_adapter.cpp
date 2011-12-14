
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
#include "snmp.h"


class StreamTransport 
{
private:
	netsnmp_transport t_;
	v8::Persistent<v8::Object>   stream;
	v8::Persistent<v8::Function> on_binding_callback;
	v8::Persistent<v8::Function> on_message_callback;
public:
	StreamTransport() {
		
		t_.f_open     = NULL;//netsnmp_udpbase_recv;
		t_.f_recv     = NULL;//netsnmp_udpbase_recv;
		t_.f_send     = NULL;//netsnmp_udpbase_send;
		t_.f_close    = NULL;//netsnmp_socketbase_close;
		t_.f_accept   = NULL;
		t_.f_fmtaddr  = NULL;//netsnmp_udp_fmtaddr;
		t_.f_setup_session  = NULL;//netsnmp_udp_fmtaddr;
		t_.f_copy     = NULL;//netsnmp_udp_fmtaddr;
		t_.f_config   = NULL;//netsnmp_udp_fmtaddr;

		t_.flags      = 0;
#ifdef FOR_STANDARDS_COMPLIANCE_OR_FUTURE_USE
		t_.tmStateRef = NULL;
#endif
		t_.base_transport = NULL;
		t_.identifier = NULL;

		t_.sock       = 0;
	}
	
	netsnmp_transport* transport(){
		return &t_;
	}

	bool init(struct sockaddr_storage* addr, int local) {
		v8::HandleScope scope;
		v8::Handle<v8::Value> args1[1];
		v8::Handle<v8::Value> args2[2];

		// get process from global scope.
		v8::Local<v8::Object> global = v8::Context::GetCurrent()->Global();
		v8::Local<v8::Object> process = global->Get(process_symbol)->ToObject();
		v8::Local<v8::Object> require = global->Get(require_symbol)->ToObject();
		
		// dgram = require('dgram');
		args1[0] = dgram_symbol;
		v8::Local<v8::Object> dgram = require->CallAsFunction(global, 1, args1)->ToObject();
	
		// sock = dgram.createSocket('udp4');
		args2[0] = (addr->ss_family == AF_INET)?udp4_symbol:udp6_symbol;
		args2[1] = this->on_message_callback;
		v8::Local<v8::Value> sock = v8::Local<v8::Function>::Cast(dgram->Get(createSocket_symbol))->Call(dgram, 2, args1);
		if(sock->IsNull() || sock->IsUndefined()) {
			return false;
		}

		stream = v8::Persistent<v8::Object>::New(sock);

		// sock.bind(port)
		args2[0] = from_uint16(ntohs((addr->ss_family == AF_INET)?((struct sockaddr_in*)addr)->sin_port:((struct sockaddr_in6*)addr)->sin6_port));
		args2[1] = on_binding_callback;
		v8::Local<v8::Function>::Cast(stream->Get(bind_symbol))->Call(sock, 2, args2);
	}
};



void* stream_create(struct sockaddr_storage* addr, int local) {
	std::auto_ptr<StreamTransport> stream(new StreamTransport());
	if(!stream->init(addr, local)){
		return 0;
	}
	return stream.release();
}

netsnmp_transport* stream_get_transport(void* stream){
	return ((StreamTransport*)stream)->transport();
}