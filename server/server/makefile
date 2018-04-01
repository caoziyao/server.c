
lua_lib = -llua -Ilua-5.3.4/src -Llua-5.3.4/src
oppenssl_lib = -lssl -lcrypto -I/usr/local/opt/openssl/include -L/usr/local/opt/openssl/lib
code_lib = core/*.c -Icore -Lcore
event_lib = event/*.c -Ievent -Levent
os_lib = os/*.c -Ios -Los
upstream_lib = upstream/*.c -Iupstream -Lupstream
worker_lib = -I. -L.

all:
	cc *.c $(worker_lib) $(upstream_lib) $(event_lib) $(os_lib) $(code_lib) $(lua_lib) $(oppenssl_lib) && ./a.out
clean:
	rm  *.o
