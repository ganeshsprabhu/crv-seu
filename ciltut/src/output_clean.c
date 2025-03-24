typedef unsigned long size_t;
typedef long ssize_t;
typedef long off_t;
struct _IO_FILE;
typedef struct _IO_FILE FILE;
typedef __builtin_va_list va_list;
typedef __builtin_va_list __isoc_va_list;
union _G_fpos64_t {
   char __opaque[16] ;
   long long __lldata ;
   double __align ;
};
typedef union _G_fpos64_t fpos_t;
typedef int wchar_t;
struct __anonstruct_div_t_1 {
   int quot ;
   int rem ;
};
typedef struct __anonstruct_div_t_1 div_t;
struct __anonstruct_ldiv_t_2 {
   long quot ;
   long rem ;
};
typedef struct __anonstruct_ldiv_t_2 ldiv_t;
struct __anonstruct_lldiv_t_3 {
   long long quot ;
   long long rem ;
};
typedef struct __anonstruct_lldiv_t_3 lldiv_t;
struct __anonstruct_PID_4 {
   float Kp ;
   float Ki ;
   float Kd ;
   float Kaw ;
   float T_C ;
   float T ;
   float max ;
   float min ;
   float max_rate ;
   float integral ;
   float err_prev ;
   float deriv_prev ;
   float command_prev ;
   float command_sat_prev ;
};
typedef struct __anonstruct_PID_4 PID;
struct __anonstruct_Object_5 {
   float m ;
   float k ;
   float F_max ;
   float F_min ;
   float T ;
   float v ;
   float z ;
};
typedef struct __anonstruct_Object_5 Object;
extern FILE * const stdin ;
extern FILE * const stdout ;
extern FILE * const stderr ;
extern FILE *fopen(char const * __restrict , char const * __restrict ) ;
extern FILE *freopen(char const * __restrict , char const * __restrict , FILE * __restrict ) ;
extern int fclose(FILE * ) ;
extern int remove(char const * ) ;
extern int rename(char const * , char const * ) ;
extern int feof(FILE * ) ;
extern int ferror(FILE * ) ;
extern int fflush(FILE * ) ;
extern void clearerr(FILE * ) ;
extern int fseek(FILE * , long , int ) ;
extern long ftell(FILE * ) ;
extern void rewind(FILE * ) ;
extern int fgetpos(FILE * __restrict , fpos_t * __restrict ) ;
extern int fsetpos(FILE * , fpos_t const * ) ;
extern size_t fread(void * __restrict , size_t , size_t , FILE * __restrict ) ;
extern size_t fwrite(void const * __restrict , size_t , size_t , FILE * __restrict ) ;
extern int fgetc(FILE * ) ;
extern int getc(FILE * ) ;
extern int getchar(void) ;
extern int ungetc(int , FILE * ) ;
extern int fputc(int , FILE * ) ;
extern int putc(int , FILE * ) ;
extern int putchar(int ) ;
extern char *fgets(char * __restrict , int , FILE * __restrict ) ;
extern int fputs(char const * __restrict , FILE * __restrict ) ;
extern int puts(char const * ) ;
extern int printf(char const * __restrict , ...) ;
extern int fprintf(FILE * __restrict , char const * __restrict , ...) ;
extern int sprintf(char * __restrict , char const * __restrict , ...) ;
extern int snprintf(char * __restrict , size_t , char const * __restrict , ...) ;
extern int vprintf(char const * __restrict , __isoc_va_list ) ;
extern int vfprintf(FILE * __restrict , char const * __restrict , __isoc_va_list ) ;
extern int vsprintf(char * __restrict , char const * __restrict , __isoc_va_list ) ;
extern int vsnprintf(char * __restrict , size_t , char const * __restrict ,
                     __isoc_va_list ) ;
