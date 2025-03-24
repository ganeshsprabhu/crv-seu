__extension__
typedef unsigned long size_t;
typedef long ssize_t;
typedef long off_t;
typedef struct _IO_FILE FILE;
typedef __builtin_va_list va_list;
typedef __builtin_va_list __isoc_va_list;
typedef union _G_fpos64_t {
 char __opaque[16];
 long long __lldata;
 double __align;
} fpos_t;
extern FILE *const stdin;
extern FILE *const stdout;
extern FILE *const stderr;
FILE *fopen(const char *restrict, const char *restrict);
FILE *freopen(const char *restrict, const char *restrict, FILE *restrict);
int fclose(FILE *);
int remove(const char *);
int rename(const char *, const char *);
int feof(FILE *);
int ferror(FILE *);
int fflush(FILE *);
void clearerr(FILE *);
int fseek(FILE *, long, int);
long ftell(FILE *);
void rewind(FILE *);
int fgetpos(FILE *restrict, fpos_t *restrict);
int fsetpos(FILE *, const fpos_t *);
size_t fread(void *restrict, size_t, size_t, FILE *restrict);
size_t fwrite(const void *restrict, size_t, size_t, FILE *restrict);
int fgetc(FILE *);
int getc(FILE *);
int getchar(void);
int ungetc(int, FILE *);
int fputc(int, FILE *);
int putc(int, FILE *);
int putchar(int);
char *fgets(char *restrict, int, FILE *restrict);
int fputs(const char *restrict, FILE *restrict);
int puts(const char *);
int printf(const char *restrict, ...);
int fprintf(FILE *restrict, const char *restrict, ...);
int sprintf(char *restrict, const char *restrict, ...);
int snprintf(char *restrict, size_t, const char *restrict, ...);
int vprintf(const char *restrict, __isoc_va_list);
int vfprintf(FILE *restrict, const char *restrict, __isoc_va_list);
int vsprintf(char *restrict, const char *restrict, __isoc_va_list);
int vsnprintf(char *restrict, size_t, const char *restrict, __isoc_va_list);
int scanf(const char *restrict, ...);
int fscanf(FILE *restrict, const char *restrict, ...);
int sscanf(const char *restrict, const char *restrict, ...);
int vscanf(const char *restrict, __isoc_va_list);
int vfscanf(FILE *restrict, const char *restrict, __isoc_va_list);
int vsscanf(const char *restrict, const char *restrict, __isoc_va_list);
void perror(const char *);
int setvbuf(FILE *restrict, char *restrict, int, size_t);
void setbuf(FILE *restrict, char *restrict);
char *tmpnam(char *);
FILE *tmpfile(void);
FILE *fmemopen(void *restrict, size_t, const char *restrict);
FILE *open_memstream(char **, size_t *);
FILE *fdopen(int, const char *);
FILE *popen(const char *, const char *);
int pclose(FILE *);
int fileno(FILE *);
int fseeko(FILE *, off_t, int);
off_t ftello(FILE *);
int dprintf(int, const char *restrict, ...);
int vdprintf(int, const char *restrict, __isoc_va_list);
void flockfile(FILE *);
int ftrylockfile(FILE *);
void funlockfile(FILE *);
int getc_unlocked(FILE *);
int getchar_unlocked(void);
int putc_unlocked(int, FILE *);
int putchar_unlocked(int);
ssize_t getdelim(char **restrict, size_t *restrict, int, FILE *restrict);
ssize_t getline(char **restrict, size_t *restrict, FILE *restrict);
int renameat(int, const char *, int, const char *);
char *ctermid(char *);
char *tempnam(const char *, const char *);
char *cuserid(char *);
void setlinebuf(FILE *);
void setbuffer(FILE *, char *, size_t);
int fgetc_unlocked(FILE *);
int fputc_unlocked(int, FILE *);
int fflush_unlocked(FILE *);
size_t fread_unlocked(void *, size_t, size_t, FILE *);
size_t fwrite_unlocked(const void *, size_t, size_t, FILE *);
void clearerr_unlocked(FILE *);
int feof_unlocked(FILE *);
int ferror_unlocked(FILE *);
int fileno_unlocked(FILE *);
int getw(FILE *);
int putw(int, FILE *);
char *fgetln(FILE *, size_t *);
int asprintf(char **, const char *, ...);
int vasprintf(char **, const char *, __isoc_va_list);
__extension__
typedef int wchar_t;
int atoi (const char *);
long atol (const char *);
long long atoll (const char *);
double atof (const char *);
float strtof (const char *restrict, char **restrict);
double strtod (const char *restrict, char **restrict);
long double strtold (const char *restrict, char **restrict);
long strtol (const char *restrict, char **restrict, int);
unsigned long strtoul (const char *restrict, char **restrict, int);
long long strtoll (const char *restrict, char **restrict, int);
unsigned long long strtoull (const char *restrict, char **restrict, int);
int rand (void);
void srand (unsigned);
void *malloc (size_t);
void *calloc (size_t, size_t);
void *realloc (void *, size_t);
void free (void *);
void *aligned_alloc(size_t, size_t);
int atexit (void (*) (void));
int at_quick_exit (void (*) (void));
char *getenv (const char *);
int system (const char *);
void *bsearch (const void *, const void *, size_t, size_t, int (*)(const void *, const void *));
void qsort (void *, size_t, size_t, int (*)(const void *, const void *));
int abs (int);
long labs (long);
long long llabs (long long);
typedef struct { int quot, rem; } div_t;
typedef struct { long quot, rem; } ldiv_t;
typedef struct { long long quot, rem; } lldiv_t;
div_t div (int, int);
ldiv_t ldiv (long, long);
lldiv_t lldiv (long long, long long);
int mblen (const char *, size_t);
int mbtowc (wchar_t *restrict, const char *restrict, size_t);
int wctomb (char *, wchar_t);
size_t mbstowcs (wchar_t *restrict, const char *restrict, size_t);
size_t wcstombs (char *restrict, const wchar_t *restrict, size_t);
size_t __ctype_get_mb_cur_max(void);
int posix_memalign (void **, size_t, size_t);
int setenv (const char *, const char *, int);
int unsetenv (const char *);
int mkstemp (char *);
int mkostemp (char *, int);
char *mkdtemp (char *);
int getsubopt (char **, char *const *, char **);
int rand_r (unsigned *);
char *realpath (const char *restrict, char *restrict);
long int random (void);
void srandom (unsigned int);
char *initstate (unsigned int, char *, size_t);
char *setstate (char *);
int putenv (char *);
int posix_openpt (int);
int grantpt (int);
int unlockpt (int);
char *ptsname (int);
char *l64a (long);
long a64l (const char *);
void setkey (const char *);
double drand48 (void);
double erand48 (unsigned short [3]);
long int lrand48 (void);
long int nrand48 (unsigned short [3]);
long mrand48 (void);
long jrand48 (unsigned short [3]);
void srand48 (long);
unsigned short *seed48 (unsigned short [3]);
void lcong48 (unsigned short [7]);
void *alloca(size_t);
char *mktemp (char *);
int mkstemps (char *, int);
int mkostemps (char *, int, int);
void *valloc (size_t);
void *memalign(size_t, size_t);
int getloadavg(double *, int);
int clearenv(void);
void *reallocarray (void *, size_t, size_t);
void qsort_r (void *, size_t, size_t, int (*)(const void *, const void *, void *), void *);
typedef struct {
    float Kp, Ki, Kd, Kaw, T_C, T;
    float max, min, max_rate;
    float integral, err_prev, deriv_prev;
    float command_prev, command_sat_prev;
} PID;
typedef struct {
    float m, k, F_max, F_min, T;
    float v, z;
} Object;
float PID_Object_Step(PID *pid, Object *obj, float measurement, float setpoint)
{
    float err = setpoint - measurement;
    pid->integral += pid->Ki * err * pid->T + pid->Kaw * (pid->command_sat_prev - pid->command_prev) * pid->T;
    float deriv_filt = (err - pid->err_prev + pid->T_C * pid->deriv_prev) / (pid->T + pid->T_C);
    pid->err_prev = err;
    pid->deriv_prev = deriv_filt;
    float command = pid->Kp * err + pid->integral + pid->Kd * deriv_filt;
    pid->command_prev = command;
    float command_sat;
    if (command > pid->max) command_sat = pid->max;
    else if (command < pid->min) command_sat = pid->min;
    else command_sat = command;
    if (command_sat > pid->command_sat_prev + pid->max_rate * pid->T)
        command_sat = pid->command_sat_prev + pid->max_rate * pid->T;
    else if (command_sat < pid->command_sat_prev - pid->max_rate * pid->T)
        command_sat = pid->command_sat_prev - pid->max_rate * pid->T;
    pid->command_sat_prev = command_sat;
    float F_sat;
    if (command_sat > obj->F_max) F_sat = obj->F_max;
    else if (command_sat < obj->F_min) F_sat = obj->F_min;
    else F_sat = command_sat;
    float dv_dt = (F_sat - obj->k * obj->v) / obj->m;
    obj->v += dv_dt * obj->T;
    obj->z += obj->v * obj->T;
    return obj->z;
}
int main()
{
    float t = 0;
    int i = 0;
    float command1 = 0, command2 = 0;
    float stp1 = 100, stp2 = 50;
    float z1 = 0, z2 = 0;
    PID pid1 = {1, 0.1, 5, 0.1, 1, 0.1, 100, -100, 40, 0, 0, 0, 0, 0};
    PID pid2 = {1.8, 0.3, 7, 0.3, 1, 0.1, 100, -100, 40, 0, 0, 0, 0, 0};
    Object obj1 = {10, 0.5, 100, -100, 0.1, 0, 0};
    Object obj2 = {10, 0.5, 100, -100, 0.1, 0, 0};
    while (i < 1000)
    {
        if (t >= 60)
        {
            stp1 = 200;
            stp2 = 150;
        }
        z1 = PID_Object_Step(&pid1, &obj1, z1, stp1);
        z2 = PID_Object_Step(&pid2, &obj2, z2, stp2);
        t += 0.1;
        i++;
    }
    return 0;
}
