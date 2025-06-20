#!/bin/sh

dir=$(cd $(dirname $0); pwd)

seed=123
if [ $# -ge 1 ]; then
    seed=$1
fi
opt=""
if [ $# -ge 2 ]; then
    opt=$2
fi
echo seed = $seed

before_process=$(lsof -t -i:6562)

if [ -n "$before_process" ]; then
    echo "kill old auction-worker..."
    kill $before_process
    sleep 1
fi

for datasize in small medium large challenge
do
    echo "--------------------$datasize--------------------"
    if [ "$datasize" != "challenge" ]; then
        echo -n "init DB data...   "
        $dir/setup_db.sh $seed $datasize
        echo "done."
    else
        sleep 0.5
    fi
    echo "run auction-worker..."
    make -C $dir/.. inner-run > $dir/stdout.log 2> $dir/stderr.log &
    echo "wait 5 second for setup auction worker..."
    sleep 5
    nice -n19 ionice -c3 python3 $dir/calculate_performance.py $seed $datasize $opt
    code=$?

    kill $(lsof -t -i:6562)
    if [ $code -ne 0 ]; then
        echo "\e[31mFAILED...\e[m"
        exit 1
    else
        echo "\e[32mPASSED\e[m"
    fi
done
echo "\e[32mALL TEST PASSED\e[m"
