void bar()
{
    
}

int baz()
{
    bar();
    return 0;
}

void aaa();

void foo(void)
{
    bar();
    baz();
    bar();
}