/**
 *
 * @name GetValueByPointer
 * @title
 * @category
 * @version 2.0
 * @public
 * @tObject UDF
 * @author rkooijman 
 *
 * @description 
 *
 * @usage 
 *
 * @param 
 * {@type}
 *
 * @return 
 *
 * @details  
 *
 * @example 
 *
 * @see
 *
 */
 
#include "udxinc.h"

#include "rapidjson/document.h"
#include "rapidjson/pointer.h"
#include "rapidjson/error/en.h"

using namespace nz::udx_ver2;
using namespace rapidjson;

#define STRING_TYPE(type) (type == UDX_FIXED || type == UDX_VARIABLE || \
			type == UDX_NATIONAL_FIXED || type == UDX_NATIONAL_VARIABLE || \
			type == UDX_GEOMETRY || type == UDX_VARBINARY)

#define MAX_JSON_DOCUMENT_LENGTH	64*1024
#define MAX_JSON_STRING_LENGTH		04*1024

// DocumentNZ type that uses 64K document length and 4K string length on the stack
typedef GenericDocument<UTF8<>, MemoryPoolAllocator<>, MemoryPoolAllocator<> > DocumentNZ;

class GetValueByPointer : public Udf {
public:
	static Udf* instantiate(UdxInit* pInit);
	GetValueByPointer(UdxInit* pInit);
	
	virtual ReturnValue evaluate()
	{
		char valueBuffer[MAX_JSON_DOCUMENT_LENGTH];
		char parseBuffer[MAX_JSON_STRING_LENGTH];
		MemoryPoolAllocator<> valueAllocator(valueBuffer, sizeof(valueBuffer));
		MemoryPoolAllocator<> parseAllocator(parseBuffer, sizeof(parseBuffer));

		// input: JSON document, JSON pointer
		DocumentNZ document(&valueAllocator, sizeof(parseBuffer), &parseAllocator);
		Pointer pointer;

		// JSON input document in argument[0]
		StringArg *arg0;
		arg0 = stringArg(0);
		char *input = arg0->data;
		int inputLen = arg0->length;
		if (inputLen > MAX_JSON_DOCUMENT_LENGTH) {
			throwUdxException("JSON input too long");
		}
		logMsg(LOG_DEBUG, "INPUT: %.*s\n", inputLen, input);

		// parse JSON input
		ParseResult ok = document.Parse(input);
		if (!ok) {
			char msg[1024];
			sprintf(msg, "JSON parse error (offset %u): %s\n",
		        ok.Offset(), GetParseError_En(ok.Code()));
			throwUdxException(msg);
		}

		// JSON input pointer in argument[1]
		StringArg *arg1;
		arg1 = stringArg(1);
		char *ptr = arg1->data;
		int ptrLen = arg1->length;
		logMsg(LOG_DEBUG, "POINTER: %.*s\n", ptrLen, ptr);
		pointer = Pointer(ptr);
		if (!pointer.IsValid()) {
			char msg[1024];
			sprintf(msg, "JSON pointer error (offset %u): %d\n",
		        pointer.GetParseErrorOffset(), pointer.GetParseErrorCode());
			throwUdxException(msg);
		}

		Value* result = pointer.Get(document);
		if (!result || result->IsNull()) {
			logMsg(LOG_DEBUG, "RESULT: NULL\n");
			NZ_UDX_RETURN_NULL();
		}
		if (result->IsBool()) {
			if (STRING_TYPE(returnType())) {
				StringReturn* ret = stringReturnInfo();
				const char* retval = result->GetBool() ? "true" : "false";
				ret->size = strlen(retval);
				memcpy(ret->data, retval, ret->size);
				NZ_UDX_RETURN_STRING(ret);
			}
			logMsg(LOG_DEBUG, "RESULT: %s\n", result->GetBool() ? "true" : "false");
			NZ_UDX_RETURN_BOOL(result->GetBool());
		}
		if (result->IsInt()) {		// check for Int first to do the 'smallest' ones earlier
			if (STRING_TYPE(returnType())) {
				StringReturn* ret = stringReturnInfo();
				sprintf(ret->data, "%ld", result->GetInt());
				ret->size = strlen(ret->data);
				NZ_UDX_RETURN_STRING(ret);
			}
			logMsg(LOG_DEBUG, "RESULT: %ld\n", result->GetInt());
			NZ_UDX_RETURN_INT32(result->GetInt());
		}
		if (result->IsInt64()) {	// then for Int64 to handle left over big ints
			if (STRING_TYPE(returnType())) {
				StringReturn* ret = stringReturnInfo();
				sprintf(ret->data, "%lld", result->GetInt64());
				ret->size = strlen(ret->data);
				NZ_UDX_RETURN_STRING(ret);
			}
			logMsg(LOG_DEBUG, "RESULT: %lld\n", result->GetInt64());
			NZ_UDX_RETURN_INT64(result->GetInt64());
		}
		if (result->IsDouble()) {
			if (STRING_TYPE(returnType())) {
				StringReturn* ret = stringReturnInfo();
				sprintf(ret->data, "%g", result->GetDouble());
				ret->size = strlen(ret->data);
				NZ_UDX_RETURN_STRING(ret);
			}
			logMsg(LOG_DEBUG, "RESULT: %g\n", result->GetDouble());
			NZ_UDX_RETURN_DOUBLE(result->GetDouble());
		}
		if (result->IsString()) {
			// output: JSON string value at pointer
			StringReturn* ret = stringReturnInfo();
			logMsg(LOG_DEBUG, "RESULT: %.*s\n", result->GetStringLength(), result->GetString());
			ret->size = result->GetStringLength();
			memcpy(ret->data, result->GetString(), ret->size);
			NZ_UDX_RETURN_STRING(ret);
		}
		if (result->IsObject()) {		// TODO
			// output: JSON object stringified
			StringReturn* ret = stringReturnInfo();
			logMsg(LOG_DEBUG, "RESULT: %.*s\n", result->GetStringLength(), result->GetString());
			ret->size = result->GetStringLength();
			memcpy(ret->data, result->GetString(), ret->size);
			NZ_UDX_RETURN_STRING(ret);
		}
		// give up
		throwUdxException("unknown how to handle data result");
	}
};

GetValueByPointer::GetValueByPointer(UdxInit* pInit) : Udf(pInit) {
	// insert code here
}

Udf* GetValueByPointer::instantiate (UdxInit* pInit) {	
	return new GetValueByPointer(pInit); 
}
