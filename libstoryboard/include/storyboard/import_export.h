#pragma once

#if defined(_WIN32) || defined(__CYGWIN__)
#ifdef storyboard_EXPORTS
#ifdef __GNUC__
#define STORYBOARD_EXPORT __attribute__((dllexport))
#else
#define STORYBOARD_EXPORT __declspec(dllexport)
#endif
#else
#ifdef __GNUC__
#define STORYBOARD_EXPORT __attribute__((dllimport))
#else
#define STORYBOARD_EXPORT __declspec(dllimport)
#endif
#endif
#else
#if __GNUC__ >= 4
#define STORYBOARD_EXPORT __attribute__((visibility("default")))
#else
#define STORYBOARD_EXPORT
#endif
#endif
