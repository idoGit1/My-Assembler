label:
mov ax, 5
jmp label

%macro print %3:
mov ax, %1
mov %2, %3
%endmacro
