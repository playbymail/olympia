
cpio:
	-mv ../g1.cpio ../g1.cpio-
	find * -depth -print | \
	egrep -v '(^|/)(.*\.o|core|oly|tags|old|log|.*\.shar.*|.*\.out|loc|gate|road|master|.*-|.*\.old)($$|/)' | \
					$(SYS_CPIO) > ../g1.cpio
	chmod go-rwx ../g1.cpio*
