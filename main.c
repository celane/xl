/*
 *  Danny Dulai
 *
 *  Simple, transparent screen locker in X11
 */
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <crypt.h>
#include <string.h>
#include <pwd.h>
#include <security/pam_appl.h>
#include <security/pam_misc.h>


/**
 * @brief This function performs a conversation between our application and the module.
 * It receives an array of messages in the parameters (set by the module) and sets
 * the array of responses in the parameters with the appropriate content.
 * For example, if the module calls this function with a message "login:" we are expected
 * to set the firs response with the username
 *
 * @param num_msg Number of messages from the module
 * @param msg Array of messages (set by the module, should not be modified)
 * @param resp Array of responses this function must set
 * @param appdata_ptr Pointer to additional data. The pointer received is that specified
 * in the pam_conv structure below.
 *
 * @return PAM return code
 */
static char blank[] = "";

int conversation(int num_msg, const struct pam_message **msg,
		 struct pam_response **resp, void *appdata_ptr)
{ /* We malloc an array of num_msg responses */
	struct pam_response *array_resp = (struct pam_response *)malloc(
		num_msg * sizeof(struct pam_response));
	for (int i = 0; i < num_msg; i++) {
          char *text = blank;
          
          /* resp_retcode should be set to zero */
          array_resp[i].resp_retcode = 0;
          
          /* The message received from the module */
          const char *msg_content = msg[i]->msg;
          
          if (strncasecmp("pass",msg_content,4) == 0) {
            text = appdata_ptr;
          } else if (strncasecmp("user",msg_content,4) == 0) {
            text = getlogin();
          } else if (strncasecmp("login",msg_content,5) == 0) {
            text = getlogin();                  
          }

          /* Malloc-ing the resp string of the i-th response */
          array_resp[i].resp = (char *)malloc(strlen(text) + 1);
            
          /* Writing password in the allocated string */
          strcpy(array_resp[i].resp, text);          
	}

	/* setting the param resp with our array of responses */
	*resp = array_resp;

	/* Here we return PAM_SUCCESS, which means that the conversation happened correctly.
	 * You should always check that, for example, the user didn't insert a NULL password etc */
	return PAM_SUCCESS;
}






int
main(int argc, char *argv[])
{
    Display *display;
    Window root;
    int len = 0, i;
    XEvent ev;
    char keybuffer[256], *passwd = NULL;

    
    if (argc > 1) {
	setenv("XLPASSWD", argv[1], 1);
	execl("/proc/self/exe", argv[0], NULL);
	fprintf(stderr, "failed to execute self, use XLPASSWD environment\n");
	exit(1);
    } else if ((passwd = getenv("XLPASSWD"))) {
	passwd = strdup(crypt(passwd, "ax"));
    }

    if (passwd && (strlen(passwd) < 2)) {
	fprintf(stderr, "passwd too short\n");
	exit(1);
    }

    if ((display = XOpenDisplay(NULL)) == NULL) {
	fprintf(stderr, "could not connect to $DISPLAY\n");
	exit(1);
    }

    root = DefaultRootWindow(display);
    XGrabPointer(display, root, 1, ButtonPress, GrabModeAsync, GrabModeAsync, None, None, CurrentTime);
    XGrabKeyboard(display, root, 0, GrabModeAsync, GrabModeAsync, CurrentTime);
    XSelectInput(display, root, KeyPressMask);

    while (XNextEvent(display, &ev), 1) {
	if (ev.type == KeyPress) {
	    KeySym keysym;
	    XComposeStatus compose;
	    if (len > sizeof(keybuffer)-10)
		len = 0;
	    i = XLookupString(&ev.xkey, keybuffer+len, 10, &keysym, &compose);

	    if (keysym == XK_Return) {
		keybuffer[len] = 0;

                if (len && passwd && !strcmp(crypt(keybuffer, passwd), passwd)) {
		    XUngrabKeyboard(display, CurrentTime);
		    XUngrabPointer(display, CurrentTime);
		    exit(0);
                } else if (len && (passwd == NULL)) {
                  int iss;
                  pam_handle_t *pamh = NULL;
                  struct pam_conv pam_convo = {
                    conversation, keybuffer };

                  iss = pam_start("system-auth",(const char*)getlogin(), &pam_convo, &pamh);
                  if (iss != PAM_SUCCESS) {
                    printf("pam_start error %s\n",pam_strerror(pamh,iss));
                  }
                  iss = pam_authenticate(pamh,PAM_DISALLOW_NULL_AUTHTOK);
                  /* if (iss != PAM_SUCCESS) {
                    printf("pam_authenticate error %s\n",pam_strerror(pamh,iss));
                    }  */                
                  pam_end(pamh,iss);
                  if (iss == PAM_SUCCESS) {
		    XUngrabKeyboard(display, CurrentTime);
		    XUngrabPointer(display, CurrentTime);
		    exit(0);
                  }
                  len = 0;
                  
		} else
		    len = 0;
	    } else
		len += i;
	}
    }
}
