#include <iostream>
#include <string>
#include <cstring>
#include <cstdio>

#define MAXSTR 10000 

int main()
{

    int retval = -1;
    
    char message[] = "[>> Empty Message <<]";
    
    // open mail service
    FILE *mailpipe = popen("/usr/lib/sendmail -t", "w");
    
    // open status page
    FILE *systat = fopen("/proc/stat", "r");
    
    if(systat != NULL)
    {
        size_t len = fread(message, sizeof(char), MAXSTR, systat);
        
        if(len == 0)
        {
            std::cout << ">> Failed to read from /proc/stat !" << std::endl;
        }
        
        else
        {
            std::cout << ">> Read from /proc/stat OK !\n" << std::endl;
        }
        
        fclose(systat);
    }
    
    else
    {
        std::cout << ">> Failed to open /proc/stat !\n" << std::endl;
    }
    
    if (mailpipe != NULL)
    {
        fprintf(mailpipe, "To: cosailer@gmail.com\n");
        fprintf(mailpipe, "From: root@erlangen.de\n");
        fprintf(mailpipe, "Subject: this is a test mail\n\n");
        fwrite(message, 1, strlen(message), mailpipe);
        fwrite(".\n", 1, 2, mailpipe);
        pclose(mailpipe);
        retval = 0;
        
        std::cout << ">> Mail sent successfully !\n" << std::endl;
    }
     
    else
    {
        std::cout << ">> Failed to invoke sendmail !\n" << std::endl;
    }
    
    return 0;
     
}


