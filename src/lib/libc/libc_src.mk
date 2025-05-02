# dir errno
LIBC_ERRNO_SRC := strerror.c
LIBC_ERRNO_SRC := $(addprefix $(LIBC_RELATIVE_PATH)/src/errno/, $(LIBC_ERRNO_SRC))

# dir string
LIBC_STRING_SRC := bcmp.c bcopy.c bzero.c memchr.c memcmp.c memcpy.c memmove.c \
	memrchr.c memset.c stpcpy.c stpncpy.c strcasecmp.c strcasestr.c strcat.c \
	strchr.c strchrnul.c strcmp.c strcpy.c strcspn.c strdup.c strerror_r.c \
	strlcpy.c strlen.c strncasecmp.c strncat.c strncmp.c strncpy.c strnlen.c \
	strpbrk.c strrchr.c strsep.c strspn.c strstr.c strtok.c strtok_r.c wcschr.c \
	wcscmp.c wcslen.c wcsncmp.c wcsncpy.c wcsnlen.c wcsstr.c wmemchr.c wmemcmp.c \
	wmemcpy.c wmemmove.c wmemset.c
LIBC_STRING_SRC := $(addprefix $(LIBC_RELATIVE_PATH)/src/string/, $(LIBC_STRING_SRC))

# dir stdlib
LIBC_STDLIB_SRC := abs.c atof.c atoi.c atol.c atoll.c bsearch.c labs.c llabs.c \
	qsort.c strtod.c strtol.c wcstol.c
LIBC_STDLIB_SRC := $(addprefix $(LIBC_RELATIVE_PATH)/src/stdlib/, $(LIBC_STDLIB_SRC))

# dir internal
LIBC_INTERNAL_SRC := floatscan.c intscan.c libc.c shgetc.c syscall_ret.c
LIBC_INTERNAL_SRC := $(addprefix $(LIBC_RELATIVE_PATH)/src/internal/, $(LIBC_INTERNAL_SRC))

# dir time
LIBC_TIME_SRC := __month_to_secs.c __secs_to_tm.c __tm_to_secs.c __tz.c __year_to_secs.c \
	asctime.c asctime_r.c clock_gettime.c ctime.c ctime_r.c gmtime.c gmtime_r.c localtime.c \
	localtime_r.c mktime.c strftime.c strptime.c time.c time_impl.h wcsftime.c
LIBC_TIME_SRC := $(addprefix $(LIBC_RELATIVE_PATH)/src/time/, $(LIBC_TIME_SRC))

# dir misc
LIBC_MISC_SRC := getentropy.c getopt.c
LIBC_MISC_SRC := $(addprefix $(LIBC_RELATIVE_PATH)/src/misc/, $(LIBC_MISC_SRC))

# dir locale
LIBC_LOCALE_SRC := __lctrans.c c_locale.c langinfo.c setlocale.c strcoll.c strxfrm.c \
	wcscoll.c wcsxfrm.c
LIBC_LOCALE_SRC := $(addprefix $(LIBC_RELATIVE_PATH)/src/locale/, $(LIBC_LOCALE_SRC))

# dir ctype
LIBC_CTYPE_SRC := __ctype_get_mb_cur_max.c isalnum.c isalpha.c isascii.c isblank.c \
	iscntrl.c isdigit.c isgraph.c islower.c isprint.c ispunct.c isspace.c isupper.c \
	iswalnum.c iswalpha.c iswblank.c iswcntrl.c iswctype.c iswdigit.c iswgraph.c \
	iswlower.c iswprint.c iswpunct.c iswspace.c iswupper.c iswxdigit.c isxdigit.c \
	toascii.c tolower.c toupper.c towctrans.c
LIBC_CTYPE_SRC := $(addprefix $(LIBC_RELATIVE_PATH)/src/ctype/, $(LIBC_CTYPE_SRC))

# dir temp
LIBC_TEMP_SRC := __randname.c mkostemps.c mkstemp.c mkstemps.c
LIBC_TEMP_SRC := $(addprefix $(LIBC_RELATIVE_PATH)/src/temp/, $(LIBC_TEMP_SRC))

# dir fcntl
LIBC_FCNTL_SRC := creat.c
LIBC_FCNTL_SRC := $(addprefix $(LIBC_RELATIVE_PATH)/src/fcntl/, $(LIBC_FCNTL_SRC))

# dir signal
LIBC_SIGNAL_SRC := sigrtmax.c sigrtmin.c
LIBC_SIGNAL_SRC := $(addprefix $(LIBC_RELATIVE_PATH)/src/signal/, $(LIBC_SIGNAL_SRC))

