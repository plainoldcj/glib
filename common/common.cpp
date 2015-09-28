/*
solution:	glib
project:	common
file:		common.cpp
author:		cj
*/

#include "common.h"

int COM_Tokenize(ctoken_t** tokens, const char* string) {
	ctoken_t* token;
	ctoken_t* head = NULL;
	ctoken_t* tail = NULL;
	bool reading = false;
	const char* strPtr = string;
	char* tokenPtr;
	int num = 0;
	int i;

	COM_assert(NULL != tokens);

	while('\0' != *strPtr) {
		if(' ' == *strPtr || '\t' == *strPtr) {
			if(reading) reading = false;
			strPtr++;
			continue;
		}
		if(!reading) {
			token = (ctoken_t*)malloc(sizeof(ctoken_t));
			if(!token) {
				COM::log << "out of memory tokenizing string" << std::endl;
				return -1;
			}
			// for(i = 0; i < MAX_TOKEN; ++i) token->string[i] = '\0';
			memset(token->string, 0, MAX_TOKEN);
			token->next = NULL;
			if(tail) tail->next = token;
			else head = token;
			tail = token;
			
			tokenPtr = token->string;
			reading = true;
		}
		*tokenPtr++ = *strPtr++;
	}

	token = head;
	while(NULL != token) {
		num++;
		sscanf(token->string, "%11f", &token->f);
		sscanf(token->string, "%d", &token->i);
		token = token->next;
	}

	*tokens = head;
	return num;
}

void COM_FreeTokens(ctoken_t* tokens) {
	ctoken_t* tmp;
	while(NULL != tokens) {
		tmp = tokens->next;
		free(tokens);
		tokens = tmp;
	}
}

namespace COM {

	LogStream log;

} // namespace COM