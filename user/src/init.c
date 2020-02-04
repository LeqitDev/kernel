
int i = 0;
void _start(void)
{
    int j = i + 5;
    for (; i < j; i++) {
        asm("int $0x30" : : "a" (0), "b" ('0' + i));
    }

    while(1);
}
