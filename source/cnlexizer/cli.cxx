#include <getopt.h>
#include <iostream>
#include <stdexcept>

#include "cnlexizer.hxx"

static void _help_message()
{
	std::cout << "Usage: cnlexizer [OPTIONS] file\n"
				 "OPTIONS:\n"
				 "        -h|--help             help message\n"
				 "        -c|--config           config file, default=/etc/sgmt_cn.cfg\n"
				 "                              default to unigram\n"
				 "\n"
				 "Report bugs to jianing.yang@alibaba-inc.com\n"
			  << std::endl;
}

static int _do(const char *cfg, const char *file)
{
	char s[4096], t[8192];
	FILE *fp = NULL;
	try {
		std::cerr << "Loading configuration " << cfg << std::endl;
		CNLexizer clx(cfg);
		std::cerr << "Opening text " << file << std::endl;
		if (strcmp(file, "-") == 0) {
			fp = stdin;
		} else {
			fp = fopen(file, "r");
			if (fp == NULL) {
				std::cerr << "can not read file " << file << std::endl;
				return 1;
			}
		}
		while (EOF != fscanf(fp, "%4096s", s)) {
			clx.process(t, s);
			std::cout << t << "\n";	
		}
	} catch (std::exception &e) {
		std::cerr << "ERROR: " << e.what() << std::endl;
		return 1;
	}

	return 0;
}

int main(int argc, char *argv[])
{
	int c;
	const char default_config[] = "/etc/sgmt_cn.cfg";
	const char default_file[] = "-";
	const char *config = default_config, *file = default_file;
	
	while (true) {
		static struct option long_options[] =
		{
			{"help", no_argument, 0, 'h'},
			{"config", required_argument, 0, 'c'},
			{0, 0, 0, 0}
		};
		int option_index;
		
		c = getopt_long(argc, argv, "c:h", long_options, &option_index);
		if (c == -1) break;

		switch(c) {
			case 'h':
				_help_message();
				return 0;
			case 'c':
				config = optarg;
				break;
		}
	}

	if (optind < argc)
		file = argv[optind];

	return _do(config, file);
}