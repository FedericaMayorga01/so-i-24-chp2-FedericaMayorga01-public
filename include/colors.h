#ifndef COLORS_H
#define COLORS_H

/**
 * @file colors.h
 * @brief Defines ANSI color codes for terminal output.
 */

/**
 * @brief Resets the terminal color to default.
 */
#define COLOR_RESET "\033[0m"

/**
 * @brief Sets the text color to blue.
 */
#define COLOR_BLUE "\033[34m"

/**
 * @brief Sets the text color to green.
 */
#define COLOR_GREEN "\033[32m"

/**
 * @brief Sets the text color to yellow.
 */
#define COLOR_YELLOW "\033[33m"

/**
 * @brief Sets the text color to red.
 */
#define COLOR_RED "\033[31m"

/**
 * @brief Sets the text color to cyan.
 */
#define COLOR_CYAN "\033[36m"

/**
 * @brief Sets the text color to magenta.
 */
#define COLOR_MAGENTA "\033[35m"

/**
 * @brief Enables bold text.
 */
#define COLOR_BOLD "\033[1m"

#endif // COLORS_H
