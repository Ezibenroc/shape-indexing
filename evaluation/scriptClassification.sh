#!/bin/zsh
zmodload -i zsh/mathfunc

##YOUR Classification progname
CLASSIFPROG="./classify -c classes.csv -d descriptors.info -i"

#nb of noisyfied copies
NBTESTS=1

#No noise here
MAXNOISE=0.0
sum=0.0
variance=0.0
cpt=0.0

for ((CLASSID=1; CLASSID <= 70; CLASSID++)); do
    ##Pick a random class
    CLASSNAME=`head -$CLASSID classes.csv | tail -1 | sed -e 's/,//'`
    echo "Classname: "$CLASSNAME

    ##Pick a random image in this class
    IMGID=`expr $RANDOM % 14 + 1`
    IMGNAME=database/$CLASSNAME-$IMGID.pgm
    echo $IMGNAME

    if [[ -e $IMGNAME ]]; then
        for ((j=0; j < $NBTESTS; j+=1)); do
            ##Random scale+rotation
            ANGLE=$((rand48()*3.1415))
            SCALE=$((rand48()*3))
            NOISE=$((rand48()*0.5))
            ./imgRotate -i $IMGNAME -o tmp_$$.pgm -a $ANGLE 2> /dev/null
            ./imgScale -i tmp_$$.pgm -o tmp2_$$.pgm -s $SCALE 2> /dev/null
            ./imgAddNoise -i tmp2_$$.pgm -o tmp_$$.pgm -n $NOISE  2> /dev/null

            ##Running the retrieval
            eval `echo $CLASSIFPROG tmp_$$.pgm` >! scores_tmp_$$.txt || cp tmp_$$.pgm error_`date +%Y-%m-%d_%H-%M-%S_`$$.pgm
            RANK=` ./getRank scores_tmp_$$.txt $CLASSID`
            echo "Rank=$RANK"

            ##Number of correct results in the first 10
            if [[ $RANK -le 10 ]]; then
               correct=$(($correct + 1.0))
            fi
            cpt=$(($cpt + 1.0))
        done
    fi
done
echo
echo "Number of 'correct' classification (first 10 classes)= "$correct
echo "Total number of tests= "$cpt
echo "Final score= " $(( $correct / $cpt ))
rm tmp_$$.pgm
rm tmp2_$$.pgm
rm scores_tmp_$$.txt
