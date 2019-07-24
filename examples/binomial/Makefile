include ./Makefile.conf

.DEFAULT_GOAL=runtests

.PHONY : runtests
runtests: binomial expr.txt
	time ./binomial
	time form eval.frm

expr.txt: form_to_exparse.py expr.out
	python form_to_exparse.py

expr.out: gen.frm
	form gen.frm

.PHONY : clean very-clean
clean:
	rm -rf *.o *.out binomial expr.out expr.txt expr.frm

very-clean: clean
