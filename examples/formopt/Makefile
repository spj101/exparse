include ./Makefile.conf

.DEFAULT_GOAL=runtests

.PHONY : runtests
runtests: formopt expr.txt
	time ./formopt

expr.txt: form_to_exparse.py expr.out
	python form_to_exparse.py

expr.out: gen.frm
	form gen.frm

.PHONY : clean very-clean
clean:
	rm -rf *.o *.out formopt expr.txt

very-clean: clean