extern int scanf(char const * __restrict , ...) ;
extern int fscanf(FILE * __restrict , char const * __restrict , ...) ;
extern int sscanf(char const * __restrict , char const * __restrict , ...) ;
extern int vscanf(char const * __restrict , __isoc_va_list ) ;
extern int vfscanf(FILE * __restrict , char const * __restrict , __isoc_va_list ) ;
extern int vsscanf(char const * __restrict , char const * __restrict , __isoc_va_list ) ;
extern void perror(char const * ) ;
extern int setvbuf(FILE * __restrict , char * __restrict , int , size_t ) ;
extern void setbuf(FILE * __restrict , char * __restrict ) ;
extern char *tmpnam(char * ) ;
extern FILE *tmpfile(void) ;
extern FILE *fmemopen(void * __restrict , size_t , char const * __restrict ) ;
extern FILE *open_memstream(char ** , size_t * ) ;
extern FILE *fdopen(int , char const * ) ;
extern FILE *popen(char const * , char const * ) ;
extern int pclose(FILE * ) ;
extern int fileno(FILE * ) ;
extern int fseeko(FILE * , off_t , int ) ;
extern off_t ftello(FILE * ) ;
extern int dprintf(int , char const * __restrict , ...) ;
extern int vdprintf(int , char const * __restrict , __isoc_va_list ) ;
extern void flockfile(FILE * ) ;
extern int ftrylockfile(FILE * ) ;
extern void funlockfile(FILE * ) ;
extern int getc_unlocked(FILE * ) ;
extern int getchar_unlocked(void) ;
extern int putc_unlocked(int , FILE * ) ;
extern int putchar_unlocked(int ) ;
extern ssize_t getdelim(char ** __restrict , size_t * __restrict , int , FILE * __restrict ) ;
extern ssize_t getline(char ** __restrict , size_t * __restrict , FILE * __restrict ) ;
extern int renameat(int , char const * , int , char const * ) ;
extern char *ctermid(char * ) ;
extern char *tempnam(char const * , char const * ) ;
extern char *cuserid(char * ) ;
extern void setlinebuf(FILE * ) ;
extern void setbuffer(FILE * , char * , size_t ) ;
extern int fgetc_unlocked(FILE * ) ;
extern int fputc_unlocked(int , FILE * ) ;
extern int fflush_unlocked(FILE * ) ;
extern size_t fread_unlocked(void * , size_t , size_t , FILE * ) ;
extern size_t fwrite_unlocked(void const * , size_t , size_t , FILE * ) ;
extern void clearerr_unlocked(FILE * ) ;
extern int feof_unlocked(FILE * ) ;
extern int ferror_unlocked(FILE * ) ;
extern int fileno_unlocked(FILE * ) ;
extern int getw(FILE * ) ;
extern int putw(int , FILE * ) ;
extern char *fgetln(FILE * , size_t * ) ;
extern int asprintf(char ** , char const * , ...) ;
extern int vasprintf(char ** , char const * , __isoc_va_list ) ;
extern int atoi(char const * ) ;
extern long atol(char const * ) ;
extern long long atoll(char const * ) ;
extern double atof(char const * ) ;
extern float strtof(char const * __restrict , char ** __restrict ) ;
extern double strtod(char const * __restrict , char ** __restrict ) ;
extern long double strtold(char const * __restrict , char ** __restrict ) ;
extern long strtol(char const * __restrict , char ** __restrict , int ) ;
extern unsigned long strtoul(char const * __restrict , char ** __restrict ,
                             int ) ;
extern long long strtoll(char const * __restrict , char ** __restrict , int ) ;
extern unsigned long long strtoull(char const * __restrict , char ** __restrict ,
                                   int ) ;
extern int rand(void) ;
extern void srand(unsigned int ) ;
extern void *malloc(size_t ) ;
extern void *calloc(size_t , size_t ) ;
extern void *realloc(void * , size_t ) ;
extern void free(void * ) ;
extern void *aligned_alloc(size_t , size_t ) ;
extern int atexit(void (*)(void) ) ;
extern int at_quick_exit(void (*)(void) ) ;
extern char *getenv(char const * ) ;
extern int system(char const * ) ;
extern void *bsearch(void const * , void const * , size_t , size_t , int (*)(void const * ,
                                                                                   void const * ) ) ;
extern void qsort(void * , size_t , size_t , int (*)(void const * , void const * ) ) ;
extern int abs(int ) ;
extern long labs(long ) ;
extern long long llabs(long long ) ;
extern div_t div(int , int ) ;
extern ldiv_t ldiv(long , long ) ;
extern lldiv_t lldiv(long long , long long ) ;
extern int mblen(char const * , size_t ) ;
extern int mbtowc(wchar_t * __restrict , char const * __restrict , size_t ) ;
extern int wctomb(char * , wchar_t ) ;
extern size_t mbstowcs(wchar_t * __restrict , char const * __restrict , size_t ) ;
extern size_t wcstombs(char * __restrict , wchar_t const * __restrict , size_t ) ;
extern size_t __ctype_get_mb_cur_max(void) ;
extern int posix_memalign(void ** , size_t , size_t ) ;
extern int setenv(char const * , char const * , int ) ;
extern int unsetenv(char const * ) ;
extern int mkstemp(char * ) ;
extern int mkostemp(char * , int ) ;
extern char *mkdtemp(char * ) ;
extern int getsubopt(char ** , char * const * , char ** ) ;
extern int rand_r(unsigned int * ) ;
extern char *realpath(char const * __restrict , char * __restrict ) ;
extern long random(void) ;
extern void srandom(unsigned int ) ;
extern char *initstate(unsigned int , char * , size_t ) ;
extern char *setstate(char * ) ;
extern int putenv(char * ) ;
extern int posix_openpt(int ) ;
extern int grantpt(int ) ;
extern int unlockpt(int ) ;
extern char *ptsname(int ) ;
extern char *l64a(long ) ;
extern long a64l(char const * ) ;
extern void setkey(char const * ) ;
extern double drand48(void) ;
extern double erand48(unsigned short * ) ;
extern long lrand48(void) ;
extern long nrand48(unsigned short * ) ;
extern long mrand48(void) ;
extern long jrand48(unsigned short * ) ;
extern void srand48(long ) ;
extern unsigned short *seed48(unsigned short * ) ;
extern void lcong48(unsigned short * ) ;
extern void *alloca(size_t ) ;
extern char *mktemp(char * ) ;
extern int mkstemps(char * , int ) ;
extern int mkostemps(char * , int , int ) ;
extern void *valloc(size_t ) ;
extern void *memalign(size_t , size_t ) ;
extern int getloadavg(double * , int ) ;
extern int clearenv(void) ;
extern void *reallocarray(void * , size_t , size_t ) ;
extern void qsort_r(void * , size_t , size_t , int (*)(void const * , void const * ,
                                                         void * ) , void * ) ;
