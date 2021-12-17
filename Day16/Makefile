default: PacketDecoder

%: %.c
	clang -std=c11 -Wall -pedantic -g $@.c -lm -o $@ \
		 -fsanitize=undefined -fsanitize=address
