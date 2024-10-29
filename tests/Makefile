
TESTS= basic-builtins \
       basic-arrays   \
       basic-packing

HEADERS= $(addsuffix .h, ${TESTS})
CFLAGS=-I. -I ..

%.h: %.bs
	( cd .. ; python example.py tests/$< > tests/$@ )

all: $(HEADERS) $(TESTS)

clean:
	-rm $(TESTS)
	-rm $(HEADERS)