float PID_Object_Step(PID *pid , Object *obj , float measurement , float setpoint )
{
  float err ;
  float deriv_filt ;
  float command ;
  float command_sat ;
  float F_sat ;
  float dv_dt ;
  {
  err = setpoint - measurement;
  pid->integral += (pid->Ki * err) * pid->T + (pid->Kaw * (pid->command_sat_prev - pid->command_prev)) * pid->T;
  deriv_filt = ((err - pid->err_prev) + pid->T_C * pid->deriv_prev) / (pid->T + pid->T_C);
  pid->err_prev = err;
  pid->deriv_prev = deriv_filt;
  command = (pid->Kp * err + pid->integral) + pid->Kd * deriv_filt;
  pid->command_prev = command;
  if (command > pid->max) {
    command_sat = pid->max;
  } else
  if (command < pid->min) {
    command_sat = pid->min;
  } else {
    command_sat = command;
  }
  if (command_sat > pid->command_sat_prev + pid->max_rate * pid->T) {
    command_sat = pid->command_sat_prev + pid->max_rate * pid->T;
  } else
  if (command_sat < pid->command_sat_prev - pid->max_rate * pid->T) {
    command_sat = pid->command_sat_prev - pid->max_rate * pid->T;
  }
  pid->command_sat_prev = command_sat;
  if (command_sat > obj->F_max) {
    F_sat = obj->F_max;
  } else
  if (command_sat < obj->F_min) {
    F_sat = obj->F_min;
  } else {
    F_sat = command_sat;
  }
  dv_dt = (F_sat - obj->k * obj->v) / obj->m;
  obj->v += dv_dt * obj->T;
  simulate_seu_main(& obj->z);
  obj->z += obj->v * obj->T;
  return (obj->z);
}
}
int main(void)
{
  float t ;
  int i ;
  float command1 ;
  float command2 ;
  float stp1 ;
  float stp2 ;
  float z1 ;
  float z2 ;
  PID pid1 ;
  PID pid2 ;
  Object obj1 ;
  Object obj2 ;
  {
  t = (float )0;
  i = 0;
  command1 = (float )0;
  command2 = (float )0;
  stp1 = (float )100;
  stp2 = (float )50;
  z1 = (float )0;
  z2 = (float )0;
  pid1.Kp = (float )1;
  pid1.Ki = (float )0.1;
  pid1.Kd = (float )5;
  pid1.Kaw = (float )0.1;
  pid1.T_C = (float )1;
  pid1.T = (float )0.1;
  pid1.max = (float )100;
  pid1.min = (float )-100;
  pid1.max_rate = (float )40;
  pid1.integral = (float )0;
  pid1.err_prev = (float )0;
  pid1.deriv_prev = (float )0;
  pid1.command_prev = (float )0;
  pid1.command_sat_prev = (float )0;
  pid2.Kp = (float )1.8;
  pid2.Ki = (float )0.3;
  pid2.Kd = (float )7;
  pid2.Kaw = (float )0.3;
  pid2.T_C = (float )1;
  pid2.T = (float )0.1;
  pid2.max = (float )100;
  pid2.min = (float )-100;
  pid2.max_rate = (float )40;
  pid2.integral = (float )0;
  pid2.err_prev = (float )0;
  pid2.deriv_prev = (float )0;
  pid2.command_prev = (float )0;
  pid2.command_sat_prev = (float )0;
  obj1.m = (float )10;
  obj1.k = (float )0.5;
  obj1.F_max = (float )100;
  obj1.F_min = (float )-100;
  obj1.T = (float )0.1;
  obj1.v = (float )0;
  simulate_seu_main(& obj1.z);
  obj1.z = (float )0;
  obj2.m = (float )10;
  obj2.k = (float )0.5;
  obj2.F_max = (float )100;
  obj2.F_min = (float )-100;
  obj2.T = (float )0.1;
  obj2.v = (float )0;
  obj2.z = (float )0;
  while (i < 1000) {
    if (t >= (float )60) {
      stp1 = (float )200;
      stp2 = (float )150;
    }
    z1 = PID_Object_Step(& pid1, & obj1, z1, stp1);
    z2 = PID_Object_Step(& pid2, & obj2, z2, stp2);
    t = (float )((double )t + 0.1);
    i ++;
  }
  return (0);
}
}
