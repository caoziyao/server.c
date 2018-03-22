
lua_lib = -llua -Ilua-5.3.4/src -Llua-5.3.4/src
oppenssl_lib = -lssl -lcrypto -I/usr/local/opt/openssl/include -L/usr/local/opt/openssl/lib
src_lib = -Isrc -Lsrc
test_lib = -Itest -Ltest
worker_lib = -I. -L.

all:
	cc *.c $(worker_lib) $(src_lib) $(test_lib) $(lua_lib) $(oppenssl_lib) && ./a.out
clean:
	rm  *.o
