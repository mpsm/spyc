void bar()
{
    
}

int baz()
{
    bar();
    return 0;
}

void foo(void)
{
    bar();
    baz();
    bar();
}