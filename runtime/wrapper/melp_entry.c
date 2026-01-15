// MELP Stage 1 - C Runtime Wrapper
// Provides entry point for MELP programs

#include <stdio.h>
#include <stdlib.h>

// MELP main function (returns i64)
extern long long melp_main(void);

int main(int argc, char** argv) {
    long long result = melp_main();
    printf("Program exited with code: %lld\n", result);
    return (int)result;
}
