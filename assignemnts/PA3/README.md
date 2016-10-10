# Programming Assignment 3

Usage:
```bash
make
./buffer <sleep time> <producer threads> <consumer threads>
```

Example:
```bash
./buffer 10 1 1
```

The `buffer.h` file contains definitions relevent for the working buffer. `my_semaphore.h` includes the books implementation of semaphores, since the `semaphore.h` library does not work on osx. The main program file contains all of the logic and threading.
