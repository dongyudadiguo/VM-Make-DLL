#ifdef INCLUDE_MAKEDLL
    extern void **bse_ptr; 
    extern void **crt_ptr;
    extern int *imp_ptr;
    extern FILE **file_ptr;
    extern void ***funcs_ptr;
    extern void **basic_funcs_ptr;
    extern int *funcs_size_ptr;

    extern void *bse;
    extern void *crt;
    extern int imp;
    extern FILE *file;
    extern void **funcs;
    extern void *basic_funcs;
    extern int funcs_size;
#else
    #include <stdlib.h>
    #include <string.h>
    #include <stdio.h>
    #include <windows.h>

    extern void (*new_funcs[])();
    extern int new_funcs_size;

    typedef void** (*get_bse_ptr_func)();
    typedef void** (*get_crt_ptr_func)();
    typedef int* (*get_imp_ptr_func)();
    typedef FILE** (*get_file_ptr_func)();
    typedef void*** (*get_funcs_ptr_func)();
    typedef int* (*get_funcs_size_ptr_func)();
    typedef void** (*get_basic_funcs_ptr_func)();

    static get_bse_ptr_func get_bse_ptr = NULL;
    static get_crt_ptr_func get_crt_ptr = NULL;
    static get_imp_ptr_func get_imp_ptr = NULL;
    static get_file_ptr_func get_file_ptr = NULL;
    static get_funcs_ptr_func get_funcs_ptr = NULL;
    static get_funcs_size_ptr_func get_funcs_size_ptr = NULL;
    static get_basic_funcs_ptr_func get_basic_funcs_ptr = NULL;

    void **bse_ptr; 
    void **crt_ptr;
    int *imp_ptr;
    FILE **file_ptr;
    void ***funcs_ptr;
    void **basic_funcs_ptr;
    int *funcs_size_ptr;

    void *bse;
    void *crt;
    int imp;
    FILE *file;
    void **funcs;
    void *basic_funcs;
    int funcs_size;

    void initialize_globals() {
        bse_ptr = get_bse_ptr();
        bse = *bse_ptr;
        crt_ptr = get_crt_ptr();
        crt = *crt_ptr;
        imp_ptr = get_imp_ptr();
        imp = *imp_ptr;
        file_ptr = get_file_ptr();
        file = *file_ptr;
        funcs_ptr = get_funcs_ptr();
        funcs = *funcs_ptr;
        funcs_size_ptr = get_funcs_size_ptr();
        funcs_size = *funcs_size_ptr;
        basic_funcs_ptr = get_basic_funcs_ptr();
        basic_funcs = *basic_funcs_ptr;
    }

    __declspec(dllexport) void loadfuncs() {
        HMODULE hModule = GetModuleHandle(NULL);
        
        get_bse_ptr = (get_bse_ptr_func)GetProcAddress(hModule, "vmc_get_bse_ptr");
        get_crt_ptr = (get_crt_ptr_func)GetProcAddress(hModule, "vmc_get_crt_ptr");
        get_imp_ptr = (get_imp_ptr_func)GetProcAddress(hModule, "vmc_get_imp_ptr");
        get_file_ptr = (get_file_ptr_func)GetProcAddress(hModule, "vmc_get_file_ptr");
        get_funcs_ptr = (get_funcs_ptr_func)GetProcAddress(hModule, "vmc_get_funcs_ptr");
        get_funcs_size_ptr = (get_funcs_size_ptr_func)GetProcAddress(hModule, "vmc_get_funcs_size_ptr");
        get_basic_funcs_ptr = (get_basic_funcs_ptr_func)GetProcAddress(hModule, "vmc_get_basic_funcs_ptr");

        initialize_globals();
        
        *funcs_ptr = realloc(*funcs_ptr, (*funcs_size_ptr + new_funcs_size) * sizeof(void*));
        
        memcpy(*(char***)funcs_ptr + *funcs_size_ptr - 1, new_funcs, new_funcs_size * sizeof(void*));
        *funcs_size_ptr += new_funcs_size - 1;
        *imp_ptr = *(int*)(*crt_ptr += 1*sizeof(int));
    }
#endif

// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>

// // void function(void);

// #define add_instructions //function, ...

// void (*new_funcs[])() = {add_instructions};
// int new_funcs_size = sizeof(new_funcs) / sizeof(new_funcs[0]);

// #define INCLUDE_MAKEDLL

// #include "makedll.c"

// // void function(void){...}


// gcc makedll.c -o makedll.o -c
// gcc dll_name.c makedll.o -o dll_name.dll -shared
// del *.o
