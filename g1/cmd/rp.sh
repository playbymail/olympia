#!/bin/sh

WHO=$1
TURN=$2

case $TURN in
	"" )
		DIR=/u/oly/g1/lib/log
		;;
	"-" )
		DIR=/u/oly/g1/lib/save/`turn`
		;;
	* )
		DIR=/u/oly/g1/lib/save/$TURN
		;;
esac

case $WHO in
[0-9]* )
	;;
* )
	WHO=`conv $WHO`
	;;
esac

if [ -f $DIR/$WHO.Z ]
then
	zcat $DIR/$WHO.Z > /tmp/$WHO.$$
	rep /tmp/$WHO.$$
	rm -f /tmp/$WHO.$$
else
	rep $DIR/$WHO
fi
