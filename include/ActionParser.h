#ifndef ACTION_PARSER_H
#define ACTION_PARSER_H

#include "Map.h"
#include "SerialMap.h"
#include <functional>
#include <string>

/**
 * @brief A class that stores an amount of action names associated with a
 * callback function to be executed when requested
 *
 * @tparam N The maximum number of actions to store
 */
template <int N>
class ActionParser
{
public:
	ActionParser() = default;
	ActionParser &with(String &action,
					   std::function<void(SerialMap<10> &, Stream &)> callback)
	{
		actions.put(action, callback);
		return *this;
	}
	ActionParser &with(const char *action,
					   std::function<void(SerialMap<10> &, Stream &)> callback)
	{
		actions.put(String(action), callback);
		return *this;
	}
	bool execute(SerialMap<10> &data, Stream &output)
	{
		if (data.has("action"))
		{
			String action = *data.get("action");
			for (auto it = actions.begin(); it != actions.end(); it++)
			{
				if (action == (*it).key())
				{
					(*it).value()(data, output);
					return true;
				}
			}
		}
		return false;
	}

private:
	Map<String, std::function<void(SerialMap<10> &, Stream &)>, N> actions;
};

#endif