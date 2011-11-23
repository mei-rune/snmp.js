
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

#include "snmp.h"

class AeLoop : public node::ObjectWrap {

private:
    aeEventLoop *native_;

    AeLoop() {
        native_ = aeCreateEventLoop();
    }
    
    virtual ~AeLoop() {
        close();
    }

    void close() {
        if(0 != native_) {
            aeDeleteEventLoop(native_);
            native_ = 0;
        }
    }

    void stop() {
        if(0 == native_) {
            return;
        }

        aeStop(native_);
    }

public:

    static void Initialize(v8::Handle<v8::Object> target){
        v8::HandleScope scope;

        
        oid_symbol = v8::Persistent<v8::String>::New(v8::String::NewSymbol("oid"));
        type_symbol = v8::Persistent<v8::String>::New(v8::String::NewSymbol("type"));
        value_symbol = v8::Persistent<v8::String>::New(v8::String::NewSymbol("value"));

        v8::Local<v8::FunctionTemplate> t = v8::FunctionTemplate::New(New);    
        t->SetClassName(v8::String::NewSymbol("AeLoop"));
        t->InstanceTemplate()->SetInternalFieldCount(1);

        NODE_SET_PROTOTYPE_METHOD(t, "close", Close);
        NODE_SET_PROTOTYPE_METHOD(t, "stop", Stop);


        target->Set(v8::String::NewSymbol("AeLoop"), t->GetFunction());
        
        //target->Set(v8::String::NewSymbol("appendVariableList"),  v8::FunctionTemplate::New(Append)->GetFunction());
        //target->Set(v8::String::NewSymbol("getVariableList"),  v8::FunctionTemplate::New(Get)->GetFunction());
        //target->Set(v8::String::NewSymbol("clearVariableList"),  v8::FunctionTemplate::New(Clear)->GetFunction());        
    }


    static v8::Handle<v8::Value> New(const v8::Arguments& args) {
        v8::HandleScope scope;
        if(0 != args.Length()) {
            return ThrowError("Must pass empty argument to constructor.");
        }

        AeLoop* loop = new AeLoop();
        loop->Wrap(args.This());
        return args.This();
    }


    static v8::Handle<v8::Value> Stop(const v8::Arguments& args) {
        v8::HandleScope scope;
        UNWRAP(AeLoop, wrap, args.This());
        wrap->stop();
        return v8::Undefined();
    }

    static v8::Handle<v8::Value> Close(const v8::Arguments& args) {
        v8::HandleScope scope;
        UNWRAP(AeLoop, wrap, args.This());        
        wrap->close();
        return v8::Undefined();
    }
};

class AeFile : public node::ObjectWrap {

private:

    aeFileEvent handle_;
    AeLoop* loop;

    v8::Persistent<v8::Object> object_;
    v8::Persistent<v8::Object> loop_;

    AeFile() : loop(0){
    }

    virtual ~AeFile() {
        if(0 != loop) {
            aeDeleteFileEvent(loop, handle_.)
        }
    }


    void close() {
        if (NULL != native_) {
            snmp_free_pdu(native_);
            native_ = NULL;
        }
    }

    static void OnEvent(struct aeEventLoop *eventLoop, int fd, void *clientData, int mask) {
        HandleScope scope;
        AeFile* wrap = reinterpret_cast<AeFile*>(handle->data);
        assert(wrap->object_.IsEmpty() == false);
        MakeCallback(wrap->object_
            , (0 == (mask | AE_READABLE))?"onWrite" : "onRead"
            , 0
            , v8::Null());
    }


public:
    static void Initialize(Handle<Object> target) {
      HandleScope scope;

      Local<FunctionTemplate> t = FunctionTemplate::New(New);
      t->InstanceTemplate()->SetInternalFieldCount(1);
      t->SetClassName(String::NewSymbol("AeFile"));

      NODE_SET_PROTOTYPE_METHOD(t, "close", Close);

      target->Set(String::NewSymbol("FSEvent"),
                  Persistent<FunctionTemplate>::New(t)->GetFunction());
    }


    static Handle<Value> New(const Arguments& args){
        HandleScope scope;
        assert(args.IsConstructCall());

        UNWRAP(AeLoop, loop, args[0]);

        AeFile* acFile = new AeFile();
        acFile->object_ = v8::Persistent<v8::Object>::New(object);
        acFile->object_ = v8::Persistent<v8::Object>::New(a);


        aeCreateFileEvent(acFile- *eventLoop, int fd, AE_READABLE | AE_WRITABLE,
        OnEvent, acFile);



      return scope.Close(args.This());
    }
  
};
