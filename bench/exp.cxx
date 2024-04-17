#include <xprec/ddouble.h>
#include <iostream>
#include <time.h>

using xprec::DDouble;

double elapsed(const struct timespec *start, const struct timespec *end)
{
    return end->tv_sec - start->tv_sec + 1e-9 * (end->tv_nsec - start->tv_nsec);
}

int main()
{
    const long n = 10000000;
    double x = 0;
    DDouble r = 1.0;

    struct timespec c_start, c_end;
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &c_start);
    for (int i = 0; i != n; ++i) {
        x += 10.0/n;
        r *= sin(DDouble(x));
        r *= sin(DDouble(-x));
    }
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &c_end);
    double c_elapsed = elapsed(&c_start, &c_end);

    std::cout << r << std::endl;
    //std::cout << c_elapsed << std::endl;
    std::cout << c_elapsed/(2*n*1e-9) << " ns/call" <<std::endl;
}
