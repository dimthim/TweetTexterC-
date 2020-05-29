#include <stdio.h>
#include <string.h>
#include <curl.h>
#include <windows.h>
#include <stdlib.h>

#define Assert(Expression) if(!Expression){*(int *)0 = 0;}
#define FROM    "<1s22p63d104f14@gmail.com>"
#define TO      "<3149414314@vtext.com>"
#define CC      "<1s22p63d104f14@gmail.com>"

struct upload_status {
    int lines_read;
};

static char *payload_text[] =
{
    "Date: Mon, 29 Nov 2010 21:54:29 +1100\r\n",
    "To: " TO "\r\n", // 1 phone number
    "From: " FROM "\r\n",
    "Cc: " CC "\r\n",
    "Message-ID: <dcd7cb36-11db-487a-9f3a-e652a9458efd@"
    "rfcpedant.example.org>\r\n",
    "Subject: New Tweet!\r\n", //5 Subject
    "\r\n",
    "New Tweet Posted\r\n", //7 Message
    "\r\n",
    NULL
};

static size_t payload_source(void *ptr, size_t size, size_t nmemb, void *userp)
{
    struct upload_status *upload_ctx = (struct upload_status *)userp;
    const char *data;

    if((size == 0) || (nmemb == 0) || ((size*nmemb) < 1)) {
        return 0;
    }

    data = payload_text[upload_ctx->lines_read];

    if(data) {
        size_t len = strlen(data);
        memcpy(ptr, data, len);
        upload_ctx->lines_read++;

        return len;
    }

    return 0;
}



//TODO: Learn how to have window disappear, but keep app running in the background.
// TODO: open and close files more frequently. At each attempt to open, check that the seulting pointer is not null


