#include <unordered_set>
#include <string>
#include <cstring>
#include "application.h"

int	main(int argc, char **argv)
{
	bool	debug = false;
	bool	lcd = false;
	std::unordered_set<std::string>	args;
	for (int i = 0; i < argc; ++i)
		args.insert(std::string(argv[i], strlen(argv[i])));
	if (args.find("-d") != args.end())
		debug = true;
	if (args.find("-l") != args.end())
		lcd = true;
	Application app(debug, lcd, argv[0]);
	return app.run();
}
