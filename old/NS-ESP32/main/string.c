#include <string.h> /* size_t strchr() */
#include <stddef.h> /* size_t */


void *memchr(const void *s, int c, size_t n)
{
    unsigned char *p = (unsigned char*)s;
    while( n-- )
        if( *p != (unsigned char)c )
            p++;
        else
            return p;
    return 0;
}

int memcmp(const void* s1, const void* s2,size_t n)
{
    const unsigned char *p1 = s1, *p2 = s2;
    while(n--)
        if( *p1 != *p2 )
            return *p1 - *p2;
        else
            p1++,p2++;
    return 0;
}

void *memcpy(void *dest, const void *src, size_t n)
{
    char *dp = dest;
    const char *sp = src;
    while (n--)
        *dp++ = *sp++;
    return dest;
}

void *memmove(void *dest, const void *src, size_t n)
{
    unsigned char tmp[n];
    memcpy(tmp,src,n);
    memcpy(dest,tmp,n);
    return dest;
}

void *memset(void *s, int c, size_t n)
{
    unsigned char* p=s;
    while(n--)
        *p++ = (unsigned char)c;
    return s;
}

// char *strcat(char *dest, const char *src)
// {
//     char *ret = dest;
//     while (*dest)
//         dest++;
//     while (*dest++ = *src++)
//         ;
//     return ret;
// }

char *strchr(const char *s, int c)
{
    while (*s != (char)c)
        if (!*s++)
            return 0;
    return (char *)s;
}

int strcmp(const char* s1, const char* s2)
{
    while(*s1 && (*s1==*s2))
        s1++,s2++;
    return *(const unsigned char*)s1-*(const unsigned char*)s2;
}

int strcoll(const char *s1, const char *s2)
{
    char t1[1 + strxfrm(0, s1, 0)];
    strxfrm(t1, s1, sizeof(t1));
    char t2[1 + strxfrm(0, s2, 0)];
    strxfrm(t2, s2, sizeof(t2));
    return strcmp(t1, t2);
}

// #ifdef _NC_RESTRICT
// char *strcpy(char *restrict dest, const char *restrict src)
// #else
// char *strcpy(char *dest, const char* src)
// #endif
// {
//     char *ret = dest;
//     while (*dest++ = *src++)
//         ;
//     return ret;
// }

// char *strerror(int errnum)
// {
//     /*
//         That is actually interpreting the standard by the letter, not intent.
//         We only know about the "C" locale, no more. That's the only mandatory locale anyway.
//     */
//     return errnum ? "There was an error, but I didn't crash yet!" : "No error.";
// }

size_t strcspn(const char *s1, const char *s2)
{
    size_t ret=0;
    while(*s1)
        if(strchr(s2,*s1))
            return ret;
        else
            s1++,ret++;
    return ret;
}


char *strncat(char *dest, const char *src, size_t n)
{
    char *ret = dest;
    while (*dest)
        dest++;
    while (n--)
        if (!(*dest++ = *src++))
            return ret;
    *dest = 0;
    return ret;
}


int strncmp(const char* s1, const char* s2, size_t n)
{
    while(n--)
        if(*s1++!=*s2++)
            return *(unsigned char*)(s1 - 1) - *(unsigned char*)(s2 - 1);
    return 0;
}

// char *strncpy(char *dest, const char *src, size_t n)
// {
//     char *ret = dest;
//     do {
//         if (!n--)
//             return ret;
//     } while (*dest++ = *src++);
//     while (n--)
//         *dest++ = 0;
//     return ret;
// }

char *strpbrk(const char *s1, const char *s2)
{
    while(*s1)
        if(strchr(s2, *s1++))
            return (char*)--s1;
    return 0;
}

// char *strrchr(const char *s, int c)
// {
//     char* ret=0;
//     do {
//         if( *s == (char)c )
//             ret=s;
//     } while(*s++);
//     return ret;
// }

size_t strspn(const char *s1, const char *s2)
{
    size_t ret=0;
    while(*s1 && strchr(s2,*s1++))
        ret++;
    return ret;    
}

// char *strstr(const char *s1, const char *s2)
// {
//     size_t n = strlen(s2);
//     while(*s1)
//         if(!memcmp(s1++,s2,n))
//             return s1-1;
//     return 0;
// }

char *strtok(char * str, const char * delim)
{
    static char* p=0;
    if(str)
        p=str;
    else if(!p)
        return 0;
    str=p+strspn(p,delim);
    p=str+strcspn(str,delim);
    if(p==str)
        return p=0;
    p = *p ? *p=0,p+1 : 0;
    return str;
}

size_t strxfrm(char *dest, const char *src, size_t n)
{
    /* This implementation does not know about any locale but "C"... */
    size_t n2=strlen(src);
    if(n>n2)
        strcpy(dest,src);
    return n2;
}