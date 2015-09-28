/*
solution:	glib
project:	common
file:		config.h
author:		cj
*/

#pragma once

#include <stdio.h>
#include <string>
#include <map>
#include "singleton.h"

namespace COM {

	class Config : public GEN::Singleton<Config> {
		friend class GEN::Singleton<Config>;
	private:
		struct Value {
			std::string str;
			int i;
			float f;
		};

		typedef std::map<std::string, Value>::const_iterator cvarIt_t;

		std::map<std::string, Value> _variables;

		Config(void);
	public:

		/*
		conditional gets. type Get(name, defaultVal) returns value of
		variable name, if it exists, defaultVal otherwise.
		*/
		const std::string& Get(const std::string& name, const std::string& defaultVal) const;
		int Get(const std::string& name, int defaultVal) const;
		float Get(const std::string& name, float defaultVal) const;
	};

} // namespace COM