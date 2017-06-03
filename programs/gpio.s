ldr r0,=0x20200000
mov r1,#0x40000
mov r3,#0x10000
str r1,[r0,#4]
str r3,[r0,#0x28]
loop:
ldr r2,=0xFFFFFF
wait1:
sub r2,r2,#1
cmp r2,#0
bne wait1
str r3,[r0,#0x1c]
ldr r2,=0xFFFFFF
wait:
sub r2,r2,#1
cmp r2,#0
bne wait
str r3,[r0,#0x28]
b loop
