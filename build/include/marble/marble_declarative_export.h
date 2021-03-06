
#ifndef MARBLE_DECLARATIVE_EXPORT_H
#define MARBLE_DECLARATIVE_EXPORT_H

#ifdef MARBLE_DECLARATIVE_STATIC_DEFINE
#  define MARBLE_DECLARATIVE_EXPORT
#  define MARBLE_DECLARATIVE_NO_EXPORT
#else
#  ifndef MARBLE_DECLARATIVE_EXPORT
#    ifdef marbledeclarative_EXPORTS
        /* We are building this library */
#      define MARBLE_DECLARATIVE_EXPORT __declspec(dllexport)
#    else
        /* We are using this library */
#      define MARBLE_DECLARATIVE_EXPORT __declspec(dllimport)
#    endif
#  endif

#  ifndef MARBLE_DECLARATIVE_NO_EXPORT
#    define MARBLE_DECLARATIVE_NO_EXPORT 
#  endif
#endif

#ifndef MARBLE_DECLARATIVE_DEPRECATED
#  define MARBLE_DECLARATIVE_DEPRECATED __attribute__ ((__deprecated__))
#endif

#ifndef MARBLE_DECLARATIVE_DEPRECATED_EXPORT
#  define MARBLE_DECLARATIVE_DEPRECATED_EXPORT MARBLE_DECLARATIVE_EXPORT MARBLE_DECLARATIVE_DEPRECATED
#endif

#ifndef MARBLE_DECLARATIVE_DEPRECATED_NO_EXPORT
#  define MARBLE_DECLARATIVE_DEPRECATED_NO_EXPORT MARBLE_DECLARATIVE_NO_EXPORT MARBLE_DECLARATIVE_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef MARBLE_DECLARATIVE_NO_DEPRECATED
#    define MARBLE_DECLARATIVE_NO_DEPRECATED
#  endif
#endif

#endif /* MARBLE_DECLARATIVE_EXPORT_H */
