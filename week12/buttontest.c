int main(int argc, char *argv[])
{
    int fp;
    int readSize, inputIndex;
    struct input_event stEvent;
    
    if (buttonInit() == 0)
    {
        printf("ERROR! File Not Found!\r\n");
        printf("Did you insmod?\r\n");
        return 0;
    }

    buttonStatus();
    
    close(fp);
}