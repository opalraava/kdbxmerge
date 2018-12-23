#include "askpass_tty.h"

#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <termios.h>

#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/select.h>
#include <fcntl.h>
#include <unistd.h>

static int interactive_pass(const char* prompt, char* pass, size_t maxlen, int timeout);
static int untimed_read(int fd, char* pass, size_t maxlen);
static int timed_read(int fd, char* pass, size_t maxlen, long timeout);

/*
 * the code used as inspiration is utils_password.c from the 
 * cryptsetup package (hosted on gitlab).
 *
 * In the cryptsetup implementation:
 *
 * - my first problem is that if the verified passwords don't match,
 * we return -EPERM. but we do read() select() and so on, which might
 * return EPERM if they fail.
 * - second issue, in crypt_get_key_tty() we do a pass[MAX]=0 but
 * when verifying we don't do a pass_verify[MAX] = 0. why? interactive_login()
 * is called in exactly the same way for getting both passwords. Also,
 * 'pass_verify' is not allocated with (MAX+1), where 'pass' is.
 * - I don't see the point of the tools_passphrase_msg() function (yet).
 * - in untimed_read(), we have a *pass=0 should be *pass='\0';
 *
 * In my implementation, I have the following issues:
 * - I use malloc()/free() instead of some secure memory malloc thing
 * - I don't check for blocking signals
 * - I don't implement the keyfile stuff
 * - I don't use password quality checking in any way.
 */


int
askpass_tty(const char* prompt, char** key, int timeout, int verify)
{
  const int key_size_max = ASKPASS_TTY_PASSWORD_SIZE_MAX;
  char* pass = NULL;

  /* making this NULL ensures we can always free() it. */
  *key = NULL;

  pass = malloc(key_size_max+1);
  if (!pass)
    return -ENOMEM;

  if (interactive_pass(prompt ? prompt : "Enter passphrase: ", pass, key_size_max, timeout)) {
    free(pass);
    return -EINVAL;
  }

  pass[key_size_max] = '\0';	/* see notes */

  if (verify)
    {
      char* pass_verify = malloc(key_size_max);
      if (!pass_verify) {
	free(pass);
	return -ENOMEM;
      }

      if (interactive_pass("Verify passphrase: ",
			   pass_verify, key_size_max, timeout))
	{
	  free(pass);
	  free(pass_verify);
	  return -EINVAL;
	}

      if (strncmp(pass, pass_verify, key_size_max)) {
	free(pass);
	free(pass_verify);
	return -EPERM;		/* see notes */
      }

      free(pass_verify);	/* not burning it, perhaps bzero() */
    }
  
  *key = pass;
  return 0;
}



/* 
 * The difference with the canonical implementation is that we fail always if /dev/tty is
 * not available. this implies that 'infd' == 'outfd' which makes it simpler.
 */

static int
interactive_pass(const char* prompt, char* pass, size_t maxlen, int timeout)
{
  struct termios orig, tmp;
  int fd, err;

  fd = open("/dev/tty", O_RDWR);
  if (fd < 0) return -1;

  if (tcgetattr(fd, &orig)) { close(fd); return -1; }

  memcpy(&tmp, &orig, sizeof(tmp));
  tmp.c_lflag &= ~ECHO;

  if (prompt) {
    if (write(fd, prompt, strlen(prompt)) < 0) { close(fd); return -1; }
  }

  tcsetattr(fd, TCSAFLUSH, &tmp);
  if (timeout)
    err = timed_read(fd, pass, maxlen, (long)timeout);
  else
    err = untimed_read(fd, pass, maxlen);
  tcsetattr(fd, TCSAFLUSH, &orig);
  
  if (err) { close(fd); return -1; }
  
  write(fd, "\n", 1);
  close(fd);
  return 0;
}



static int
untimed_read(int fd, char* pass, size_t maxlen)
{
  ssize_t i;

  i = read(fd, pass, maxlen);
  if (i < 0) return -1;
  if (i == 0) return -1; 	/* indicates EOF */
  
  pass[i-1] = '\0';
  return 0;
}

static int
timed_read(int fd, char* pass, size_t maxlen, long timeout)
{
  struct timeval t;
  fd_set fds;

  FD_ZERO(&fds);
  FD_SET(fd,&fds);
  t.tv_sec = timeout;
  t.tv_usec = 0;

  if (select(fd+1, &fds, NULL, NULL, &t) <= 0)
    return -1;

  return untimed_read(fd,pass,maxlen);
}