# dir network
LIBC_NETWORK_SRC := dn_comp.c gai_strerror.c
LIBC_NETWORK_SRC := $(addprefix $(LIBC_RELATIVE_PATH)/src/network/, $(LIBC_NETWORK_SRC))

# dir unistd
LIBC_UNISTD_SRC := _exit.c
LIBC_UNISTD_SRC := $(addprefix $(LIBC_RELATIVE_PATH)/src/unistd/, $(LIBC_UNISTD_SRC))

# dir complex
LIBC_COMPLEX_SRC := cabs.c
LIBC_COMPLEX_SRC := $(addprefix $(LIBC_RELATIVE_PATH)/src/complex/, $(LIBC_COMPLEX_SRC))

# dir termios
LIBC_TERMIOS_SRC := tcgetattr.c tcsetattr.c
LIBC_TERMIOS_SRC := $(addprefix $(LIBC_RELATIVE_PATH)/src/termios/, $(LIBC_TERMIOS_SRC))

# dir stdio
LIBC_STDIO_SRC := __fdopen.c __fmodeflags.c __lockfile.c __overflow.c __stdio_close.c \
	__stdio_read.c __stdio_seek.c __stdio_write.c __stdout_write.c __string_read.c \
	__toread.c __towrite.c __uflow.c asprintf.c clearerr.c fclose.c feof.c ferror.c \
	fflush.c fgetc.c fgetpos.c fgets.c fgetwc.c fileno.c fopen.c fprintf.c fputc.c \
	fputs.c fputwc.c fread.c freopen.c fscanf.c fseek.c fsetpos.c ftell.c fwide.c \
	fwrite.c getc.c getc.h getchar.c gets.c getwc.c ofl.c ofl_add.c perror.c printf.c \
	putc.c putc.h putchar.c puts.c putw.c putwc.c remove.c rewind.c scanf.c setbuf.c \
	setbuffer.c setvbuf.c snprintf.c sprintf.c sscanf.c stderr.c stdin.c stdout.c \
	swprintf.c tmpnam.c ungetc.c ungetwc.c vasprintf.c vfprintf.c vfscanf.c vfwprintf.c \
	vfwscanf.c vprintf.c vscanf.c vsnprintf.c vsprintf.c vsscanf.c vswprintf.c
LIBC_STDIO_SRC := $(addprefix $(LIBC_RELATIVE_PATH)/src/stdio/, $(LIBC_STDIO_SRC))

# dir exit
LIBC_EXIT_SRC := _Exit.c abort.c assert.c atexit.c exit.c
LIBC_EXIT_SRC := $(addprefix $(LIBC_RELATIVE_PATH)/src/exit/, $(LIBC_EXIT_SRC))

# dir prng
LIBC_PRNG_SRC := __rand48_step.c __seed48.c lrand48.c rand.c rand48.h random.c seed48.c srand48.c
LIBC_PRNG_SRC := $(addprefix $(LIBC_RELATIVE_PATH)/src/prng/, $(LIBC_PRNG_SRC))

# dir multibyte
LIBC_MULTIBYTE_SRC := btowc.c c32rtomb.c internal.c mblen.c mbrlen.c mbrtoc32.c \
	mbrtowc.c mbsinit.c mbsnrtowcs.c mbsrtowcs.c mbstowcs.c mbtowc.c wcrtomb.c \
	wcsnrtombs.c wcsrtombs.c wctob.c wctomb.c
LIBC_MULTIBYTE_SRC := $(addprefix $(LIBC_RELATIVE_PATH)/src/multibyte/, $(LIBC_MULTIBYTE_SRC))

# dir env
LIBC_ENV_SRC := __environ.c getenv.c
LIBC_ENV_SRC := $(addprefix $(LIBC_RELATIVE_PATH)/src/env/, $(LIBC_ENV_SRC))

# dir linux
LIBC_LINUX_SRC := stime.c
LIBC_LINUX_SRC := $(addprefix $(LIBC_RELATIVE_PATH)/src/linux/, $(LIBC_LINUX_SRC))

# dir process
LIBC_PROCESS_SRC := system.c
LIBC_PROCESS_SRC := $(addprefix $(LIBC_RELATIVE_PATH)/src/process/, $(LIBC_PROCESS_SRC))

# dir legacy
LIBC_LEGACY_SRC := err.c getpagesize.c
LIBC_LEGACY_SRC := $(addprefix $(LIBC_RELATIVE_PATH)/src/legacy/, $(LIBC_LEGACY_SRC))

# dir thread
LIBC_THREAD_SRC := pthread_getspecific.c pthread_key_create.c pthread_setspecific.c
LIBC_THREAD_SRC := $(addprefix $(LIBC_RELATIVE_PATH)/src/thread/, $(LIBC_THREAD_SRC))
