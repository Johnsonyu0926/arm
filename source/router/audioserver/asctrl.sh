//asctrl.sh
#!/bin/sh
RUNNING_DIR="/sbin"
EXE_NAME="audioserver"

logname=/var/log/audioserver.log
logbakname=/var/log/audioserver.log.bak
max_size=4096000

save_prefix=`cm get_val sys saveprefix | tail -1`
mp3cfg_filename=add_column_custom_audio_file.json
origin_mp3dir=/usr/lib/mp3
dest_mp3dir=$save_prefix/audiodata
dest_mp3cfg_dir=$save_prefix/cfg

init_dest_mp3_dir() {
	mkdir -p $dest_mp3dir
	mkdir -p $dest_mp3cfg_dir
	return 0
}

init_mp3() {
	if [ -d $origin_mp3dir ] ; then
	init_dest_mp3_dir
	cp $origin_mp3dir/*.mp3 $dest_mp3dir
	cp $origin_mp3dir/$mp3cfg_filename $dest_mp3cfg_dir
	fi
	return 0
}

# check mp3 config file is exist already. 
# return
# 1 - exist
# 0 - not exist
check_mp3_cfg_file() {

 	if [ -f $dest_mp3cfg_dir/$mp3cfg_filename ]
	then
		return 1
	fi
	return 0
}

check_log_size() {
sizelog=`ls -l $logname | awk '{print $5}'`
echo $sizelog
if [ $sizelog -gt $max_size ]; then
	return 1
else
	return 0
fi
}

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

	check_mp3_cfg_file
	if [ "$?" = "0" ] ; then
		init_mp3
	fi
	
    local exefile="${RUNNING_DIR}/${EXE_NAME}"

    if [ -e "${exefile}" ];then
    	if [ -f "/mnt/cfg/log_flag" ];then
		/sbin/audioserver -p 34508 2>&1 > /var/log/audioserver.log &
    	else
    	 	/sbin/audioserver -p 34508 2>&1 > /dev/null &
    	fi
    fi
}
stop_audioserver() {
	killall -9 madplay
	killall -9 ffmpeg
	killall -9 ffplay
	killall -9 audioserver
}
backup_log() {
	cp $logname $logbakname
	echo > $logname
}
check_audioserver() {
    check_running
    [ "$?" = "0" ] && return
    start_audioserver
    return
}
check_log_backup() {
	check_log_size
	[ "$?" = "0" ] && return
	backup_log
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
	# check_log_backup
    sleep 10 
done

