#ifndef _STRING_H
#define _STRING_H

#ifndef NULL
#define NULL 0
#endif

#ifndef _SIZE_T
#define _SIZE_T
typedef __np_size_t size_t;
#endif

void   *memchr(const void *, int, size_t);
int     memcmp(const void *, const void *, size_t);
void   *memcpy(void *_NC_RESTRICT, const void *_NC_RESTRICT, size_t);
void   *memmove(void *, const void *, size_t);
void   *memset(void *, int, size_t);
// char   *strcat(char *_NC_RESTRICT, const char *_NC_RESTRICT);
char   *strchr(const char *, int);
int     strcmp(const char *, const char *);
int     strcoll(const char *, const char *);
// char   *strcpy(char *_NC_RESTRICT, const char *_NC_RESTRICT);
size_t  strcspn(const char *, const char *);
// char   *strerror(int);
size_t  strlen(const char *);
char   *strncat(char *_NC_RESTRICT, const char *_NC_RESTRICT, size_t);
int     strncmp(const char *, const char *, size_t);
// char   *strncpy(char *_NC_RESTRICT, const char *_NC_RESTRICT, size_t);
char   *strpbrk(const char *, const char *);
// char   *strrchr(const char *, int);
size_t  strspn(const char *, const char *);
// char   *strstr(const char *, const char *);
char   *strtok(char *_NC_RESTRICT, const char *_NC_RESTRICT);
size_t  strxfrm(char *_NC_RESTRICT, const char *_NC_RESTRICT, size_t);

#endif /* _STRING_H */