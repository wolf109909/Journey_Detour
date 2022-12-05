#pragma once



namespace Global {

    typedef __int64(__cdecl *_getconsole)(char *);
    typedef __int64(__cdecl *_printluamem)(__int64);

    extern bool isNetGUIEnabled;
    extern bool console_OutputRedirect;
    typedef const unsigned long long base_t;
}

