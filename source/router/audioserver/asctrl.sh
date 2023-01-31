#!/bin/sh
RUNNING_DIR="/sbin"
EXE_NAME="audioserver"

check_running() {
pid=`ps | grep "/sbin/audioserver" | grep -v grep | awk -F' ' '{print $1}'`
if [ "$pid" !=  "" ]
then
    echo "target pid is exist "$pid""
    return 0
fi

if [ "$pid" =  "" ]
then
    echo "target pid is not exist "
    return 1
fi
}

start_audioserver() {
    local exefile="${RUNNING_DIR}/${EXE_NAME}"

    if [ -e "${exefile}" ];then
		/sbin/audioserver -p 34508 2>&1 > /var/log/audioserver.log &
        return
    fi

}
stop_audioserver() {
	killall -9 madplay
	killall -9 audioserver
}

check_audioserver() {
    check_running
    [ "$?" = "0" ] && return
    start_audioserver
    return
}

if [ $1 = "restart" ]
then
	echo "stop audioserver"
	stop_audioserver
	echo "start audioserver"
	start_audioserver
	exit
fi

echo "start monitor..."
while :
do
    check_audioserver
    sleep 10 
done

