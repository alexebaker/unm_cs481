typedef int sem_t;

void s_wait(sem_t *s)
{
    while (*s <= 0) ;
    *s = *s - 1;
    return;
}

void s_signal(sem_t *s)
{
    *s = *s + 1;
    return;
}
