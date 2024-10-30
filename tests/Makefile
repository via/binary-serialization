
TESTS= basic-builtins \
       basic-arrays   \
       basic-packing \
       basic-unions

HEADERS= $(addsuffix .h, ${TESTS})
CFLAGS=-I. -I ..

%.h: %.bs
	( cd .. ; python example.py tests/$< > tests/$@ )

all: $(HEADERS) $(TESTS)

check: all
	for t in ${TESTS}; do ./$$t ; echo $$t: $$? ; done

clean:
	-rm $(TESTS)
	-rm $(HEADERS)
