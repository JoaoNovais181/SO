for ((i=0 ; i<= $1 ; i++))
do
    IDADE=$(((RANDOM % 100)+1))
    # echo "./pessoas -o ${i} ${IDADE}"

    ./pessoas -o ${i} $IDADE
done