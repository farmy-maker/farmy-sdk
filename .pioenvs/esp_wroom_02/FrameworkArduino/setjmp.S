 /* 
 *setjmp/longjmp function implementations from newlib
 * https://github.com/projectgus/newlib-xtensa/blob/xtensa-2_2_0-lock/newlib/libc/machine/xtensa/setjmp.S
 * Required by PlatformIO unit testing feature
 */

    .text
    .align  4
    .literal_position
    .global setjmp
    .type   setjmp, @function
setjmp:
    s32i    a0, a2, 0
    s32i    a1, a2, 4
    s32i    a12, a2, 8
    s32i    a13, a2, 12
    s32i    a14, a2, 16
    s32i    a15, a2, 20
    movi    a2, 0
    ret
    .size   setjmp, . - setjmp

    .align  4
    .literal_position
    .global longjmp
    .type   longjmp, @function
longjmp:
    l32i    a0, a2, 0
    l32i    a12, a2, 8
    l32i    a13, a2, 12
    l32i    a14, a2, 16
    l32i    a15, a2, 20
    l32i    a1, a2, 4
    /* Return val ? val : 1.  */
    movi   a2, 1
    movnez a2, a3, a3

    ret
    .size   longjmp, .-longjmp