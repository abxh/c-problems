# rotating_starfield

This problem puts some extra "spice" onto the problem of bitwise rotation posed in K & R chapter 2 exercise 8.

**Problem:**

Generate a "sparse" random bit pattern of 1's and 0's of size 8 x 64 (or 8 x 32 if your machine is 32-bit),
and rotate the bit patterns left to right or right to left as time passes. It is "sparse" in the sense that
there are a lot of 0's compared to 1's. You are left to decide what "a lot" means.

Continously print each row of the bit pattern using blank (` `) to represent 0 and star (`*`) to represent 1.

You can use ascii escape codes to clear lines and move up lines.
