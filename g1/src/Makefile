
CC=cc
CFLAGS = -g

OLYOBJ =	add.o adv.o alchem.o art.o basic.o beast.o build.o buy.o \
		check.o c1.o c2.o cloud.o code.o combat.o day.o dir.o \
		display.o eat.o faery.o garr.o gate.o glob.o gm.o hades.o \
		immed.o input.o io.o loc.o lore.o main.o make.o move.o \
		necro.o npc.o order.o perm.o produce.o quest.o relig.o \
		report.o savage.o scry.o seed.o sout.o stack.o stealth.o \
		storm.o summary.o swear.o u.o use.o

OBJECTS = $(OLYOBJ) z.o

HEADERS =	loop.h code.h dir.h display.h etc.h garr.h loc.h order.h \
		sout.h stack.h swear.h u.h use.h

all:	oly

oly:	$(OBJECTS)
	$(CC) $(CFLAGS) -o oly $(OBJECTS) $(LFLAGS)

shar:
	-mv ../o.shar ../o.shar-
	shar -v [A-Z]* *.[ch] *.sh lib/item lib/skill lib/lore > ../o.shar

tar:
	-mv ../../g1.tar ../../g1.tar-
	tar -cvf ../../g1.tar [A-Z]* *.[ch] lib/item lib/skill lib/lore

ztar:
	tar -czf ../../g1.tar.Z [A-Z]* *.[ch] lib/item lib/skill lib/lore

clean:
	rm -f *.o

clobber:	clean
	rm -f oly

diff:
	@sh diff.sh

cpio:
	cd ..; make cpio

tags:
	ctags -t *.[ch]


$(OLYOBJ):	z.h oly.h $(HEADERS)
z.o:		z.h
