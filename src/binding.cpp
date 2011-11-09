
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

// --- Entry point ---
#ifdef _MSC_VER 
extern "C" __declspec (dllexport)
#else
extern
#endif
void init(v8::Handle<v8::Object> target) {
    v8::HandleScope scope;

    NODE_DEFINE_CONSTANT(target, ASN_BOOLEAN);
    NODE_DEFINE_CONSTANT(target, ASN_INTEGER);
    NODE_DEFINE_CONSTANT(target, ASN_BIT_STR);
    NODE_DEFINE_CONSTANT(target, ASN_OCTET_STR);
    NODE_DEFINE_CONSTANT(target, ASN_NULL);
    NODE_DEFINE_CONSTANT(target, ASN_OBJECT_ID);
    NODE_DEFINE_CONSTANT(target, ASN_SEQUENCE);
    NODE_DEFINE_CONSTANT(target, ASN_SET);

    NODE_DEFINE_CONSTANT(target, ASN_UNIVERSAL);
    NODE_DEFINE_CONSTANT(target, ASN_APPLICATION);
    NODE_DEFINE_CONSTANT(target, ASN_CONTEXT);
    NODE_DEFINE_CONSTANT(target, ASN_PRIVATE);
		
    NODE_DEFINE_CONSTANT(target, ASN_PRIMITIVE);
    NODE_DEFINE_CONSTANT(target, ASN_CONSTRUCTOR);
	
    NODE_DEFINE_CONSTANT(target, ASN_LONG_LEN);
    NODE_DEFINE_CONSTANT(target, ASN_EXTENSION_ID);
    NODE_DEFINE_CONSTANT(target, ASN_BIT8);


    session_initialize(target);
}



BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}
