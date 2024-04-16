.code16

.global _start

_start:
    mov $0xe, %ah

    mov $'H', %al
	int $0x10

	mov $'e', %al
	int $0x10

	mov $'l', %al
	int $0x10

	mov $'l', %al
	int $0x10

	mov $'o', %al
	int $0x10

	mov $0x20, %al
	int $0x10

	mov $'W', %al
	int $0x10

	mov $'o', %al
	int $0x10

	mov $'r', %al
	int $0x10

	mov $'l', %al
	int $0x10

	mov $'d', %al
	int $0x10

halt:
    hlt
    jmp halt

    .fill 510 - (. - begin), 1, 0
    .word 0xaa55
