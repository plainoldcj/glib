/*
solution:	glib
project:	common
file:		config.cpp
author:		cj
*/

#include "common.h"
#include "config.h"

#define LINE_BUFFER_SIZE 2048

void Flatten(char* ptr) {
	while(NULL != ptr && '\0' != *ptr) {
		if('\n' == *ptr) *ptr = ' ';
		++ptr;
	}
}

namespace COM {

	Config::Config(void) {
		const char* filename = "config.txt";

		FILE* file = fopen(filename, "r");
		if(!file) {
			COM::log << "Config::Config: config file " << filename << " does not exist" << std::endl;
			return;
		}

		/*
		syntax:
		// comment
		name = value
		*/

		char lineBuffer[LINE_BUFFER_SIZE];
		while(!feof(file)) {
			memset(lineBuffer, 0, sizeof(lineBuffer));
			fgets(lineBuffer, LINE_BUFFER_SIZE, file);
			Flatten(lineBuffer);

			ctoken_t* tokens;
			int num = COM_Tokenize(&tokens, lineBuffer);
			if(!tokens) continue;

			ctoken_t* token = tokens;
			if(true) { //strlen(token->string) >= 2) { // MAX_TOKEN > 2!
				if(token->string[0] == '/' && token->string[1] == '/') {
					COM_FreeTokens(tokens);
					tokens = NULL;
					continue;
				}
			}

			if(3 != num) {
				COM::log << "Config::Config: too few arguments in line \"" << lineBuffer << "\"" << std::endl;
				COM_FreeTokens(tokens);
				tokens = NULL;
				continue;
			}

			ctoken_t* first = token;
			ctoken_t* second = first->next;
			ctoken_t* third = second->next;

			if(strcmp("=", second->string)) {
				COM::log << "Config::Config: malformed syntax in line \"" << lineBuffer 
					<< "\". expected \"name = value\"" << std::endl;
				COM_FreeTokens(tokens);
				tokens = NULL;
				continue;
			}

			Value val;
			val.str = std::string(third->string);
			val.i = third->i;
			val.f = third->f;

			_variables[first->string] = val;
		}

		fclose(file);
	};

	const std::string& Config::Get(const std::string& name, const std::string& defaultVal) const {
		cvarIt_t varIt(_variables.find(name));
		if(_variables.end() == varIt) return defaultVal;
		return varIt->second.str;
	}

	int Config::Get(const std::string& name, int defaultVal) const {
		cvarIt_t varIt(_variables.find(name));
		if(_variables.end() == varIt) return defaultVal;
		return varIt->second.i;
	}

	float Config::Get(const std::string& name, float defaultVal) const {
		cvarIt_t varIt(_variables.find(name));
		if(_variables.end() == varIt) return defaultVal;
		return varIt->second.f;
	}

} // namespace COM