int main()
{

    const int MinuteInMilliseconds = 1000 * 60;
    const int MinutesOfSleep = 1;
    int StatusInput = 0;
    bool ValidInput = 0;
    
    char PhoneNumber[20] = {};
    char Alias[20] = {};


    
    while(!ValidInput)
    {
        
        FILE *AddressFile = NULL;
        AddressFile = fopen("UserAddresses.txt","r+");
        fseek(AddressFile,0,SEEK_END);
        long int AddressFileSize = ftell(AddressFile);
        fclose(AddressFile);
        
        if(AddressFileSize < 2)
        {
            StatusInput = 1;
        }
        else
        {
            printf("%s","If you are a new user, press 1.\nIf you are a returning user, press 2.\nAfter making your selection, hit enter.\n");
            scanf("%i",&StatusInput);
        }
        // ask if user is a first time or returning.
        

        if(StatusInput == 1)
        {
            
            ValidInput = 1;
            printf("%s","New User Detected\n");
            // if new user, ask for their name and phone number. Save their phone number to file, associating it with the name.            

            // write new name and address.
            printf("%s","Please enter a Twitter Username (without the '@' symbol): ");            
            scanf("%s",Alias);
            while(getchar() != '\n');

            bool ValidPhoneNumber = false;
            do
            {
                printf("%s","Please enter your 10-digit phone number without spaces or punctuation. (e.g. 3149414314): ");            
                scanf("%s",PhoneNumber);

                if(strlen(PhoneNumber) == 10)
                {
                    ValidPhoneNumber = true;
                    for(int i = 0; i < strlen(PhoneNumber); ++i)
                    {
                        if(PhoneNumber[i] != '0' && PhoneNumber[i] != '1' && PhoneNumber[i] != '2' &&
                           PhoneNumber[i] != '3' && PhoneNumber[i] != '4' && PhoneNumber[i] != '5' &&
                           PhoneNumber[i] != '6' && PhoneNumber[i] != '7' && PhoneNumber[i] != '8' &&
                           PhoneNumber[i] != '9')
                        {
                            ValidPhoneNumber = false;
                            break;
                        }
                    }
                }
            
            while(getchar() != '\n');
            
            }while(!ValidPhoneNumber);

            
            AddressFile = fopen("UserAddresses.txt", "r+");
            
            fprintf(AddressFile,"%s ", Alias);
            fprintf(AddressFile,"%s\n", PhoneNumber);
            
            fclose(AddressFile);
            
        }
        else if(StatusInput == 2)
        {
            ValidInput = 1;
            printf("%s","Returning User Detected\n");
            // if returning, have them select their name from a list, and read in the file that corresponds with that name.

            AddressFile = fopen("UserAddresses.txt", "r");
            char NextChar = 0;
            
            for(int i = 0;i < 100; ++i)
            {
                
                NextChar = getc(AddressFile);
                if(NextChar != ' ')
                {
                    Alias[i] = NextChar;
                }
                else
                {
                    break;
                }
            }
            
            for(int i = 0;i < 100; ++i)
            {
                
                NextChar = getc(AddressFile);
                if(NextChar != '\n')
                {
                    PhoneNumber[i] = NextChar;
                }
                else
                {
                    break;
                }
            }
            
            fclose(AddressFile);
            AddressFile = NULL;
            printf("You will get a text at %s when @%s posts a new Tweet!\n", PhoneNumber, Alias);
        }
        else
        {
            while(getchar() != '\n');
            printf("%s","Invalid Input Detected\n");
            
        }
    }

    char PhoneNumberEmail[30] = "<";
    strcat(PhoneNumberEmail,PhoneNumber);
    strcat(PhoneNumberEmail,"@vtext.com>");
    char ToLine[50] = "To: ";
    strcat(ToLine, PhoneNumberEmail);
    strcat(ToLine, "\r\n");

    payload_text[1] = ToLine;

//    "To: " TO "\r\n", // 1 phone number
    
    //NOTE: Have this program call the code below as a separate winmain program that doesn't have a window?
    //Or have it start as a winmain program, and simply close the window at this point
    //E.G system("TwitterCheck.exe")
    

    //Initialize once for entire program
    curl_global_init(CURL_GLOBAL_ALL);

    // see https://curl.haxx.se/libcurl/c/curl_version_info.html for how to interpret
    // the returned struct
    curl_version_info_data * CurlVersionData = curl_version_info(CURLVERSION_NOW);

    FILE *NewTweet = NULL;
    NewTweet = fopen("w:\\TwitterTextsBuffer\\TwitterTexts\\data\\NewTweet.txt","r+");    
    FILE *OldTweet = NULL;        
    OldTweet = fopen("w:\\TwitterTextsBuffer\\TwitterTexts\\data\\OldTweet.txt","r+");

    char URLString[150] = "https://api.twitter.com/1.1/statuses/user_timeline.json?screen_name=";
    strcat(URLString,Alias);
    strcat(URLString,"&count=1&trim_user=1");
    
    CURL *EZHandle = curl_easy_init();
    CURLcode res = CURLE_OK;
    
    for(int i = 0; i < 2; ++i)
    {
        curl_easy_reset(EZHandle);
        Assert(curl_easy_setopt(EZHandle,CURLOPT_URL,URLString) == CURLE_OK);
        Assert(curl_easy_setopt(EZHandle,CURLOPT_HTTPGET)== CURLE_OK);
        Assert(curl_easy_setopt(EZHandle,CURLOPT_HTTPAUTH, CURLAUTH_BEARER) == CURLE_OK);
        Assert(curl_easy_setopt(EZHandle, CURLOPT_XOAUTH2_BEARER,"AAAAAAAAAAAAAAAAAAAAANIFEgEAAAAAC1%2F4cuMGUpNgGfdcfCteF%2F4JHSA%3DhSNOROobfSNHj3rrVJ3AnTi3i2X8NhZ6m7gBJut2NIK0NgB0XC") == CURLE_OK);
        if(i){Assert(curl_easy_setopt(EZHandle,CURLOPT_WRITEDATA,(void *)NewTweet) == CURLE_OK);}
        else{Assert(curl_easy_setopt(EZHandle,CURLOPT_WRITEDATA,(void *)OldTweet) == CURLE_OK);}
//        curl_easy_setopt(EZHandle, CURLOPT_VERBOSE, 1L);
        printf("%s","Trying pre perform\n");
        res = curl_easy_perform(EZHandle);
        if(res != CURLE_OK)
        {
            printf("%s"," pre perform failed\n");
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));
        }
        else
        {
            printf("%s"," pre perform passed\n");
        }
        if(!i){fclose(OldTweet);OldTweet = NULL;}
        else{rewind(NewTweet);}
    }
    
    bool GotNewTweet = false;

    SetThreadExecutionState(ES_CONTINUOUS | ES_SYSTEM_REQUIRED | ES_AWAYMODE_REQUIRED);
    //web-checking loop
    while(true)
    {

        //TODO:(close window) but have app run in the background

        //
        //
        //
        
        // get interface with website that allows one to notice (or preferably read updates).
        printf("%s","Trying perform\n");
        res = curl_easy_perform(EZHandle);
        if(res != CURLE_OK)
        {
            printf("%s","perform failed\n");
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));
        }
        else
        {
            printf("%s","perform passed\n");
        }
        
        rewind(NewTweet);
        OldTweet = fopen("w:\\TwitterTextsBuffer\\TwitterTexts\\data\\OldTweet.txt","r+");
        
        for(int i = 0; i < 37; ++i)
        {
            char NewChar = getc(NewTweet);
            char OldChar = getc(OldTweet);
            
            if(NewChar != OldChar)
            {
                GotNewTweet = true;
                break;
            }
        }

        if(GotNewTweet)
        {

            printf("%s","new tweet detected!");
            
            rewind(NewTweet);
            rewind(OldTweet);

            for(int i = 0; i < 37; ++i)
            {
                putc(getc(NewTweet),OldTweet);
            }

            //TODO: send data to email/phone from a program
            
            struct curl_slist *recipients = NULL;
            struct upload_status upload_ctx;

            upload_ctx.lines_read = 0;

            curl_easy_reset(EZHandle);
            
            curl_easy_setopt(EZHandle, CURLOPT_USERNAME, "1s22p63d104f14@gmail.com");
            curl_easy_setopt(EZHandle, CURLOPT_PASSWORD, "Cna874195?");
            curl_easy_setopt(EZHandle, CURLOPT_URL, "smtp://smtp.gmail.com:587");
            curl_easy_setopt(EZHandle, CURLOPT_USE_SSL, (long)CURLUSESSL_ALL);
            curl_easy_setopt(EZHandle, CURLOPT_CAINFO, "mail-google-com-chain.pem");    
            curl_easy_setopt(EZHandle, CURLOPT_MAIL_FROM, FROM);

            recipients = curl_slist_append(recipients, PhoneNumberEmail);
            recipients = curl_slist_append(recipients, CC);
            curl_easy_setopt(EZHandle, CURLOPT_MAIL_RCPT, recipients);
            curl_easy_setopt(EZHandle, CURLOPT_READFUNCTION, payload_source);
            curl_easy_setopt(EZHandle, CURLOPT_READDATA, &upload_ctx);
            curl_easy_setopt(EZHandle, CURLOPT_UPLOAD, 1L);
//            curl_easy_setopt(EZHandle, CURLOPT_VERBOSE, 1L);

            res = curl_easy_perform(EZHandle);
            if(res != CURLE_OK)
                fprintf(stderr, "curl_easy_perform() failed: %s\n",
                        curl_easy_strerror(res));

            curl_slist_free_all(recipients);

            curl_easy_reset(EZHandle);
            Assert(curl_easy_setopt(EZHandle,CURLOPT_URL,URLString) == CURLE_OK);
            Assert(curl_easy_setopt(EZHandle,CURLOPT_HTTPGET)== CURLE_OK);
            Assert(curl_easy_setopt(EZHandle,CURLOPT_HTTPAUTH, CURLAUTH_BEARER) == CURLE_OK);
            Assert(curl_easy_setopt(EZHandle, CURLOPT_XOAUTH2_BEARER,"AAAAAAAAAAAAAAAAAAAAANIFEgEAAAAAC1%2F4cuMGUpNgGfdcfCteF%2F4JHSA%3DhSNOROobfSNHj3rrVJ3AnTi3i2X8NhZ6m7gBJut2NIK0NgB0XC") == CURLE_OK);
            Assert(curl_easy_setopt(EZHandle,CURLOPT_WRITEDATA,(void *)NewTweet) == CURLE_OK);
//            curl_easy_setopt(EZHandle, CURLOPT_VERBOSE, 1L);
            
            GotNewTweet = false;
        }        
        
        fclose(OldTweet);
        OldTweet = NULL;
        fclose(NewTweet);
        NewTweet = NULL;
        Sleep(MinuteInMilliseconds*MinutesOfSleep);
        printf("%s","Sleepy Time is Over\n");
        NewTweet = fopen("w:\\TwitterTextsBuffer\\TwitterTexts\\data\\NewTweet.txt","r+");    
        
        
        
        
    }

    //call once program is completely done using libcurl
    fclose(NewTweet);
    curl_easy_cleanup(EZHandle);
    curl_global_cleanup();
    SetThreadExecutionState(ES_CONTINUOUS);
    
    return 0;
}
