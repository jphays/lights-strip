// Utilities
// ---------

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

uint8_t random8Except(uint8_t max, uint8_t except)
{
    // returns a random number 0 <= r < max, but won't return r == except.
    // useful for picking a random array value different from the previous one.
    uint8_t r = random8(max);
    while (r == except && max > 1)
    {
        r = random8(max);
    }
    return r;
}

