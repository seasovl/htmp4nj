#include <stdio.h>
#include <string.h>
#include <memory.h>
#include <malloc.h>

#define LENGTH 256
static char* dic[] = {"北京", "大学", "北京大学"};

#define NUMBER (sizeof(dic) / sizeof(char*))
static char* buffer[LENGTH] = {0};
static int max_len = 0;
static int min_len = 0;

static int  find_max_length()
{
    int index;
    unsigned int len;

    len = 0;
    for(index = 0; index < NUMBER; index ++)
    {
        if(len < strlen(dic[index]))
        {
            len = strlen(dic[index]);
        }
    }

    return len;
}

static int find_min_length()
{
    int index;
    unsigned int len;

    len = strlen(dic[0]);

    for(index = 1; index < NUMBER; index++)
    {
        if(strlen(dic[index]) < len)
        {
            len = strlen(dic[index]);
        }
    }

    return len;
}

static void show_buffer(int end)
{
    int start;

    for(start = 0; start < end; start ++)
    {
        printf("%s ", buffer[start]);
    }

    printf("\n");
}

static void _process_segment(char* str, int index)
{
    int start;
    int len ;
    int  found;
    char* data;
    char* label;

    if('\0' == *str)
    {
        show_buffer(index);
        return;
    }

    label = str + strlen(str);

retry:
    len = strlen(str);
    if(len > LENGTH)
    {
        len = LENGTH;
    }

    if(len > max_len)
    {
        len = max_len;
    }

    found = 0;
    while(len >= min_len)
    {
        for(start = 0; start < NUMBER; start ++)
        {
            if(0 == strncmp(str, dic[start], len))
            {
                found = 1;
                break;
            }
        }

        if(found)
        {
            break;
        }

        len --;
    }

    /* if no str was found, just step forward, but cannot beyond label */
    if(len < min_len && str < label)
    {
        str ++;
        goto retry;
    }

    /* if no str was left, show all the str */
    if(str >= label)
    {
        show_buffer(index);
        return;
    }

    data = (char*) malloc(len + 1);
    if(NULL == data)
    {
        return;
    }

    data[len] = '\0';
    memmove(data, str, len);
    buffer[index] = data;

    _process_segment(str + len, index + 1);
    free(data);
    buffer[index] = NULL;
}

void process_segment(char* str)
{
    int length;

    if(NULL == str)
    {
        return;
    }

    length = strlen(str);
    if(length > LENGTH)
    {
        return;
    }

    _process_segment(str, 0);
}

void segment_init()
{
    min_len = find_min_length();
    max_len = find_max_length();
    memset(buffer, 0,  sizeof(buffer));
}

int main(int argc, char* argv[])
{
    segment_init();
    process_segment("北京 大学 日本");
    return 1;
}
