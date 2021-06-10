# STM32F7 template using CMake

This template includes the basics to get a project up and running with CMake!

## Logging configuration

We use a cmake file, logging_config.cmake, to set the default log level, and add
overides for specific files. Here's a simple example:

    logging_set_default_level(${EXECUTABLE} ERROR)
    
    logging_set_level(DEBUG src/main.c)

The file-specific config will override the default. If no configuration is
specified, the default level with be `LOG_LEVEL_NONE`.
