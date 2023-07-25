#include <stdio.h>
#include <stdint.h> // uint8_t
#include <getopt.h> // getopt_long(...) for "--help"
#include <stdlib.h> // EXIT_SUCCESS
#include <errno.h>  // errno == ERANGE
#include <time.h>   // clock_gettime()
#include <string.h> // memset(...)
#include "implementation_md2.h"
#include "test.h"

const char *usage_msg =
    "Usage: %s [-V number] [-B number] [filePath]   Hash the file with MD2\n"
    "   or: %s [-h | --help]                        Show help message and exit\n"
    "   or: %s [-t | --test]                        Run tests and exit\n";

const char *help_msg =
    "------------- Positional arguments ------------\n"
    "  filePath   The path of the input file\n\n"
    "-------------- Optional arguments -------------\n"
    "Priority of Optional arguments: h > t > other\n"
    "  -V <version> Specify the implementation type to use\n"
    "               0 SIMD-Optimized Implementation (default)\n"
    "               1 Library-Optimized Implementation\n"
    "  -B <number>  The number of times to repeat the implementation (default: 1)\n"
    "  -h, --help   Get help for commands\n"
    "  -t, --test   Run tests\n";

void print_usage(const char *progname)
{
    fprintf(stderr, usage_msg, progname, progname, progname);
}

void print_help(const char *progname)
{
    print_usage(progname);
    fprintf(stderr, "\n%s", help_msg);
}

int main(int argc, char **argv)
{
    const char *progname = argv[0];

    if (argc < 2)
    {
        print_usage(progname);
        return EXIT_FAILURE;
    }

    // Variables for command-line arguments and options
    long implementationVersion = 0; // Version of implementation to use (default: 0)
    char *endptr;                   // Pointer for strtol error checking
    int repeat = 1;                 // Number of times to repeat the implementation (default: 1)
    int timeMeasurementFlag = 0;    // Flag to enable time measurement
    int testFlag = 0;               // Flag to indicate running tests

    const char *optstring = "V:B:th";

    // Struct for getopt_long(). Needed to support the command-line options "--help" and "--test"
    struct option options[] = {
        {"help", no_argument, NULL, 'h'},
        {"test", no_argument, NULL, 't'},
    };
    int opt;
    while ((opt = getopt_long(argc, argv, optstring, options, NULL)) != -1)
    {
        switch (opt)
        {
        case 'V':
            // Parsing and validating the implementation version argument
            errno = 0;
            implementationVersion = strtol(optarg, &endptr, 10);
            if (endptr == optarg || *endptr != '\0' || errno == ERANGE || implementationVersion < 0 || implementationVersion > 1)
            {
                fprintf(stderr, "Invalid implementation version! Please enter either 0 or 1 after the \"-V\" option\n");
                print_help(progname);
                return EXIT_FAILURE;
            }
            break;
        case 'B':
            // Parsing and validating the repetition number argument
            errno = 0;
            repeat = strtol(optarg, &endptr, 10);
            if (endptr == optarg || *endptr != '\0' || errno == ERANGE || repeat < 1)
            {
                fprintf(stderr, "Invalid repetition number! Please enter a positive integer after \"-B\" option\n");
                print_help(progname);
                return EXIT_FAILURE;
            }
            timeMeasurementFlag = 1;
            break;
        case 'h':
            print_help(progname);
            return EXIT_SUCCESS;
        case 't':
            testFlag = 1;
            break;
        default:
            fprintf(stderr, "Invalid option!\n");
            print_usage(progname);
            return EXIT_FAILURE;
        }
    }

    if (testFlag)
    {
        return test();
    }

    // check if the 'filePath' argument is missing
    if (optind >= argc)
    {
        fprintf(stderr, "%s: Missing positional argument -- 'filePath'\n", progname);
        print_usage(progname);
        return EXIT_FAILURE;
    }

    // Retrieving the input file path from positional argument
    const char *inputFilePath = argv[optind];

    return implementation_md2(timeMeasurementFlag, implementationVersion, repeat, inputFilePath);
}
